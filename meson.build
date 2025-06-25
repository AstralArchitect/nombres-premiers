project('nombres-premiers', 'cpp', version : '1.0.0')

#add_global_arguments('-DBENCH', language: 'cpp')

# Define sources
main_sources = ['src/search.cc', 'src/main.cc']
bench_sources = ['src/bench.cc', 'src/search.cc']

# Define the executable
executable('main', main_sources, include_directories: 'Include/')
executable('bench', bench_sources, include_directories: 'Include/')