import random
from tkinter import *
from tkinter import ttk
from tkinter.messagebox import askyesno, showerror
import string as s

alph = s.ascii_uppercase + s.ascii_lowercase

def q(l:int,v:int) -> str:
    if v == 0: return ''.join(random.choice(alph) for _ in range(l))
    elif v == 1: return ''.join(random.choice(s.ascii_lowercase) for _ in range(l))
    elif v == 2: return ''.join(random.choice(s.ascii_uppercase) for _ in range(l))
    elif v == 3: return ''.join(random.choice(s.punctuation) for _ in range(l))
    elif v == 4: return ''.join(random.choice(s.punctuation+alph) for _ in range(l))
    else: return 'UNKNOWN TYPE'

def delete() -> None:
    selection = listb.curselection()
    if not selection or listb.get(selection[0]) == '':
        showerror(q(random.randint(3,9),1), q(random.randint(3,9),1))
        return

    sel = selection[0]
    selected_item = listb.get(sel)
    result = askyesno(title=q(random.randint(3,9),2), message=f'{q(random.randint(3,9),3)} "{selected_item}"?')

    if result:
        listb.delete(sel)
        print(f'{q(random.randint(3,9),2)} "{selected_item}" {q(random.randint(3,9),1)}!')
    else:
        pass

def add() -> None:
    new_e = entry.get()
    listb.insert(0, str(q(random.randint(2,30),4)))
    print(f'{q(random.randint(3,10),1)} "{new_e}" {q(random.randint(4,7),1)}!')


root = Tk()
root.title(q(random.randint(5,7),2))
root.geometry("300x250")
root.columnconfigure(index=0, weight=4)
root.columnconfigure(index=1, weight=1)
root.rowconfigure(index=0, weight=1)
root.rowconfigure(index=1, weight=3)
root.rowconfigure(index=2, weight=1)

entry = ttk.Entry()
entry.grid(column=0, row=0, padx=6, pady=6, sticky=EW)
ttk.Button(text=q(random.randint(3,5),2)+q(1,3)+q(1,0), command=add).grid(column=1, row=0, padx=6, pady=6)

listb = Listbox()
listb.grid(row=1, column=0, columnspan=2, sticky=EW, padx=5, pady=5)

listb.insert(END, q(random.randint(6,8),2))
listb.insert(END, q(random.randint(5,9),0))

ttk.Button(text=q(random.randint(3,5),3)+q(random.randint(3,5),1)+q(random.randint(3,5),3), command=delete).grid(row=2, column=1, padx=5, pady=5)

root.mainloop()
