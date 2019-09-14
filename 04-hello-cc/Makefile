# make start
# make -s start (slient)

CC = g++

SRC_DIR = src/
ENTRY = app.cc

BUILD_DIR = build/
TARGET= app

start: build
	./build/app

build:
	mkdir -p build && ${CC} -o $(BUILD_DIR)${TARGET} ${SRC_DIR}${ENTRY}

clean:
	rm -rf ${BUILD_DIR}

.PHONY: all build run clean