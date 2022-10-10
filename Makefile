MAIN_DIR = Assignment3
BIN_DIR = ${MAIN_DIR}/Project/bin
SRC_DIR = ${MAIN_DIR}/src
SCONS_LAB = python3 /usr/bin/scons
BINDINGS_FLAGS = -C ${MAIN_DIR}/godot-cpp -j4 bits=64
PLUGIN_FLAGS = -C ${MAIN_DIR}

clean-bin:
	rm -rf ${BIN_DIR}/win64 ${BIN_DIR}/x11 ${BIN_DIR}/osx

clean-src:
	rm -rf ${SRC_DIR}/*.obj ${SRC_DIR}/*.os

clean: clean-bin clean-src

bindings-lab:
	${SCONS_LAB} ${BINDINGS_FLAGS}

plugin-lab:
	${SCONS_LAB} ${PLUGIN_FLAGS} p=x11

bindings:
	scons ${BINDINGS_FLAGS}

plugin:
	scons ${PLUGIN_FLAGS} p=$(p)
