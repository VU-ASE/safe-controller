.PHONY: build start clean

#
# By modifying ./boot.json you can change the bootspec that is injected by roverd normally, so you can test in isolation. 
#

build: 
	@echo "Building src/main.c with local source files and debug flags"
	@gcc -o main \
	./src/*.c ./src/rovercom/outputs/*.c ./src/rovercom/tuning/*.c \
	-lcjson -lzmq -lprotobuf-c -lhashtable -llist -I/usr/include/cjson -I./include -g

clean:
	@rm -f main

start: build
	@echo "Starting program with injected bootspec"
	@ASE_SERVICE=$$(cat test/boot.json) && export ASE_SERVICE && echo "Starting program" && ./main

debug: build
	@echo "Starting program in debug mode with injected bootspec"
	@ASE_SERVICE=$$(cat test/boot.json) && export ASE_SERVICE && echo "Starting program" && gdb ./main