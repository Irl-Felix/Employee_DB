TARGET = bin/dbview
TARGET_ASAN = bin/dbview_asan

SRC = $(wildcard source/*.c)
OBJ = $(patsubst source/%.c, obj/%.o, $(SRC))
OBJ_ASAN = $(patsubst source/%.c, obj/%_asan.o, $(SRC))

CFLAGS = -Wall -Wextra -g -Iinclude
ASAN_FLAGS = -fsanitize=address -fno-omit-frame-pointer

run: clean default
	./$(TARGET) -f mynewdb.db -n
	./$(TARGET) -f mynewdb.db -a "Maxwell,Front Street,10000"
	./$(TARGET) -f mynewdb.db -a "Emily,Canada Estate,7009"

default: $(TARGET)

# 🔹 Normal Build
$(TARGET): $(OBJ) | bin
	gcc -o $@ $^

obj/%.o: source/%.c | obj
	gcc $(CFLAGS) -c $< -o $@

# 🔹 ASan Build
asan: $(TARGET_ASAN)

$(TARGET_ASAN): $(OBJ_ASAN) | bin
	gcc $(ASAN_FLAGS) -o $@ $^

obj/%_asan.o: source/%.c | obj
	gcc $(CFLAGS) $(ASAN_FLAGS) -c $< -o $@

clean:
	rm -f obj/*.o
	rm -f bin/*
	rm -f *.db

# Directory creation rules
bin:
	mkdir -p bin

obj:
	mkdir -p obj