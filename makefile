ARCH := powerpc-e300c3-linux-gnu-
ARCH := 
CC := $(ARCH)g++
CC := $(ARCH)gcc
STRIP:= $(ARCH)strip
SRC:= $(wildcard src/*.c)
OBJ:= $(patsubst %.c, %.o, $(SRC))
CFLAGS:= -Iinclude -c
LFLAGS:= -shared
TARGET:= bacnet-core.so

test.exe: $(OBJ) test/test.o
	$(CC) $+ -o $@

target: $(TARGET)
	$(STRIP) $+
	@ls -sh $+

$(TARGET): $(OBJ)
	@$(CC) $(LFLAGS) $+ -o $@

.c.o: $(wildcard include/*.h)
	$(CC) $(CFLAGS) $*.c -o $*.o

clean:
	@rm -rfv $(OBJ) *.so *.exe
