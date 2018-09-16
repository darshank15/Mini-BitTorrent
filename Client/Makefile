CC = g++
CFLAGS = -Wall 
DEPS = clientheader.h 
OBJ = mtorrentcreator.o client.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< -lssl -lcrypto

client: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lssl -lcrypto

clean:
	rm -rf *o client