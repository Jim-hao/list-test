TARGET=test_list
CFLAGS+= -W -D$(print)            # 从终端接收变量, 并传入代码。
                                  # 定义CFLAGS等同于 $(CC) $(SRC) -D$(PRINT) -o $(OUTPUT)

CROSS=aarch64-himix100-linux-
plat=x86
ifeq ($(plat), x86)
CC=gcc
else ifeq ($(plat), arm)
CC=$(CROSS)gcc
CPP=$(CROSS)g++
endif

SRC+=./src/list.c
SRC+=./src/osa.c
INC+=-I ./include

OUTPUT=$(TARGET).$(plat)
LDFLAGS= -o0 -g -lpthread

all:
	$(CC) $(SRC) $(INC) $(CFLAGS) $(LDFLAGS) -o $(OUTPUT)
help:
	@echo "------------------------------------------------"
	@echo "   make plat=x86 print=PRINT_ON"
	@echo "   plat=[x86][arm] print=[PRINT_ON][PRINT_OFF]"
	@echo "------------------------------------------------"
clean:
	rm $(OUTPUT) ./*.o
