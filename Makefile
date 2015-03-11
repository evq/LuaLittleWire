LITTLEWIRE_SRC = ./littlewirelib #change with your libusb sources folder

OPTS=-Wall -D_GNU_SOURCE
LOPTS=-c -fPIC -fno-stack-protector
OBJS=lualittlewire.o littlewirelib/littleWire.o littlewirelib/littleWire_util.o littlewirelib/littleWire_servo.o littlewirelib/opendevice.o
OBJS_SO = lualittlewire.so

CFLAGS=$(OPTS) -I$(LITTLEWIRE_SRC) -DLINUX=true -std=c99
LDFLAGS=-shared -soname lualittlewire.so.1 -o


all: lualittlewire so

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS) -fPIC

littlewirelib/%.o: littlewirelib/%.c 
	$(CC) -c -o $@ $< $(CFLAGS) -fPIC

lualittlewire: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -c -fPIC -fno-stack-protector 

so: $(OBJS)
		$(LD) -shared -soname lualittlewire.so.1 -o $(OBJS_SO) -lusb $(OBJS)
clean:
	-rm $(OBJS) $(OBJS_SO)
