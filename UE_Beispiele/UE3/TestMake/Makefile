CC      = clang
OBJECTS = main.o test1.o test2.o
LIBS    = -lm
CFLAGS  = -c -Wall

test: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LIBS)

#es werden alle Objektdateien mit Extension. o aus C-Dateien 
#generiert und in Abhaengigkeit gesetzt
#$< -> ist der Name der ersten Abhaengigkeit der aktuellen Regel
%.o: %.c
	$(CC) $(CFLAGS) $<
	
#Ziele unter .PHONY sind kuenstliche Ziele und werden immer ausgefuehrt
#egal ob "clean" als Datei existiert oder nicht
#Diese Ziele erzeigen keine Ziel-Dateien
.PHONY: clean run

clean:
	rm *.o test

run: test
	./test


