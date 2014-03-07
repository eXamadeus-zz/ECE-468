CC=gcc
CFLAGS=-Wall -ggdb -o0
IMGDIR=.

all: x11

x11: x11.c
	$(CC) $(CFLAGS) x11.c -o x11.bin -lX11

#win: win.c
#	$(CC) $(CFLAGS) win.c -o win.bin

clean:
	rm -rf *o *.bin images/mod.ppm
	
test: x11
	./x11.bin -f $(IMGDIR)/retina.ppm -t 1 -p 20 50000 && ./x11.bin -f $(IMGDIR)/bridge.ppm -t 1

img: wimg.c
	$(CC) $(CFLAGS) wimg.c -o wimg.bin
	./wimg.bin -f $(IMGDIR)/base.ppm
