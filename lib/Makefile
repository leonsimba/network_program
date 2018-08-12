PACKAGE_VERSION=1.0
OBJ=error.o readline.o wrap_stdio.o writen.o
DEFINES += -D_GNU_SOURCE
CFLAGS = -g -fPIC
CFLAGS += -Wall -Wstrict-prototypes  -Wmissing-prototypes

all: shared static

LIB_VERSION=1.0
shared: $(OBJ)
	gcc -shared -Wl,--enable-new-dtags 			\
		-Wl,--soname,libnetp.so.$(LIB_VERSION) 	\
        $(OBJ) $(LDFLAGS) 						\
        -o libnetp.so

static: $(OBJ)
	$(AR) rcs libnetp.a $(OBJ)

clean:
	rm -f *.o *.a *.so
