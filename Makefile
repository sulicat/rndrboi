
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

SRC_DIR		= src
SRC		= $(wildcard ${SRC_DIR}/*.cpp)

FLAGS		= -std=c++17 -O2 -fPIC
MAKE_CMD	= make -j -C
MKDIR_CMD	= mkdir -p
CXX		= g++
LIB_NAME	= rndrboi
LIB_PATH	= ./lib
OBJ_DIR		= ./obj

OBJ = $(patsubst ${SRC_DIR}/%.cpp, ${OBJ_DIR}/%.o, ${SRC})

.PHONY: all apps rndrboi_shared

all: rndrboi_shared apps

lib_dir:
	@${MKDIR_CMD} ${LIB_PATH}/shared
	@${MKDIR_CMD} ${LIB_PATH}/static
	@${MKDIR_CMD} obj/

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	${CXX} ${FLAGS} ${INCLUDE} -c -o $@ $< $(CFLAGS)

rndrboi_shared: lib_dir ${OBJ}
	@echo "-------------------- RNDRBOI SHARED"
	${CXX} ${FLAGS} --shared ${OBJ} -o ${LIB_PATH}/shared/lib${LIB_NAME}.so

apps:
	@echo "-------------------- APPS"
	@${MAKE_CMD} apps/

clean:
	@rm -rf ${LIB_PATH}
	@rm -rf obj/
	@${MAKE_CMD} apps/ clean


