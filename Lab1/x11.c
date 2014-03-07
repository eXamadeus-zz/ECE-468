#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

//test change

void printPixels(unsigned char *, int, int, bool);
void printErr(char *, int);
unsigned char *convertDisplay(unsigned char *, int, int, bool);

void printPixels(unsigned char *data, int low, int high, bool RGB) {
	int i,j = RGB ? 3 : 1;

	printf("%10s | %d\n%10s | %s\n%10s | =====\n", "Depth", 24, "Pixel", "Value", "=====");

	for (i = low*j; i < j * (low+high); i += j) {
		if (j==3)
			printf("%10d | %d %d %d\n", i/j, data[i], data[i+1], data[i+2]);
		else
			printf("%10d | %d\n", i, data[i]);
	}
	printf("%10s%s\n", "==========", " | ===========" );
	printf("%10s | %d\n%10s | %s\n%10s | =====\n", "Depth", 16, "Pixel", "Value", "=====");
	for (i = low*j; i < j * (low+high); i += j) {
		if (j==3) {
			printf("%10d | %d %d %d\n", i/j,
				(data[i]   & 0xF8),
				(data[i+1] & 0xFC),
				(data[i+2] & 0xF8)
			);
		} else {
			printf("%10d   %d\n", i, data[i]);
		}
	}
}

void printErr(char *str, int err) {
	errno = err;
	char *msg  = " Usage: x11 {options}\n\n Options:\n    Specify file\n\t-f {FILENAME}\n    Specify pixel range\n\t-p {count}\n\t-p {count} {start}\n\n ";
	char *pstr = (char *)malloc(strlen(str) + strlen(msg));
	strcat(pstr, msg);
	strcat(pstr, str);
	perror(pstr);
	exit(1);
}

unsigned char *convertDisplay(unsigned char *data, int cols, int rows, bool RGB) {
	unsigned char 	*convert;
	int 			i,j;

	j = 0;

	convert = (unsigned char *) malloc(rows*cols*2);

	for (i = 0; i < rows*cols*(RGB ? 3 : 1); i+=(RGB ? 3 : 1)) {
		convert[j+1]   = (data[i] & 0xF8) + ((data[i+(RGB ? 1 : 0)] & 0xE0) >> 5);
		convert[j] = ((data[i+(RGB ? 1 : 0)] & 0x1C) << 3) + ((data[i+(RGB ? 2 : 0)] & 0xF8) >> 3);
		j+=2;
	}


	return convert;
}

