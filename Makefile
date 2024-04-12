all:
	meson setup bin/
	meson compile -C bin/
run:
	meson setup bin/
	meson compile -C bin/
	bin/main