# find_sources.py
import glob
import os
import sys

build_dir = sys.argv[1]
src_files = [f for f in glob.glob("src/**/*.cc", recursive=True) if not f.startswith(build_dir)]
print("\n".join(src_files))