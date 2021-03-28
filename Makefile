TARGET=test_list
CFLAGS+= -W -D$(print) #-fsanitize=address    # 从终端接收变量, 并传入代码。

plat=x86
ifeq ($(plat), x86)
CC=gcc
else ifeq ($(plat), arm)
CROSS=aarch64-himix100-linux-
CC=aarch64-himix100-linux-gcc
CPP=aarch64-himix100-linux-g++
endif

objs=$(patsubst %.c, %.o, $(wildcard ./src/*.c))
INC+=-I ./include

OUTPUT=$(TARGET).$(plat)
LDFLAGS= -o0 -g -lpthread #-lasan

$(OUTPUT):$(objs)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $(OUTPUT)

%.o:%.c
	$(CC) -c $^ $(INC) $(CFLAGS) -o $@

help:
	@echo "------------------------------------------------"
	@echo "   make plat=x86 print=PRINT_ON"
	@echo "   plat=[x86][arm] print=[PRINT_ON][PRINT_OFF]"
	@echo "------------------------------------------------"
clean:
	rm $(OUTPUT) $(objs)
