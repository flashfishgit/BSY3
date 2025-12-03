CC      = clang
OBJECTS = main.o
LIBS    = -lm
CFLAGS  = -Wall -g

test: $(OBJECTS)
	$(CC) -g -o $@ $(OBJECTS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean run

clean:
	rm -f *.o test

run: test
	clear
	./test Hallo Maximus
