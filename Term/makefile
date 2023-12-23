CC = gcc
CFLAGS = -Wall -Wextra -pedantic -pthread -Wunused-parameter
LDFLAGS = -lnsl -lpthread

ALL = chats chatc

chats: chats.o 
	$(CC) -o $@ $^ $(LDFLAGS)

chatc: chatc.o 
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)


clean :
	rm -rf *.o $(ALL)
