XENO_CONFIG := xeno-config

CFLAGS := $(shell $(XENO_CONFIG) --posix --alchemy --cflags)
LDFLAGS := $(shell $(XENO_CONFIG) --posix --alchemy --ldflags)

CC := gcc   
EXECUTABLE := task3

all: $(EXECUTABLE)

%: %.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

clean: 
	rm -f $(EXECUTABLE)
