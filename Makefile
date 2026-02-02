TARGET = bin/dbview
SRC = $(wildcard source/*.c)
OBJ = $(patsubst source/%.c, obj/%.o, $(SRC))

run: clean default
	./$(TARGET) -f mynewdb.db

default: $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*
	rm -f *.db

$(TARGET): $(OBJ)
	gcc -o $@ $?

obj/%.o : source/%.c
	gcc -c $< -o $@ -Iinclude
