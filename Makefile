LNAME=slip

TESTS=test-slip test-zxslip
#
LIBSRC= slip.c zxslip.c slipcrc8.c
#
LIBO=$(LIBSRC:.c=.o)
LIBH=$(LIBSRC:.c=.h)
#

all: libs$(CPU_TYPE)
	@echo "All done LIBS"
	

libs: lib$(LNAME).a
	@echo "Libs DEFAULT"

libsz80:
	make -f Makefile.z80
	@echo "Libs Z80"

test: all test-slip test-zxslip
	@echo "All done TESTS"

test-slip: lib$(LNAME).a test/test-slip.c
	$(CC) -o $@ test/$@.c -I ./ -L ./ -l slip

test-zxslip: lib$(LNAME).a  test/test-zxslip.c
	$(CC) -o $@ test/$@.c -I ./ -L ./ -l slip

lib$(LNAME).a:  $(LIBSRC) $(LIBH)
	$(CC) -c $(LIBSRC)
	$(AR) ar rcs lib$(LNAME).a $(LIBO)

clean: clean-t$(CPU_TYPE)

clean-t:
	rm -f $(TESTS) lib$(LNAME).a *.o

clean-tz80:
	make -f Makefile.z80 clean
