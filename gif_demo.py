import tkinter as tk
from PIL import Image, ImageTk, ImageSequence

class GIFPlayer(tk.Tk):
    def __init__(self, gif_path):
        super().__init__()
        self.title("GIF Player")
        self.gif_path = gif_path
        self.label = tk.Label(self)
        self.label.pack()

        # Load the GIF
        self.gif = Image.open(self.gif_path)
        self.frames = [ImageTk.PhotoImage(frame.copy().convert("RGBA")) for frame in ImageSequence.Iterator(self.gif)]
        self.frame_index = 0

        # Start animation
        self.animate()

    def animate(self):
        frame = self.frames[self.frame_index]
        self.label.configure(image=frame)
        self.frame_index = (self.frame_index + 1) % len(self.frames)
        self.after(100, self.animate)  # Adjust delay as needed

if __name__ == "__main__":
    gif_path = r"C:\Users\noahf\OneDrive\Desktop\TeenyAT\docs\img\leroy.gif"  # Change to your GIF file path
    app = GIFPlayer(gif_path)
    app.mainloop()
