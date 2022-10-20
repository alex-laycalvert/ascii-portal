UNAME := $(shell uname)
BUILD_DIR=bin

ifeq ($(UNAME), Darwin)
	CFLAGS=-I/usr/local/opt/ncurses/include -L/usr/local/opt/ncurses/lib -lncurses -lmenu -std=c++20
endif

ifeq ($(UNAME), Linux)
	CFLAGS=-lncurses -lmenu -std=c++20
endif

portal:
	mkdir -p ./bin
	$(CXX) ./src/*.cpp $(CFLAGS) -Wall -o ./$(BUILD_DIR)/portal
