import tkinter as tk
import pyautogui
import keyboard
import time
import threading
import configparser
import os

# Load or create configuration
config = configparser.ConfigParser()
cfg_name = 'config.cfg'
if os.path.isfile(cfg_name):
    config.read(cfg_name)
else:
    config['Hotkeys'] = {
        'start_hotkey': 'PgUp',
        'add_coordinate_hotkey': 'equal'
    }
    with open(cfg_name, 'w') as configfile:
        config.write(configfile)

# Initialize hotkeys and variables
start_hotkey = config.get('Hotkeys', 'start_hotkey')
add_coordinate_hotkey = config.get('Hotkeys', 'add_coordinate_hotkey')
coordinates = []  # (x, y)
autoclicker_on = False
adding = False

def toggle() -> None:
    global autoclicker_on
    autoclicker_on = not autoclicker_on
    update_status()

def update_status() -> None:
    status_msg = "Autoclicker is ON. Press \"{}\" to turn it OFF.".format(start_hotkey) if autoclicker_on else "Autoclicker is OFF. Press \"{}\" to turn it ON.".format(start_hotkey)
    status_label.config(text=status_msg)

def start() -> None:
    global autoclicker_on
    autoclicker_on = True
    update_status()

def stop() -> None:
    global autoclicker_on
    autoclicker_on = False
    update_status()

def add_coordinate() -> None:
    global adding
    if not adding:
        adding = True
        overlay = tk.Toplevel(root)
        overlay.attributes('-fullscreen', True)
        overlay.configure(bg='black')
        overlay.attributes('-alpha', 0.5)
        overlay.attributes('-topmost', True)
        overlay.bind("<Button-1>", lambda event: get_coordinate(event, overlay))
        overlay.config(cursor="crosshair")

def get_coordinate(event, overlay) -> None:
    global adding
    coordinates.append((event.x_root, event.y_root))
    coordinate_list.insert(tk.END, f"({event.x_root}, {event.y_root})")
    print(f"Added coordinate: ( X: {event.x_root}, Y: {event.y_root} )")
    overlay.destroy()
    adding = False

def handle_hotkey(event) -> None: toggle()
def handle_add_coordinate(event) -> None: add_coordinate()

def listen() -> None:
    keyboard.on_press_key(start_hotkey, handle_hotkey)
    keyboard.on_press_key(add_coordinate_hotkey, handle_add_coordinate)

def choose_hotkey(action) -> None:
    def set_hotkey(event):
        global start_hotkey, add_coordinate_hotkey
        key_mapping = {
            'prior': 'PgUp',
            'next': 'Page_Down',
            'control_l': 'Ctrl',
            'control_r': 'Ctrl',
            'alt_l': 'Alt',
            'alt_r': 'Alt',
            'shift_l': 'Shift',
            'shift_r': 'Shift'
        }
        if action == 'start':
            start_hotkey = key_mapping.get(event.keysym.lower(), event.keysym)
            config.set('Hotkeys', 'start_hotkey', start_hotkey)
        elif action == 'add':
            add_coordinate_hotkey = key_mapping.get(event.keysym.lower(), event.keysym)
            config.set('Hotkeys', 'add_coordinate_hotkey', add_coordinate_hotkey)

        with open(cfg_name, 'w') as configfile:
            config.write(configfile)
        update_hotkey_labels()
        keyboard.unhook_all_hotkeys()
        listen()
        hotkey_window.destroy()

    hotkey_window = tk.Toplevel(root)
    hotkey_window.title("Choose Hotkey")
    tk.Label(hotkey_window, text=f"Press the key for \"{action}\" action:").pack(pady=20)
    hotkey_window.bind("<KeyPress>", set_hotkey)
    tk.Label(hotkey_window, text="Press any key to set it as the hotkey.").pack(pady=10)

def delete_coordinates() -> None:
    selected = coordinate_list.curselection()
    for index in reversed(selected):
        coordinate_list.delete(index)
        del coordinates[index]

def update_hotkey_labels() -> None:
    status_label.config(text=f"Autoclicker is OFF. Press \"{start_hotkey}\" to turn it ON.")
    add_button.config(text=f"Add Coordinate (Hotkey: \"{add_coordinate_hotkey.upper()}\")")

def main_loop() -> None:
    while True:
        if autoclicker_on and coordinates:
            for x, y in coordinates:
                pyautogui.click(x, y)
                time.sleep(0.1)
        time.sleep(0.1)
        root.update()

root = tk.Tk()
root.title("Autoclicker")
root.resizable(False, False)
root.wm_iconphoto(False, tk.PhotoImage(file=r"./death.png"))

# Create UI elements
start_button = tk.Button(root, text="Start", command=start)
stop_button = tk.Button(root, text="Stop", command=stop)
add_button = tk.Button(root, text=f"Add Coordinate (Hotkey: \"{add_coordinate_hotkey.upper()}\")", command=add_coordinate)
hotkey_button = tk.Button(root, text="Choose Start Hotkey", command=lambda: choose_hotkey('start'))
add_hotkey_button = tk.Button(root, text="Choose Add Coordinate Hotkey", command=lambda: choose_hotkey('add'))
status_label = tk.Label(root, text="Autoclicker is OFF. Press \"{}\" to turn it ON.".format(start_hotkey))
delete_button = tk.Button(root, text="Delete Selected", command=delete_coordinates)
coordinate_list = tk.Listbox(root, width=30, selectmode=tk.MULTIPLE)

# Arrange UI elements in grid
start_button.grid(row=0, column=0, padx=10, pady=10)
stop_button.grid(row=0, column=1, padx=10, pady=10)
add_button.grid(row=0, column=2, padx=10, pady=10)
hotkey_button.grid(row=0, column=3, padx=10, pady=10)
add_hotkey_button.grid(row=0, column=4, padx=10, pady=10)
status_label.grid(row=1, column=0, columnspan=5, padx=10, pady=10)
delete_button.grid(row=0, column=5, padx=10, pady=10)
coordinate_list.grid(row=2, column=0, columnspan=5, padx=10, pady=10)

# Start listener thread and main loop
listener_thread = threading.Thread(target=listen, daemon=True)
listener_thread.start()
root.after(0, main_loop)

if __name__ == "__main__":
    try:
        root.mainloop()
    except Exception as e:
        print(f"Error: {e}")
    finally:
        keyboard.unhook_all_hotkeys()
        root.destroy()
