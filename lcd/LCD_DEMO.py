import subprocess
import sys
import platform
import glob
import time

def run_with_timeout(command, timeout_sec=1):
    """Runs a command with a timeout, compatible with Windows."""
    try:
        subprocess.run(command, timeout=timeout_sec, check=True)
    except subprocess.TimeoutExpired:
        print(f"Command {command} timed out after {timeout_sec} seconds.")
    except subprocess.CalledProcessError as e:
        print(f"Command {command} failed with exit code {e.returncode}.")

def main():
    system = platform.system()

    # for asm_file in glob.glob("*.asm",root_dir="C:/Users/noahf/OneDrive/Desktop/TeenyAT/lcd/asm"):
    #         print(f"Processing: {asm_file}")
    #         asm_file = "../asm/" + asm_file
    #         command = ["../../tnasm/tnasm.exe", asm_file]
    #         run_with_timeout(command)
    #         time.sleep(0.1)
    while True:
        i = 0
        for bin_file in glob.glob("*.bin",root_dir="C:/Users/noahf/OneDrive/Desktop/TeenyAT/lcd/bin"):
            print(f"Processing: {bin_file}")
            bin_file = "./bin/" + bin_file
            command = ["./lcd", bin_file]
            timer = 20
            if(i == 1 or i == 9):
                timer = 5
            run_with_timeout(command, timer)
            time.sleep(0.1)
            i += 1

if __name__ == "__main__":
    main()

