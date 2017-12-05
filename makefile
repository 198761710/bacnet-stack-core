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
TARGET += mstp.exe
#TARGET+= bacnet-core.a
#bacnet-core.a:$(OBJ)
#	@$(AR) -rc $@ $+	

target: $(TARGET)
	@$(STRIP) $+
	@ls -sh $+
install:bacnet-core.a
	[ -d lib ] || mkdir lib
bacnet-core.a:$(OBJ)
	@$(AR) -rc $@ $+
mstp.exe:$(OBJ) ztest/mstp.o
	@$(CC) $+ -o $@
recvframe.exe:$(OBJ) ztest/recvframe.o
	@$(CC) $+ -o $@
sendframe.exe:$(OBJ) ztest/sendframe.o
	@$(CC) $+ -o $@
bacnet.exe: $(OBJ) ztest/bacnet.o
	@$(CC) $+ -o $@
test.exe: $(OBJ) ztest/test.o
	@$(CC) $+ -o $@
bacapp.exe: $(OBJ) ztest/bacapp.o
	@$(CC) $+ -o $@
apdu.exe: $(OBJ) ztest/apdu.o
	@$(CC) $+ -o $@
.c.o: $(wildcard include/*.h)
	$(CC) $(CFLAGS) $*.c -o $*.o
.cpp.o: $(wildcard include/*.h)
	$(CC) $(CFLAGS) $*.cpp -o $*.o

clean:
	@rm -rfv $(OBJ) ztest/*.o *.so *.exe
