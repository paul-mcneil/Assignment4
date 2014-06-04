OBJS = tokenizer.o index.o hashmap.o search.o
CC = gcc
CFLAGS = -g -Wall

search: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o search 
