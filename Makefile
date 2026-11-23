BUILD_DIR=bin
SRC_DIR=src
INCLUDE_DIR=include
TEST_DIR=tests
TARGET_NAME=follow
TARGET_TEST_NAME=follow_test
TARGET=$(BUILD_DIR)/$(TARGET_NAME)
TARGET_TEST=$(BUILD_DIR)/$(TARGET_TEST_NAME)
CC ?= gcc
PKG_CONFIG ?= pkg-config
MINGW_PREFIX ?= x86_64-w64-mingw32
MINGW_SYSROOT ?= /usr/$(MINGW_PREFIX)/sys-root/mingw
WINDOWS_BUILD_DIR=$(BUILD_DIR)/windows
WINDOWS_TARGET=$(WINDOWS_BUILD_DIR)/$(TARGET_NAME).exe
WINDOWS_DIST=$(BUILD_DIR)/windows-dist
SRC=$(call wildcard, $(SRC_DIR)/*.c)
OBJ=$(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(filter-out $(SRC_DIR)/main.o, $(SRC:.c=.o)))
OBJ_MAIN=$(OBJ) $(BUILD_DIR)/main.o $(BUILD_DIR)/ui_resources.o
UI_RESOURCE_SRC=$(BUILD_DIR)/ui_resources.c
SRC_TEST=$(call wildcard, $(TEST_DIR)/*.c)
OBJ_TEST=$(patsubst $(TEST_DIR)/%, $(BUILD_DIR)/%, $(SRC_TEST:.c=.o))
CFLAGS = -DUNITY_INCLUDE_DOUBLE -DUNITY_DOUBLE_PRECISION=1e-12f -Wall -Wextra -Wpedantic -Wconversion -Wshadow -g -O1  -Iexternal/unity/src -I$(INCLUDE_DIR) $(shell $(PKG_CONFIG) --cflags gtk+-3.0)
LFLAGS = -lm -fno-omit-frame-pointer $(shell $(PKG_CONFIG) --libs gtk+-3.0)
LFLAGS_TEST = $(LFLAGS) -lasan -fsanitize=address,undefined

.PHONY: run setup windows windows-dist test clean

run: $(TARGET)
	$(TARGET)

windows:
	$(MAKE) CC=$(MINGW_PREFIX)-gcc PKG_CONFIG=$(MINGW_PREFIX)-pkg-config \
		BUILD_DIR=$(WINDOWS_BUILD_DIR) TARGET=$(WINDOWS_TARGET) $(WINDOWS_TARGET)

windows-dist: windows
	mkdir -p $(WINDOWS_DIST)
	cp $(WINDOWS_TARGET) $(WINDOWS_DIST)/
	cp $(MINGW_SYSROOT)/bin/*.dll $(WINDOWS_DIST)/

setup:
	git submodule update --init --recursive

$(TARGET): $(OBJ_MAIN)
	$(CC) $(OBJ_MAIN) -o $(TARGET) $(LFLAGS)

$(UI_RESOURCE_SRC): ui.glade ui.gresource.xml mk_build_dir
	glib-compile-resources --generate-source --target=$@ --sourcedir=. ui.gresource.xml

$(BUILD_DIR)/ui_resources.o: $(UI_RESOURCE_SRC)
	$(CC) $(CFLAGS) -Wno-overlength-strings -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/%.h mk_build_dir
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c mk_build_dir
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/unity.o: external/unity/src/unity.c mk_build_dir
	$(CC) $(CFLAGS) -c $< -o $@

mk_build_dir:
	mkdir -p $(BUILD_DIR)

$(TARGET_TEST): $(OBJ) $(OBJ_TEST) $(BUILD_DIR)/unity.o
	$(CC) $(OBJ_TEST) $(OBJ) $(BUILD_DIR)/unity.o -o $(TARGET_TEST) $(LFLAGS_TEST)

test: $(TARGET_TEST)
	./$(TARGET_TEST)

clean:
	rm -rf $(BUILD_DIR)
