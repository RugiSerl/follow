BUILD_DIR=bin
SRC_DIR=src
INCLUDE_DIR=include
TEST_DIR=tests
TARGET_NAME=follow
TARGET_TEST_NAME=follow_test
TARGET=$(BUILD_DIR)/$(TARGET_NAME)
TARGET_TEST=$(BUILD_DIR)/$(TARGET_TEST_NAME)
SRC=$(call wildcard, $(SRC_DIR)/*.c)
OBJ=$(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(filter-out $(SRC_DIR)/main.o, $(SRC:.c=.o)))
OBJ_MAIN=$(OBJ) $(BUILD_DIR)/main.o
SRC_TEST=$(call wildcard, $(TEST_DIR)/*.c)
OBJ_TEST=$(patsubst $(TEST_DIR)/%, $(BUILD_DIR)/%, $(SRC_TEST:.c=.o))
CFLAGS = -Wall -Iexternal/unity/src -I$(INCLUDE_DIR)
LFLAGS = -lncurses

run: $(TARGET)
	$(TARGET)

setup:
	git submodule update --init --recursive

$(TARGET): $(OBJ_MAIN)
	gcc $(OBJ_MAIN) -o $(TARGET) $(LFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/%.h mk_build_dir
	gcc $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c mk_build_dir
	gcc $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/unity.o: external/unity/src/unity.c mk_build_dir
	gcc $(CFLAGS) -c $< -o $@

mk_build_dir:
	mkdir -p $(BUILD_DIR)

$(TARGET_TEST): $(OBJ) $(OBJ_TEST) $(BUILD_DIR)/unity.o
	gcc $(OBJ_TEST) $(OBJ) $(BUILD_DIR)/unity.o -o $(TARGET_TEST) $(LFLAGS)

test: $(TARGET_TEST)
	./$(TARGET_TEST) | tee test_output.txt

clean:
	rm -rf $(BUILD_DIR)
