LIBS := $(shell pkg-config --libs glib-2.0 gobject-2.0 libsoup-2.2)
CFLAGS := $(shell pkg-config --cflags glib-2.0 gobject-2.0 libsoup-2.2)

CC := gcc -c
LINK := gcc

SERVER_OBJECTS = server.o
CLIENT_OBJECTS = client.o

all: server client

server: $(SERVER_OBJECTS)
	$(LINK) $(LIBS) -o $@ $^

client: $(CLIENT_OBJECTS)
	$(LINK) $(LIBS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $<
