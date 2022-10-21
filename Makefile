MAIN_DIR = Assignment3
BIN_DIR = ${MAIN_DIR}/Project/bin
SRC_DIR = ${MAIN_DIR}/src
SCONS_LAB = python3 /usr/bin/scons
BINDINGS_FLAGS = -C ${MAIN_DIR}/godot-cpp -j4 bits=64
PLUGIN_FLAGS = -C ${MAIN_DIR}
SERVER_SCONSTRUCT_FLAG = -f SConstruct.server
CLIENT_SCONSTRUCT_FLAG = -f SConstruct.client

clean-bin:
	rm -rf ${BIN_DIR}/win64 ${BIN_DIR}/x11 ${BIN_DIR}/osx

clean-src:
	rm -rf ${SRC_DIR}/*.obj ${SRC_DIR}/*.os

clean: clean-bin clean-src

bindings-lab:
	${SCONS_LAB} ${BINDINGS_FLAGS}

bindings:
	scons ${BINDINGS_FLAGS}
	
plugin-lab: plugin-server-lab plugin-client-lab

plugin-server-lab:
	${SCONS_LAB} ${PLUGIN_FLAGS} ${SERVER_SCONSTRUCT_FLAG} p=x11

plugin-client-lab:
	${SCONS_LAB} ${PLUGIN_FLAGS} ${CLIENT_SCONSTRUCT_FLAG} p=x11

plugin: plugin-server plugin-client

plugin-server:
	scons ${PLUGIN_FLAGS} ${SERVER_SCONSTRUCT_FLAG} p=$(p)

plugin-client:
	scons ${PLUGIN_FLAGS} ${CLIENT_SCONSTRUCT_FLAG} p=$(p)
