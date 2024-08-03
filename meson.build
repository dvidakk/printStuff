project('printStuff', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3'])

# Define the build directory
build_dir = meson.current_build_dir()

# Find the Python interpreter
python = find_program('python3')

# Check if the script exists
script = 'scripts/find_sources.py'
if files(script).length() == 0
  error('Script not found: ' + script)
endif

# Run the Python script to find source files
globber = run_command(python, script, build_dir, check: true)
if globber.returncode() != 0
  error('Failed to find source files')
endif

# Parse the output of the Python script
src = globber.stdout().strip().split('\n')

# Define the executable target
exe = executable('printStuffSrv', src,
  link_args : '-rdynamic',
  install : true)

# Define a basic test
test('basic', exe)