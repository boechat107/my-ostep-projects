APP_NAME1 = wzip
APP_NAME2 = wunzip
COMP_FLAGS = -Wall -Wextra
BUILD_DIR = ./build
TARGET1 = $(BUILD_DIR)/$(APP_NAME1)
TARGET2 = $(BUILD_DIR)/$(APP_NAME2)

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(APP_NAME1).c $(BUILD_DIR)
	c99 $(COMP_FLAGS) -lm $(APP_NAME1).c -O -o $@

$(TARGET2): $(APP_NAME2).c $(BUILD_DIR)
	c99 $(COMP_FLAGS) -lm $(APP_NAME2).c -O -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
