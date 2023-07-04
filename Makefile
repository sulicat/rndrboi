INCLUDE = 	-I ./include \
		-I ./third_party/ \
		-I ./third_party/glm/

LIBS 	=	-l vulkan \
		-l glfw \
		-l dl \
		-l pthread \
		-l X11 \
		-l Xxf86vm \
		-l Xrandr \
		-l Xi

FLAGS = -std=c++17 -O2

all:
	g++ src/main.cpp -o rndrboi $(INCLUDE) $(LIBS) $(FLAGS)
