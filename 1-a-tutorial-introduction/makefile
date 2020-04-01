SOURCE :=  $(wildcard *.c)
TARGET := $(patsubst %.c, %, $(SOURCE))

all: $(TARGET)

%: %.c
	gcc $< -o ../binary/$@
