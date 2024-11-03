# graph.py

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

fig = plt.figure()
ax = plt.axes(xlim=(0, 2), ylim=(-2, 2))
line, = ax.plot([], [], lw=2)

def init():
    line.set_data([], [])

    return line,

def animate(i):
    x = np.linspace(0, 2, 1000)
    y = np.cos(2 * np.pi * (x - 0.01 * i)) # formula
    line.set_data(x, y)

    return line,

anim = animation.FuncAnimation(fig, animate, init_func=init, frames=200, interval=20, blit=True)

plt.show()
