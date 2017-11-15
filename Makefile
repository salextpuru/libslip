LNAME=slip

APP=test

all: lib$(LNAME).a
	$(CC) -o $(APP) main.c -I ./ -L ./ -l slip

lib$(LNAME).a: slip.c slip.h
	$(CC) -c slip.c
	$(AR) ar rcs lib$(LNAME).a slip.o

clean:
	rm -f $(APP) lib$(LNAME).a *.o

