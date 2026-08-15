init:
	mkdir -p build/

compile:
	gcc ./src/main.c -o build/imgv -I/usr/include/SDL2 -D_GNU_SOURCE=1 -D_REENTRANT -L/usr/lib -lSDL2 -lSDL2_image

run: init compile
	mv ./build/imgv /usr/bin/
