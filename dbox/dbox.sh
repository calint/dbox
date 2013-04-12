bin=dbox
clang++ -o $bin src/*.cpp \
	glfw/lib/osx/libglfw.a \
	-framework cocoa \
	-framework opengl \
 	-framework iokit \
	-pedantic -pedantic-errors -Wall -Wextra -Werror -Wconversion \
	-std=gnu++11 \
	-O3 \
 	-I/usr/X11/include/ -L/usr/X11/lib/ -lpng \
	-Wfatal-errors &&


ls -la $bin &&

./$bin

