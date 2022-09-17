BUILD_DIR = ./build
TARGET = $(BUILD_DIR)/wcat

$(TARGET): wcat.c $(BUILD_DIR)
	c99 -Wall wcat.c -O -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
