project('msp-exp432e401-test', 'c',
  version : '0.1.0',
  meson_version : '>=1.10.0',
  default_options : ['c_std=c23', 'buildtype=minsize']
)

objcopy = find_program('objcopy')
objdump = find_program('objdump')
dslite = find_program('dslite')
python_exe = find_program('python3', 'python', required: true)
ccxml = files('tools/MSP432E401Y.ccxml')

serial_port_option = get_option('serial_port')

app_sources = files(
  'src/startup.c',
  'src/system.c',
  'src/uart.c',
  'src/main.c',
)
app_includes = include_directories('src')

vendor_includes = include_directories('vendor')
machine_linker = files('src/flash.ld')

app_exe = executable(
  'app',
  [app_sources],
  dependencies: [dependency('cmsis'),],
  include_directories: [app_includes, vendor_includes],
  link_args: [
    '-nostdlib',
    '-T' + machine_linker[0].full_path(),
    '-Wl,--print-map-locals,-Map=' + meson.current_build_dir() / 'app.map',
  ],
  link_depends: machine_linker,
  name_suffix: 'elf',
)

app_hex = custom_target(
  'app_hex',
  input: app_exe,
  output: app_exe.name() + '.s32',
  command: [objcopy, '-O', 'srec', '@INPUT@', '@OUTPUT@'],
  build_by_default: true,
)

app_txt = custom_target(
  'app_txt',
  input: app_exe,
  output: app_exe.name() + '.txt',
  command: [objdump, '-d', '-S', '-l', '@INPUT@'],
  capture : true,
  build_by_default: true,
)

if serial_port_option == ''
  error('serial_port option must be specified. Use -Dserial_port=<port>')
endif

run_script = meson.current_source_dir() / 'scripts' / 'run.py'

run_target(
  'deploy-run',
  command: [
    python_exe,
    run_script,
    serial_port_option,
    '--dslite', dslite,
    '--ccxml', ccxml,
    '--firmware', app_exe,
  ]
)

run_target(
  'deploy-run-capture',
  command: [
    python_exe,
    run_script,
    serial_port_option,
    '--dslite', dslite,
    '--ccxml', ccxml,
    '--firmware', app_exe,
    '--log-file', meson.current_build_dir() / 'uart.log',
  ]
)
