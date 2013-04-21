CC=clang
CFLAGS=
SOURCE= $(wildcard src/*.c)
INCLUDE=-I./include
OUT=-o
EXECUTABLE=tbot

all:
	$(CC) $(CFLAGS) $(INCLUDE) $(OUT) $(EXECUTABLE) $(SOURCE)

install:
	cp $(EXECUTABLE) /usr/bin/

clean:
	rm -f $(EXECUTABLE)
