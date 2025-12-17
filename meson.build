project(
  'expression-evaluator',
  'cpp',
  default_options: [
    'cpp_std=c++20',
    'warning_level=3',
  ]
)

add_project_arguments(
  [
    '-Wpedantic',
    '-Wconversion',
    '-Wsign-conversion',
  ],
  language: 'cpp'
)

include_dir = include_directories('include')
subdir('src')

evaluator_executable = executable(
  'expression-evaluator',
  src_sources,
  include_directories: include_dir,
)

run_target('run', command: [evaluator_executable])

subdir('tests')