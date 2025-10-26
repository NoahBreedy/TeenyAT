# A wrapper for our teenyAT functions so its cleaner to write and use the teenyAT in python
from _teenyat_cffi import ffi, lib
import sys

# Returns an instance of the teenyAT
def TeenyAT():
    return ffi.new("teenyat*")

def tny_file_open(file_name):
    # open the c standard library
    libc = ffi.dlopen(None)
    file_name = f"{file_name}".encode() # doing this to convert python string to byte string
    bin_file = libc.fopen(file_name, b"rb")
    if(bin_file == ffi.NULL):
        return None
    return  bin_file

def tny_file_close(file_ptr):
    # open the c standard library
    libc = ffi.dlopen(None)
    if(file_ptr != ffi.NULL):
        libc.fclose(file_ptr)
    return

# This is here to prevent our address from getting grabage collected
# unfortunately this gives us a lame __Pycache__
_callback_registry = []

def tny_bus_read(bus_read):
    c_callback = ffi.callback(
        "void(teenyat*, tny_uword, tny_word*, uint16_t*)",
        bus_read
    )

    _callback_registry.append(c_callback)

    return c_callback

def tny_init_unclocked(t, file_ptr, bus_read=ffi.NULL, bus_write=ffi.NULL):
    lib.tny_init_unclocked(t, file_ptr, bus_read, bus_write)
    return

def tny_clock(t):
    lib.tny_clock(t)
    return
