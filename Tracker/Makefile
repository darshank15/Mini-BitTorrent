CC = g++
CFLAGS = -Wall 
DEPS = trackerheader.h 
OBJ = tracker.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

tracker: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 

clean:
	rm -rf *o tracker