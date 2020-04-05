BUILD=build/
CFLAGS=-W -Wall

ifeq ($(ARCH),arm)
GCC := arm-linux-gnueabi-gcc # ARM Cross compiler
else
GCC := gcc # Standard Compiler
endif

all: utilities client

client: main.c utilities core definitions
	mkdir -p $(BUILD)
	$(GCC) -o $(BUILD)main.o -c main.c $(CFLAGS)
	$(GCC) $(BUILD)main.o $(BUILD)utilities.o $(BUILD)core.o -o client
	cp client client2
	mv client client1

utilities: utilities.c utilities.h definitions
	mkdir -p $(BUILD)
	$(GCC) -o $(BUILD)utilities.o -c utilities.c $(CFLAGS)

core: core.c core.h definitions
	mkdir -p $(BUILD)
	$(GCC) -o $(BUILD)core.o -c core.c $(CFLAGS)

definitions: definitions.h

clean:
	rm -rf $(BUILD)
	rm client1
	rm client2