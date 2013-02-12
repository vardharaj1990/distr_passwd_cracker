CC = g++

TARGET = client server

CFLAGS += -g -lpthread -I/usr/local/include
LDFLAGS += -lpthread -lprotobuf-c -L/usr/local/lib
all:	$(TARGET)

$(TARGET): 	lsp.o
    
client: lsp.o
	$(CC) $(CFLAGS) lspmessage.pb-c.o lsp.o client.cc -o $@ $(LDFLAGS)
       
server: lsp.o
	$(CC) $(CFLAGS) lspmessage.pb-c.o lsp.o server.cc -o $@ $(LDFLAGS)

lsp.o: lspmessage.pb-c.o

%.o:	%.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o 
	rm -f $(TARGET)

