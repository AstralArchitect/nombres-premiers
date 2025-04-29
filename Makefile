# Target to compile the project
all:
	meson compile -C builddir/

# Target to run the compiled binary
run: all
	./builddir/main

test: all
	./builddir/main 100
	timeout /t 3 > nul
	./builddir/main 1000
	echo "104729"
	timeout /t 3 > nul
	./builddir/main 10000


# Target to set up the build directory
setupClang:
	meson setup builddir --native-file=native-clang.txt
setup:
	meson setup builddir
ifeq ($(OS),Windows_NT)
	@if exist builddir/build.ninja (powershell -NoLogo -NoProfile -Command "(Get-Content builddir\build.ninja) -replace ' ""msvcrt.lib""""', '' | Set-Content builddir\build.ninja")
endif

# Target to clean the build directory
clean:
ifeq ($(OS),Windows_NT)
	@if exist builddir (rmdir builddir /S /Q)
else
	@if [ -d builddir ]; then rm -rf builddir; fi
endif