#!/usr/bin/env python3
"""
tbone: run a TeenyAT program from Python using CFFI bindings.
"""
from teenyat import *

@tny_bus_read
def bus_read(t, addr, data, delay):
    print(f"LOAD from : {addr}")
    data.u = 67
    return

# There are problems with the union...
# cffi dosnt like to pass in the full tny_word union so we have to treat it like a pointer...
#@ffi.callback("void(teenyat*, tny_uword, tny_word*, uint16_t*)")
#def bus_write(t, addr, data, delay):
#    pass

if __name__ == "__main__":

    t = TeenyAT()

    bin_file = tny_file_open("hello.bin")
    if(bin_file == None):
        print("Failed to open binary file")
        sys.exit(1)

    tny_init_unclocked(t, bin_file, bus_read)
    tny_file_close(bin_file)

    for i in range(77):
        tny_clock(t)

    # it actually works tho lol
    print(f"\n\nFinal cycle count: {t.cycle_cnt}")
    print(f"PC: {t.reg[1].u}")
    print(f"rA: {t.reg[3].u}")

