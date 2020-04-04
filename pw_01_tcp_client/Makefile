OBJS	= main.o print_page.o
SOURCE	= main.c print_page.c
HEADER	= print_page.h
OUT	= main
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c 

print_page.o: print_page.c
	$(CC) $(FLAGS) print_page.c 


clean:
	rm -f $(OBJS) $(OUT)
