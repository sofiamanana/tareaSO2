TARGET = main
CFLAGS = -Wall -g
all:
	gcc -c tarea.c $(CFLAGS)
	gcc tarea.o -o $(TARGET) $(CFLAGS)
	make clear

run: all
	./$(TARGET)

clear:
	find . -name '*.o' -type f -delete