int main (int argc, char *argv[]) {  // return 0 on success, 1 on fail

	Display				*Monitor;
	Window				ImageWindow;
	GC					ImageGC;
	XWindowAttributes	Atts;
	XImage				*Picture;
	int					rows,cols,bytes,
						startPix,countPix,
						i,dispTime;
	unsigned char		*imagedata,*displaydata,
						header[80],*fileName;
	FILE 				*fp;
	bool				RGB;

	//////////////////////////
	/// VERIFY/PARSE INPUT ///
	//////////////////////////

	if (argc < 2) {
		printErr("No file", ENOENT);
	}

	for (i = 0; i < argc-1; i++) {
		if (strcmp(argv[i], "-f") == 0) { // flag == "-f"
			if (strcmp(&argv[i+1][strlen(argv[i+1]) - 4], ".ppm") != 0) {
				printErr(argv[i+1],EINVAL);
			} else {  // open "-f {FILENAME}"
				fp = fopen(argv[i+1],"rb");

				fileName = (unsigned char*)argv[i+1]; // store for later

				if (fp == NULL) {
					printErr(argv[i+1], ENOENT);
				}

				i++; // account for filename
			}
		} else if (strcmp(argv[i], "-p") == 0 && argc >= i + 2) {
			if (argc < i + 3 || argv[i+2][0] == '-') {
				countPix = atoi(argv[i+1]);
				startPix = 0;
				i += 1; // account for pixel limits
			} else {
				startPix = atoi(argv[i+2]);
				countPix = atoi(argv[i+1]);
				i += 2; // account for pixel limit
			}
		} else if (strcmp(argv[i], "-t") == 0 && argc >= i + 2) {
			dispTime = atoi(argv[i+1]);
			i++; // account for time value
		} else if (argc == 3) {
			dispTime = 4;
			startPix = 0;
			countPix = 10;
		} else if (i > 0){
			printErr(argv[i], EINVAL);
		}
	}

	////////////////////////////////
	/// PARSE FILE HEADER & DATA ///
	////////////////////////////////
	
	fscanf(fp, "%s %d %d %d", header, &cols, &rows, &bytes); // parse header
	
	header[79] = fgetc(fp); // account for extra whitespace

	if (strcmp((const char *)&header, "P6") < 0) {
		RGB = false;
	} else {
		RGB = true;
	}

	printf("%10s | %s %d %d %d\n",
		"Header",
		header,
		cols,
		rows,
		bytes
	); // sanity check

	//if (RGB) i = 3; else i = 1;

	imagedata = (unsigned char *) malloc(rows*cols* (RGB ? 3 : 1) ); // dynamically allocate memory

	fread(imagedata, 1, rows*cols*(RGB ? 3 : 1), fp); // read in image

	fclose(fp); // close filestream

	strcpy((char *)fileName,"images/mod.ppm");

	fopen((char *)fileName, "wb");

	sprintf((char *)header,"%s %d %d %d ", (RGB ? "P6" : "P5"), cols, rows, bytes);

	fputs((char *)header, fp);
	
	for (i = 0; i < rows*cols*(RGB ? 3 : 1); i++) {
		fputc(imagedata[i],fp);
	}

	printf( // print image data
		"%10s | %dpx\n%10s | %dpx\n%10s | %d\n%10s | %s\n",
		"Width", cols,
		"Height", rows,
		"Bytes", bytes,
		"RGB", ( RGB ? "True" : "False" )
	);

	if (countPix > rows * cols) { countPix = rows * cols; } // bound check for countPix

	printPixels(imagedata, startPix, countPix, RGB); // print pixel displaydata

	displaydata = convertDisplay(imagedata, cols, rows, RGB);

	////////////////////
	/// DISPLAY CODE ///
	////////////////////

	Monitor = XOpenDisplay(NULL);

	if (Monitor == NULL) {
		printf("Unable to open graphics display\n");
		exit(1);
	}

	ImageWindow = XCreateSimpleWindow(
		Monitor,
		RootWindow(Monitor,0),
		50,10,		// x,y on screen
		cols,rows,	// width, height
		2, 			// border width
		BlackPixel(Monitor,0),
		WhitePixel(Monitor,0)
	);

	ImageGC = XCreateGC(
		Monitor,
		ImageWindow,
		0,
		NULL
	);

	XMapWindow(
		Monitor,
		ImageWindow
	);
	
	XFlush(Monitor);

	while(1) {
		XGetWindowAttributes(
			Monitor,
			ImageWindow,
			&Atts
		);

		if (Atts.map_state == IsViewable /* 2 */) break;
	}

	Picture = XCreateImage(
		Monitor,
		DefaultVisual(Monitor,0),
		DefaultDepth(Monitor,0),
		ZPixmap,	// format
		0,			// offset
		(char *)displaydata,// the data
		cols,rows,	// size of the image data
		16,			// pixel quantum (8, 16 or 32)
		0			// bytes per line (0 causes compute)
	);

	if (dispTime == 0) {
		exit(1);
	}

	XPutImage(
		Monitor,
		ImageWindow,
		ImageGC,
		Picture,
		0,0,0,0,	// src x,y and dest x,y offsets
		cols,rows	// size of the image data
	);

	// XFlush(Monitor);
	sleep(dispTime);
	// XCloseDisplay(Monitor);

	exit(0);
}

//test modification
