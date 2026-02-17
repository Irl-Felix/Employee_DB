TARGET = bin/dbview
SRC = $(wildcard source/*.c)
OBJ = $(patsubst source/%.c, obj/%.o, $(SRC))

run: clean default
	./$(TARGET) -f mynewdb.db -n
	./$(TARGET) -f mynewdb.db -a "Maxwell F., 233 Prince Lane, 4433"

default: $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*
	rm -f *.db

# Ensure bin directory exists before linking
$(TARGET): $(OBJ) | bin
	gcc -o $@ $^

# Ensure obj directory exists before compiling
obj/%.o : source/%.c | obj
	gcc -c $< -o $@ -Iinclude

# Directory creation rules
bin:
	mkdir -p bin

obj:
	mkdir -p obj
