CC = g++
CCFLAGS = -g

Main : Main.o Snake.o Snake_map.o Display.o Item.o Point.o Mission.o keyboardEventhandler.o
	$(CC) $(CCFLAGS) -o Main Main.o Snake.o Snake_map.o Display.o Item.o Point.o Mission.o keyboardEventhandler.o -lncursesw

clean:
	rm -f *.o

%.o : %.cpp %.h
	$(CC) $(CCFLAGS) -c $<

%.o : %.cpp
	$(CC) $(CCFLAGS) -c $<

% : %.cpp
	$(CC) $(CCFLAGS) -o $@ $<
