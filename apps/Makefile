INCLUDE = 	-I ../include \
		-I ../third_party/ \
		-I ../third_party/glm/

LIBS 	=	-L /usr/lib/x86_64-linux-gnu \
		-l vulkan \
		-l glfw \
		-l dl \
		-l pthread \
		-l X11 \
		-l Xxf86vm \
		-l Xrandr \
		-l Xi \
		-L ../lib/shared/ \
		-l rndrboi

all: bin_lib
	g++ simple_test.cpp ${LIBS} ${INCLUDE} -o ./bin/simple_test

bin_lib:
	@mkdir -p bin

clean:
	@rm -rf bin
