SUBDIRS=daytime echo

all: 
	for i in $(SUBDIRS); do \
		$(MAKE) $(MFLAGS) -C $$i; \
	done

install:
	for i in $(SUBDIRS); do \
		$(MAKE) $(MFLAGS) -C $$i install; \
	done

clean:
	for i in $(SUBDIRS); do \
		$(MAKE) $(MFLAGS) -C $$i clean; \
	done

