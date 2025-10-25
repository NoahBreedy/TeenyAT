# python/build_teenyat_ffi.py
from cffi import FFI
import os
import sys

p = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, p)
lib_dir = p + '/build/out/lib'

ffi = FFI()

# Read the cleaned cdefs you produced with the helper.
# Make sure teenyat_cffi_decls.h only contains declarations (no #includes, no macros)
cdef_path = os.path.join(os.path.dirname(p + "/python/"), "teenyat_cffi_decls.h")
if not os.path.exists(cdef_path):
    raise SystemExit("Run: \"python3 tools/extract_cffi_protos.py <path to teenyat.h>\" to create teenyat_cffi_decls.h first")

cdef_text = open(cdef_path, "r", encoding="utf-8").read()
ffi.cdef(cdef_text)

# Set source: include the real header so the compiled extension is compiled with exact declarations.
# This will compile a small shim C file that includes teenyat.h to guarantee ABI match.
source = r'''
#include "teenyat.h"
'''

# Configure module name and extra compile/link args. Name the extension _teenyat_cffi
ffi.set_source(
    "_teenyat_cffi",   # module name
    source,
    include_dirs=[p],  # repo root so teenyat.h is found
    libraries = ['teenyat_d'], library_dirs = [lib_dir],
)

if __name__ == "__main__":
    ffi.compile(verbose=True)

