SUBDIRS=src

all:
	for i in $(SUBDIRS); do \
		$(MAKE) $(MFLAGS) -C $$i; \
	done
clean:
	for i in $(SUBDIRS); do \
		$(MAKE) $(MFLAGS) -C $$i clean; \
	done
