CC = gcc

CFLAGS = -Wall -Wextra -I.

LIB = libcalc.a

LIB_OBJS = add.o sub.o mul.o div.o

TARGET = calc

MAIN_OBJS = main.o

all: $(TARGET)

$(TARGET): $(MAIN_OBJS) $(LIB)
	$(CC) $(CFLAGS) -o $@ $(MAIN_OBJS) -L. -lcalc

$(LIB): $(LIB_OBJS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(LIB) $(MAIN_OBJS) $(LIB_OBJS)

.PHONY: all clean
