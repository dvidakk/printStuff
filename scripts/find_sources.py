import glob
import os
import sys

# The first argument is the build directory
build_dir = sys.argv[1]

# The rest of the arguments are the files to exclude
exclude_files = sys.argv[2:]

# Find all .cc files in the src directory, excluding the build directory and platform-specific files
src_files = [
    f for f in glob.glob("src/**/*.cc", recursive=True)
    if not f.startswith(build_dir) and f not in exclude_files
]

print("\n".join(src_files))
