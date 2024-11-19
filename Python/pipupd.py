import sys
import subprocess
import requests
from PyQt6.QtWidgets import QApplication, QWidget, QVBoxLayout, QLabel, QListWidget, QListWidgetItem, QPushButton, QMessageBox, QProgressBar, QHBoxLayout
from PyQt6.QtCore import Qt, QThread, pyqtSignal

def get_outdated():
    outdated = []
    try:
        result = subprocess.run(['pip', 'list', '--outdated'], capture_output=True, text=True, check=True)
        lines = result.stdout.splitlines()[2:]
        for line in lines:
            parts = line.split()
            if len(parts) >= 3:
                outdated.append((parts[0], parts[1], parts[2]))  # package_name, old_version, new_version
    except subprocess.CalledProcessError as e:
        QMessageBox.critical(None, "Error", f"Error getting pip packages: {e}")
    return outdated

class PackageUpdater(QThread):
    update_finished = pyqtSignal(str)
    update_failed = pyqtSignal(str, str)  # package_name, error_message

    def __init__(self, packages):
        super().__init__()
        self.packages = packages

    def run(self):
        for package_name in self.packages:
            try:
                subprocess.run(['pip', 'install', '--upgrade', package_name], check=True)
                self.update_finished.emit(package_name)
            except subprocess.CalledProcessError as e:
                self.update_failed.emit(package_name, str(e))

class OutdatedChecker(QThread):
    outdated_packages_found = pyqtSignal(list)

    def run(self):
        outdated = get_outdated()
        self.outdated_packages_found.emit(outdated)

class PypiPackageFetcher(QThread):
    packages_fetched = pyqtSignal(list)

    def run(self):
        try:
            response = requests.get("https://pypi.org/pypi?%3Aaction=list")
            packages = response.text.splitlines()[:50]
            self.packages_fetched.emit(packages)
        except Exception as e:
            QMessageBox.critical(None, "Error", f"Error fetching packages from PyPI: {e}")

class App(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Pip Package Manager")
        self.setGeometry(100, 100, 600, 400)

        layout = QVBoxLayout()
        self.label = QLabel("Outdated packages:")
        layout.addWidget(self.label)

        self.list_widget = QListWidget()
        layout.addWidget(self.list_widget)

        self.update_button = QPushButton("Update all packages")
        layout.addWidget(self.update_button)

        layout.addStretch()

        self.progress_bar = QProgressBar()
        layout.addWidget(self.progress_bar)

        self.fetch_button = QPushButton("Fetch PyPI Packages")
        layout.addWidget(self.fetch_button)

        self.pypi_list_widget = QListWidget()
        layout.addWidget(self.pypi_list_widget)

        layout.addStretch()

        self.update_button.clicked.connect(self.update_all_packages)
        self.fetch_button.clicked.connect(self.fetch_pypi_packages)
        self.setLayout(layout)

        self.populate_outdated()

    def populate_outdated(self):
        self.list_widget.clear()
        self.progress_bar.setValue(0)
        self.progress_bar.setMaximum(0)

        self.checker_thread = OutdatedChecker()
        self.checker_thread.outdated_packages_found.connect(self.display_outdated_packages)
        self.checker_thread.start()

    def display_outdated_packages(self, outdated_packages):
        self.progress_bar.setMaximum(1)
        self.progress_bar.setValue(1)
        self.outdated_packages = outdated_packages
        for package_name, old_version, new_version in self.outdated_packages:
            item = QListWidgetItem(f"{package_name} - {old_version} -> {new_version}", self.list_widget)
            item.setData(Qt.ItemDataRole.UserRole, package_name)

    def update_all_packages(self):
        if not self.outdated_packages:
            print("Nothing to update")
            return
        package_names = [pkg[0] for pkg in self.outdated_packages]
        self.thread = PackageUpdater(package_names)
        self.thread.update_finished.connect(self.remove_package_item)
        self.thread.update_failed.connect(self.show_error_message)
        self.thread.start()

    def remove_package_item(self, package_name):
        for index in range(self.list_widget.count()):
            item = self.list_widget.item(index)
            if item.data(Qt.ItemDataRole.UserRole) == package_name:
                self.list_widget.takeItem(index)
                break

    def show_error_message(self, package_name, error_message):
        QMessageBox.critical(self, "Update Failed", f"Failed to update {package_name}: {error_message}")

    def fetch_pypi_packages(self):
        self.pypi_list_widget.clear()
        self.progress_bar.setValue(0)
        self.progress_bar.setMaximum(0)

        self.fetcher_thread = PypiPackageFetcher()
        self.fetcher_thread.packages_fetched.connect(self.display_pypi_packages)
        self.fetcher_thread.start()

    def display_pypi_packages(self, packages):
        self.progress_bar.setMaximum(1)
        self.progress_bar.setValue(1)
        for package_name in packages:
            self.fetch_package_details(package_name)

    def fetch_package_details(self, package_name):
        try:
            response = requests.get(f"https://pypi.org/pypi/{package_name}/json")
            data = response.json()
            version = data['info']['version']
            description = data['info']['summary']
            item = QListWidgetItem(f"{package_name} - {version}: {description}", self.pypi_list_widget)
            install_button = QPushButton("Install")
            install_button.clicked.connect(lambda: self.install_package(package_name))
            self.pypi_list_widget.addItem(item)
            self.pypi_list_widget.setItemWidget(item, install_button)
        except Exception as e:
            QMessageBox.critical(self, "Error", f"Error fetching details for {package_name}: {e}")

    def install_package(self, package_name):
        try:
            subprocess.run(['pip', 'install', package_name], check=True)
            QMessageBox.information(self, "Success", f"{package_name} installed successfully.")
        except subprocess.CalledProcessError as e:
            QMessageBox.critical(self, "Installation Failed", f"Failed to install {package_name}: {e}")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = App()
    window.show()
    sys.exit(app.exec())