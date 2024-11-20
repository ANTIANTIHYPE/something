import sys, subprocess
from PyQt6.QtWidgets import QApplication, QWidget, QVBoxLayout, QLabel, QListWidget, QListWidgetItem, QPushButton, QMessageBox, QProgressBar
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

class App(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Pip Package Updater")
        self.setGeometry(100, 100, 400, 300)

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

        layout.addStretch()

        self.update_button.clicked.connect(self.update_all_packages)
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
        QMessageBox.warning(self, "Update Failed", f"Failed to update {package_name}:\n{error_message}")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = App()
    window.show()
    sys.exit(app.exec())
