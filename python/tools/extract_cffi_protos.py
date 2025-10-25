#!/usr/bin/env python3
# tools/extract_cffi_protos.py
# Usage: python3 tools/extract_cffi_protos.py ../teenyat.h > teenyat_cffi_decls.h
# The whole reason we even have this is because cffi does not yet support things like ifndef and other preprocessor macros
# We also choose to keep some of our #define as without them then our header file does not compile
# This also makes it easier to update our bindings for when we update the teenyAT

import re
import sys

if len(sys.argv) < 2:
    print("Usage: python extract_cffi_protos.py PATH/TO/teenyat.h", file=sys.stderr)
    sys.exit(1)

path = sys.argv[1]
txt = open(path, "r", encoding="utf-8").read()

# Remove comments (/* ... */ and // ...)
txt_no_comments = re.sub(r"/\*.*?\*/", "", txt, flags=re.S)
txt_no_comments = re.sub(r"//.*?$", "", txt_no_comments, flags=re.M)

typedefs = re.findall(r"(typedef\s+struct\s+[^{]*\{[^}]*\}\s*[A-Za-z0-9_]+\s*;)", txt_no_comments, flags=re.S)
# Need to do special pattern matching for these guys unfortunately  (or maybe there is better way)
tny_word_union = re.findall(r"(union\s+tny_word\s*\{.*?\};)", txt_no_comments, flags=re.DOTALL)
teenyat_struct = re.findall(r"(struct\s+teenyat\s*\{.*?\};)", txt_no_comments, flags=re.DOTALL)

# function prototypes - look for "return_type name(args);" and dosent start with typedef
funcs = re.findall(r'^(?!\s*typedef)[A-Za-z0-9_ \t\*]+?\s+[A-Za-z0-9_]+\s*\([^;{]*\)\s*;', txt_no_comments, re.MULTILINE)

file_pointer_boiler_plate = [" typedef struct _IO_FILE FILE;",
                             "FILE *fopen(const char *pathname, const char *mode);",
                             "int fclose(FILE *stream);"]

out = []
out.extend(typedefs)
out.extend(file_pointer_boiler_plate)
out.extend(tny_word_union)
out.extend(teenyat_struct)
out.extend(funcs)
# Deduplicate while preserving order
seen = set()
filtered = []
for s in out:
    s = s.strip()
    if s and s not in seen:
        seen.add(s)
        filtered.append(s)

#print("\n\n".join(filtered))
with open('teenyat_cffi_decls.h', 'w') as f:
    print("\n\n".join(filtered), file=f)

