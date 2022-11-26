CC=gcc
CFLAGS=-g

SRCDIR=src
INCLDIR=include
LIBDIR=lib

mapreduce: $(SRCDIR)/mapreduce.c $(LIBDIR)/utils.o mapper reducer
	$(CC) $(CFLAGS) -I$(INCLDIR) $(LIBDIR)/utils.o $(SRCDIR)/mapreduce.c -o mapreduce

mapper: $(SRCDIR)/mapper.c $(LIBDIR)/utils.o
	$(CC) $(CFLAGS) -I$(INCLDIR) $(LIBDIR)/utils.o $(SRCDIR)/mapper.c -o mapper

reducer: $(SRCDIR)/reducer.c $(LIBDIR)/utils.o
	$(CC) $(CFLAGS) -I$(INCLDIR)  $(LIBDIR)/utils.o $(SRCDIR)/reducer.c -o reducer

$(LIBDIR)/utils.o: $(LIBDIR)/utils.c
	$(CC) $(CFLAGS) -I$(INCLDIR) -c $(LIBDIR)/utils.c -o $(LIBDIR)/utils.o	

.PHONY: run clean t1 t2 t3

#500KB
t1:
# 	make -i clean
# 	make
	./mapreduce 5 2 test/T1/F1.txt

clean:
	rm lib/utils.o mapreduce mapper reducer
	rm -rf output