all:
	meson compile -C bin/
run:
	meson compile -C bin/
	bin/main
configure:
	meson setup bin/ --reconfigure