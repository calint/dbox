#clear
bin=dbox
cmd0=clang++
#cmd0=g++
cmd="$cmd0 -o $bin src/*.cpp \
	glfw/lib/osx/libglfw.a \
	-framework cocoa \
	-framework opengl \
 	-framework iokit \
	-pedantic -pedantic-errors -Wall -Wextra -Werror -Wconversion \
	-O3 \
 	-I/usr/X11/include/ -L/usr/X11/lib/ -lpng \
 	-std=c++11 \
	-Wfatal-errors"&&
echo $cmd&&
clang -v&&
#cat src/main.cpp |grep 'class\|namespace'&&
$cmd&&
ls -la $bin&&
cat src/*.cpp|gzip|wc&&
#cat $bin|gzip|wc&&
./$bin



#	-std=gnu++11 \
# 	-stdlib=libc++ \
