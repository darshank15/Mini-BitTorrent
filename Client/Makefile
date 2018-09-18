CC = g++
CFLAGS = -Wall 
DEPS = clientheader.h 
OBJ = share.o get.o remove.o seeder.o mtorrentcreator.o client.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< -lssl -lcrypto -lpthread

client: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lssl -lcrypto -lpthread

clean:
	rm -rf *o client