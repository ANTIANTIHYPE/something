import sys, subprocess
from PyQt6.QtWidgets import QApplication, QWidget, QVBoxLayout, QLabel, QListWidget, QListWidgetItem, QPushButton
from PyQt6.QtGui import QColor
from PyQt6.QtCore import Qt

def get_outdated():
    outdated = list()

    try:
        result = subprocess.run(['pip', 'list', '--outdated'], capture_output=True, text=True, check=True)
        lines = result.stdout.splitlines()[2:]
        for line in lines:
            parts = line.split()
            if len(parts) >= 3:
                package_name = parts[0]
                old_version = parts[1]
                new_version = parts[2]
                outdated.append((package_name, old_version, new_version))
    except subprocess.CalledProcessError as e:
        print(f"Error getting pip packages: {e}")

    try:
        result = subprocess.run(['pipx', 'list'], capture_output=True, text=True, check=True)
        lines = result.stdout.splitlines()
        for line in lines:
            if "outdated" in line:
                parts = line.split()
                package_name = parts[0]
                old_version = parts[2]
                new_version = parts[4]
                outdated.append((package_name, old_version, new_version))
    except subprocess.CalledProcessError as e:
        print(f"Error getting pipx packages: {e}")

    return outdated

class App(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Outdated Packages")
        self.setGeometry(100, 100, 400, 300)

        layout = QVBoxLayout()

        self.label = QLabel("Outdated Packages:")
        layout.addWidget(self.label)

        self.list_widget = QListWidget()
        layout.addWidget(self.list_widget)

        self.update_button = QPushButton("Update All Packages")
        layout.addWidget(self.update_button)

        self.update_button.clicked.connect(self.update_all_packages)
        self.setLayout(layout)

        self.populate_outdated()

    def populate_outdated(self):
        outdated = get_outdated()
        for package_name, old_version, new_version in outdated:
            item = QListWidgetItem(f"{package_name} - ", self.list_widget)
            item.setText(f"{package_name} - {old_version} -> {new_version}")
#            item.setForeground(QColor("red"))

    def update_all_packages(self):
        outdated = get_outdated()
        for package_name, _, _ in outdated:
            print(f"Updating {package_name}...")
            try:
                subprocess.run(['pip', 'install', '--upgrade', package_name], check=True)
#                print(f"Successfully updated {package_name}")
            except subprocess.CalledProcessError as e:
                print(f"Failed to update {package_name}: {e}")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = App()
    window.show()
    sys.exit(app.exec())