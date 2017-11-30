ARCH := 
ARCH := powerpc-e300c3-linux-gnu-
CC := $(ARCH)gcc
CC := $(ARCH)g++
AR := $(ARCH)ar
STRIP := $(ARCH)strip
SRC := $(wildcard src/*.c)
SRC += $(wildcard uart/*.c)
SRX := $(wildcard src/*.cpp)
SRX += $(wildcard uart/*.cpp)
OBJ := $(patsubst %.c, %.o, $(SRC))
OBJ += $(patsubst %.cpp, %.o, $(SRX))
CFLAGS := -Iinclude -Isrc -Iuart -c
LFLAGS := -shared
TARGET := test.exe
TARGET += bacapp.exe
TARGET += apdu.exe
TARGET += recvframe.exe
TARGET += sendframe.exe
TARGET += bacnet.exe
#TARGET+= bacnet-core.a
#bacnet-core.a:$(OBJ)
#	@$(AR) -rc $@ $+	

target: $(TARGET)
	@$(STRIP) $+
	@ls -sh $+
recvframe.exe:$(OBJ) test/recvframe.o
	@$(CC) $+ -o $@
sendframe.exe:$(OBJ) test/sendframe.o
	@$(CC) $+ -o $@
bacnet.exe: $(OBJ) test/bacnet.o
	@$(CC) $+ -o $@
test.exe: $(OBJ) test/test.o
	@$(CC) $+ -o $@
bacapp.exe: $(OBJ) test/bacapp.o
	@$(CC) $+ -o $@
apdu.exe: $(OBJ) test/apdu.o
	@$(CC) $+ -o $@
.c.o: $(wildcard include/*.h)
	$(CC) $(CFLAGS) $*.c -o $*.o
.cpp.o: $(wildcard include/*.h)
	$(CC) $(CFLAGS) $*.cpp -o $*.o

clean:
	@rm -rfv $(OBJ) test/*.o *.so *.exe
