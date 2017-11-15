LNAME=slip

APP=test-slip test-zxslip
#
LIBSRC= slip.c zxslip.c
#
LIBO=$(LIBSRC:.c=.o)
LIBH=$(LIBSRC:.c=.h)
#

all: lib$(LNAME).a $(APP)

test-slip: lib$(LNAME).a test-slip.c
	$(CC) -o $@ $@.c -I ./ -L ./ -l slip

test-zxslip: lib$(LNAME).a  test-zxslip.c
	$(CC) -o $@ $@.c -I ./ -L ./ -l slip

lib$(LNAME).a:  $(LIBSRC) $(LIBH)
	$(CC) -c $(LIBSRC)
	$(AR) ar rcs lib$(LNAME).a $(LIBO)

clean:
	rm -f $(APP) lib$(LNAME).a *.o

