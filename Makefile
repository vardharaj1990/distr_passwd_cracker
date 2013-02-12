CC = gcc

TARGET = client server

CFLAGS += -g -lpthread -I/opt/local/include
LDFLAGS += -lpthread -lprotobuf-c -L/opt/local/lib
all:	$(TARGET)

$(TARGET): 	lsp.o
    
client: lsp.o
	$(CC) $(CFLAGS) lspmessage.pb-c.o lsp.o client.c -o $@ $(LDFLAGS)
       
server: lsp.o
	$(CC) $(CFLAGS) lspmessage.pb-c.o lsp.o server.c -o $@ $(LDFLAGS)

lsp.o: lspmessage.pb-c.o

%.o:	%.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o 
	rm -f $(TARGET)

