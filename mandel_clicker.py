from PIL import Image, ImageDraw, ImageTk
import tkinter as tk
import numpy as np

# Window dimensions
WIDTH = 300
HEIGHT = 200

# Initial plot dimensions
R_START = -2
R_END = 1

I_START = 1
I_END = -1

# Mandelbrot function
def f(c, z):
    n = 0
    while n < CURR_ITER and abs(z) <= 2:
        z = z*z + c
        n += 1
    return n

# Window class for the fractal
class App(tk.Frame):

    #Constructor
    def __init__(self, master=None):
        super().__init__(master)
        self.im = Image.new("RGB", (WIDTH, HEIGHT), (0, 0, 0))
        self.iter = 5
        self.create_complexes()
        self.iterations = np.zeros((WIDTH * HEIGHT))
        self.z = np.zeros((WIDTH * HEIGHT), np.complex64)
        self.pack()
        self.create_widgets()

    # Instantiate the complex array
    def create_complexes(self):
        self.complexes = np.empty((WIDTH * HEIGHT), np.complex64)
        for y in range(HEIGHT):
            for x in range(WIDTH):
                c = complex(R_START + (x/WIDTH) * (R_END - R_START),
                            I_START + (y/HEIGHT) * (I_END - I_START))
                self.complexes[y * WIDTH + x] = c

    # Widget creation handler
    def create_widgets(self):
        self.canvas = tk.Canvas(self, height=HEIGHT, width=WIDTH)
        self.canvas.bind("<Button-1>", self.on_space)
        self.canvas.bind("<Button-4>", self.on_scroll)
        self.canvas.bind("<Button-5>", self.on_scroll)
        self.draw()
   
    # Calculate values and draw the image
    def draw(self):
        # Create new PIL image
        draw = ImageDraw.Draw(self.im)
        for y in range(HEIGHT):
            for x in range(WIDTH):
                i = y * WIDTH + x
                (iters, z) = self.f(self.iterations[i],
                               self.complexes[i],
                               self.z[i])
                self.iterations[i] = iters
                self.z[i] = z
                color = 255 - int(iters * 255 / self.iter)
                draw.point([x, y], (color, color, color))

        self.photo = ImageTk.PhotoImage(self.im)
        self.canvas.create_image(0, 0, image=self.photo, anchor='nw')
        self.canvas.pack()

    # Event handler to permit zooming by scrolling
    def on_scroll(self, event):
        zoom = 0
        if (event.num == 4 or event.delta == 120):
            zoom += 1
        if (event.num == 5 or event.delta == -120):
            zoom -= 1
        print(str(event.num))


    # Increase CURR_ITER and redraw when space is hit
    def on_space(self, event):
        self.iter += 5
        print("Space hit, iters at: " + str(self.iter))
        self.draw()

     # Mandelbrot function
    def f(self, iters, c, z):
        while iters < self.iter and abs(z) <= 2:
            z = z*z + c
            iters += 1
        return (iters, z)

root = tk.Tk()
root.attributes("-type", "dialog")
app = App(master=root)
app.mainloop()
