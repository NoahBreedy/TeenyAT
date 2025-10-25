#!/usr/bin/env python3
"""
tbone: run a TeenyAT program from Python using CFFI bindings.
"""

from _teenyat_cffi import ffi, lib
import sys

@ffi.callback("void(teenyat*, tny_uword, tny_word*, uint16_t*)")
def bus_read(t, addr, data, delay):
    print(f"LOAD from : {addr}")
    return

# There are problems with the union...
# cffi dosnt like to pass in the full tny_word union so we have to treat it like a pointer...
@ffi.callback("void(teenyat*, tny_uword, tny_word*, uint16_t*)")
def bus_write(t, addr, data, delay):
    pass

if __name__ == "__main__":

    t = ffi.new("teenyat*")

    # This is the nonsense we need to do to open a file pointer
    libc = ffi.dlopen(None)         # note we can only do this by including the the FILE* and fopens functions into our program
    # This could probably be mitigated if we had the c program open the file for ous instead of doing it system side...
    bin_file = libc.fopen(b"hello.bin", b"rb")
    if(bin_file == ffi.NULL):
        print("Failed to open binary file")
        sys.exit(1)

    # cant get bus write callbacks to build
    lib.tny_init_unclocked(t, bin_file, bus_read, ffi.NULL)

    libc.fclose(bin_file)

    for i in range(77):
        lib.tny_clock(t)

    # it actually works tho lol
    print(f"Final cycle count: {t.cycle_cnt}")

