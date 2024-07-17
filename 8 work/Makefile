CC = gcc

CFLAGS = -Wall -Wextra -fPIC -I.

LIB = libcalc.so

LIB_OBJS = add.o sub.o mul.o div.o

TARGET = calc

MAIN_OBJS = main.o

all: $(TARGET)

$(TARGET): $(MAIN_OBJS) $(LIB)
	$(CC) $(CFLAGS) -o $@ $(MAIN_OBJS) -L. -lcalc

$(LIB): $(LIB_OBJS)
	$(CC) -shared -o $@ $^

%.o: %.c calc.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(LIB) $(MAIN_OBJS) $(LIB_OBJS)

.PHONY: all clean
