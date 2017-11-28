ARCH := powerpc-e300c3-linux-gnu-
ARCH := 
CC := $(ARCH)gcc
CC := $(ARCH)g++
STRIP:= $(ARCH)strip
SRC:= $(wildcard src/*.c)
SRX:= $(wildcard src/*.cpp)
OBJ:= $(patsubst %.c, %.o, $(SRC))
OBJ+= $(patsubst %.cpp, %.o, $(SRX))
CFLAGS:= -Iinclude -c
LFLAGS:= -shared
TARGET:= test.exe
TARGET+= bacapp.exe
TARGET+= bacnet.exe

target: $(TARGET)
	$(STRIP) $+
	@ls -sh $+

test.exe: $(OBJ) test/test.o
	@$(CC) $+ -o $@
bacapp.exe: $(OBJ) test/bacapp.o
	@$(CC) $+ -o $@
bacnet.exe: $(OBJ) test/bacnet.o
	@$(CC) $+ -o $@
.c.o: $(wildcard include/*.h)
	$(CC) $(CFLAGS) $*.c -o $*.o
.cpp.o: $(wildcard include/*.h)
	$(CC) $(CFLAGS) $*.cpp -o $*.o

clean:
	@rm -rfv $(OBJ) test/*.o *.so *.exe
