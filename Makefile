all:
	mkdir build/ -p
	nasm -f elf64 src/assembly/finder.asm -o build/finder.o
	g++ src/c++/main.cc src/c++/find.cc build/finder.o -o build/main -no-pie
run: all
	build/main
bench:
	mkdir build/ -p
	nasm -f elf64 src/assembly/finder.asm -o build/finder.o
	g++ src/c++/main.cc src/c++/find.cc build/finder.o -o build/main -no-pie -DBENCH
	build/main