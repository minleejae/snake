CC = g++
CCFLAGS = -g

Main: Main.o Snake.o Snake_map.o
	$(CC) $(CCFLAGS) -o Main Main.o Snake.o Snake_map.o -lncursesw

%.o : %.cpp %.h
	$(CC) $(CCFLAGS) -c $<

%.o : %.cpp
	$(CC) $(CCFLAGS) -c $<

% : %.cpp
	$(CC) $(CCFLAGS) -o $@ $<
