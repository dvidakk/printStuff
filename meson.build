project('printStuff', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'b_staticpic=true'])

build_dir = meson.current_build_dir()

python = find_program('python3')

# Check if the script exists
script = 'scripts/find_sources.py'
if files(script).length() == 0
  error('Script not found: ' + script)
endif

exclude_files = []
if host_machine.system() == 'windows'
  exclude_files += ['src\\CUPSPrinter.cc']
elif host_machine.system() == 'linux' or host_machine.system() == 'darwin'
  exclude_files += ['src/WinPrinter.cc']
endif

globber = run_command(python, [script, build_dir] + exclude_files, check: true)
if globber.returncode() != 0
  error('Failed to find source files')
endif
src = globber.stdout().strip().split('\n')

# Add CUPS dependency only for Linux and macOS
cups_dep = []
if host_machine.system() == 'linux' or host_machine.system() == 'darwin'
  cups_dep = dependency('cups', required: true, static: true)
endif

inc_dir = include_directories('include')

exe = executable('printStuff', src,
  dependencies : cups_dep,
  include_directories : inc_dir,
  install : true)

test('basic', exe)