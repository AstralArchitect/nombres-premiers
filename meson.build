project('nombres-premiers', 'cpp', 'cuda', version : '3.0.0')

# Define sources
sources = ['src/main.cc', 'src/search.cu']

# Define the executable
executable('main', sources)
bench = executable('bench', sources, cpp_args: '-DBENCH')