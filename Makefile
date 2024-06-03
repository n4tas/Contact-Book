CC:=gcc
LIBS:=-lllist -lncurses
SRC:=book.c
BIN:=ContactBook
INSTALL_PATH:=/usr/local/bin
all: $(BIN)
$(BIN): 
	$(CC) $(SRC) $(LIBS) -o $(BIN)
.PHONY: install
install:
	install -D -m 644 book.h $(INSTALL_PATH)/$(BIN)
uninstall:
	rm -f $(INSTALL_PATH)/$(BIN)
clean:
	rm -f $(OBJ) $(BIN)
