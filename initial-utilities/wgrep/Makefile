APP_NAME = wgrep
COMP_FLAGS = -Wall -Wextra
BUILD_DIR = ./build
TARGET = $(BUILD_DIR)/$(APP_NAME)

$(TARGET): $(APP_NAME).c $(BUILD_DIR)
	gcc $(COMP_FLAGS) -lm $(APP_NAME).c -O -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
