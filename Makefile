SRC:=$(wildcard *.c)
BIN:=AddressBook
OBJ:=$(SRC:.c=.o)
CFLAGS:=-Wall -Wextra
CPPFLAGS:=-I../lib
LDFLAGS:=-L../lib
LIBS:=-llinked_list
BIN_DIR:=/usr/local/bin

.PHONY: all clean install uninstall

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $^ $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $^

install: $(BIN)
	install -D -m 644 $(BIN) $(BIN_DIR)/$(AddressBook)

uninstall:
	$(RM) $(BIN) $(BIN_DIR)/$(BIN)

clean:
	$(RM) $(OBJ) $(BIN)
