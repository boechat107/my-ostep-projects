CC := c99
CFLAGS := -Wall -Wextra
BUILD_DIR := ./build
MYLIB_OBJ := $(BUILD_DIR)/my_getline.c.o
WISH_BIN := $(BUILD_DIR)/wish
WISH_OBJS := $(MYLIB_OBJ) $(BUILD_DIR)/wish.c.o

$(WISH_BIN): $(WISH_OBJS)
	$(CC) $(WISH_OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

valgrind:
	-valgrind --leak-check=full \
          --show-leak-kinds=all \
          --track-origins=yes \
          --verbose \
          --log-file=valgrind-out.txt \
          $(args)
	@grep -A2 SUMMARY valgrind-out.txt
