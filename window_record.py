import subprocess
import time
import pygetwindow as gw
import pyautogui
import imageio
from PIL import Image
import os

# --- Settings ---
EXECUTABLE_PATH = r'C:\Users\noahf\OneDrive\Desktop\TeenyAT\lcd\lcd.exe C:\Users\noahf\OneDrive\Desktop\TeenyAT\lcd\bin\example_random.bin'
WINDOW_TITLE_KEYWORD = "TeenyAT LCD"  # Partial or full window title
DURATION = 10                    # Seconds to record
FPS = 5                          # Frames per second
OUTPUT_GIF = 'output.gif'

def launch_and_capture():
    # Launch the executable
    process = subprocess.Popen(EXECUTABLE_PATH)

    # Wait for the window to appear
    print("Waiting for window...")
    window = None
    for _ in range(50):
        windows = gw.getWindowsWithTitle(WINDOW_TITLE_KEYWORD)
        if windows:
            window = windows[0]
            break
        time.sleep(0.2)

    if not window:
        print("Window not found!")
        process.terminate()
        return

    print(f"Found window: {window.title}")
    images = []
    frame_interval = 1 / FPS
    end_time = time.time() + DURATION

    while time.time() < end_time:
        if window.isMinimized:
            window.restore()
        bbox = (window.left, window.top, window.left + window.width, window.top + window.height)
        screenshot = pyautogui.screenshot(region=bbox)
        images.append(screenshot)
        time.sleep(frame_interval)

    print("Recording complete. Creating GIF...")

    # Convert to GIF
    images[0].save(OUTPUT_GIF, save_all=True, append_images=images[1:], duration=int(1000/FPS), loop=0)

    print(f"GIF saved to {OUTPUT_GIF}")

    # Optional: close the app
    process.terminate()

if __name__ == '__main__':
    launch_and_capture()
