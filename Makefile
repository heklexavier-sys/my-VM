CC = gcc
CFLAGS := -O2

VM_OBJ = vm.o main.o

# compile the library so we can link it
include C-lib/clib.mk

all: vm

clean:
	rm -rf $(VM_OBJ)

clean-all: clean clean-clib

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(CLIB_INCLUDE_LOC)

vm: $(VM_OBJ) $(CLIB_STATIC_LIB)
	$(CC) $(VM_OBJ) -o $@ -static -l$(CLIB_LIB) -L$(CLIB_LIB_LOC)
	@echo "vm binary built"

.PHONY: all clean clean-all
