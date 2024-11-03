import tkinter as tk
import pyautogui
import keyboard
import time
import threading
import configparser
import os

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

start_hotkey = config.get('Hotkeys', 'start_hotkey')
add_coordinate_hotkey = config.get('Hotkeys', 'add_coordinate_hotkey')

coordinates = []  # (x, y)
autoclicker_on = False
adding = False

def toggle() -> None:
    global autoclicker_on
    autoclicker_on = not autoclicker_on
    update("Autoclicker is ON. Press \"{}\" to turn it OFF.".format(
        start_hotkey) if autoclicker_on else "Autoclicker is OFF. Press \"{}\" to turn it ON.".format(start_hotkey))

def update(msg) -> None: status_label.config(text=msg)

def start() -> None:
    global autoclicker_on
    autoclicker_on = True
    toggle()

def stop() -> None:
    global autoclicker_on
    autoclicker_on = False
    toggle()

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
    x, y = event.x_root, event.y_root
    coordinates.append((x, y))
    coordinate_list.insert(tk.END, f"({x}, {y})")
    print(f"Added coordinate: ( X: {x}, Y: {y} )")
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
        try:
            if action == 'start':
                if event.keysym.lower() == 'prior':
                    start_hotkey = 'PgUp'
                elif event.keysym.lower() == 'next':
                    start_hotkey = 'Page_Down'
                elif event.keysym.lower() == 'control_l' or event.keysym.lower() == 'control_r':
                    start_hotkey = 'Ctrl'
                elif event.keysym.lower() == 'alt_l' or event.keysym.lower() == 'alt_r':
                    start_hotkey = 'Alt'
                elif event.keysym.lower() == 'shift_l' or event.keysym.lower() == 'shift_r':
                    start_hotkey = 'Shift'
                else:
                    start_hotkey = event.keysym
                config.set('Hotkeys', 'start_hotkey', start_hotkey)
            elif action == 'add':
                if event.keysym.lower() == 'prior':
                    add_coordinate_hotkey = 'PgUp'
                elif event.keysym.lower() == 'next':
                    add_coordinate_hotkey = 'Page_Down'
                elif event.keysym.lower() == 'control_l' or event.keysym.lower() == 'control_r':
                    add_coordinate_hotkey = 'Ctrl'
                elif event.keysym.lower() == 'alt_l' or event.keysym.lower() == 'alt_r':
                    add_coordinate_hotkey = 'Alt'
                elif event.keysym.lower() == 'shift_l' or event.keysym.lower() == 'shift_r':
                    add_coordinate_hotkey = 'Shift'
                else:
                    add_coordinate_hotkey = event.keysym
                config.set('Hotkeys', 'add_coordinate_hotkey', add_coordinate_hotkey)
            with open('config.cfg', 'w') as configfile:
                config.write(configfile)
            update_hotkey_labels()
            keyboard.unhook_all_hotkeys()
            listen()
            hotkey_window.destroy()
        except Exception as e:
            print(f"Error setting hotkey: {e}")

    hotkey_window = tk.Toplevel(root)
    hotkey_window.title("Choose Hotkey")
    hotkey_label = tk.Label(hotkey_window, text=f"Press the key for \"{action}\" action:")
    hotkey_label.pack(pady=20)
    hotkey_window.bind("<KeyPress>", set_hotkey)
    instruction_label = tk.Label(hotkey_window, text="Press any key to set it as the hotkey.")
    instruction_label.pack(pady=10)

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
        try:
            time.sleep(0.1)
        except:
            break
        root.update()

root = tk.Tk()
root.title("Autoclicker")
root.resizable(False, False)
root.wm_iconphoto(False, tk.PhotoImage(file=r"./death.png"))

start_button = tk.Button(root, text="Stop", command=start)
stop_button = tk.Button(root, text="Start", command=stop)
add_button = tk.Button(root, text=f"Add Coordinate (Hotkey: \"{add_coordinate_hotkey.upper()}\")", command=add_coordinate)
hotkey_button = tk.Button(root, text="Choose Start Hotkey", command=lambda: choose_hotkey('start'))
add_hotkey_button = tk.Button(root, text="Choose Add Coordinate Hotkey", command=lambda: choose_hotkey('add'))
status_label = tk.Label(root, text="Autoclicker is OFF. Press \"{}\" to turn it ON.".format(start_hotkey))
delete_button = tk.Button(root, text="Delete Selected", command=delete_coordinates)
coordinate_list = tk.Listbox(root, width=30, selectmode=tk.MULTIPLE)

start_button.grid(row=0, column=0, padx=10, pady=10)
stop_button.grid(row=0, column=1, padx=10, pady=10)
add_button.grid(row=0, column=2, padx=10, pady=10)
hotkey_button.grid(row=0, column=3, padx=10, pady=10)
add_hotkey_button.grid(row=0, column=4, padx=10, pady=10)
status_label.grid(row=1, column=0, columnspan=5, padx=10, pady=10)
delete_button.grid(row=0, column=5, padx=10, pady=10)
coordinate_list.grid(row=2, column=0, columnspan=5, padx=10, pady=10)

listener_thread = threading.Thread(target=listen, daemon=True)
listener_thread.start()
root.after(0, main_loop)

if __name__ == "__main__":
    try:
        root.mainloop()
    except:
        exit()
    finally:
        try:
            keyboard.unhook_all_hotkeys()
            root.destroy()
        except:
            exit()
