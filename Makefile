CC=clang
CFLAGS=
SOURCE= $(wildcard src/*.c)
INCLUDE=-I./include
OUT=-o
EXECUTABLE=tbot

all:
	$(CC) $(CFLAGS) $(INCLUDE) $(OUT) $(EXECUTABLE) $(SOURCE)

clean:
	rm -f $(EXECUTABLE)
