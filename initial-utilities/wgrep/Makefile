APP_NAME = wgrep
COMP_FLAGS = -Wall -Wextra
BUILD_DIR = ./build
TARGET = $(BUILD_DIR)/$(APP_NAME)

$(TARGET): $(APP_NAME).c $(BUILD_DIR)
	c99 $(COMP_FLAGS) -lm $(APP_NAME).c -O -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

valgrind: $(TARGET)
	-valgrind --leak-check=full \
          --show-leak-kinds=all \
          --track-origins=yes \
          --verbose \
          --log-file=valgrind-out.txt \
          $(TARGET) match wgrep.c README.md
	@grep -A2 SUMMARY valgrind-out.txt
