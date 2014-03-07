#include <windows.h>
#include <stdio.h>

#define IS_RGB 1 // 1 is yes
//#define FILENAME "bridge.ppm"

unsigned char *convertDisplay(unsigned char *, int, int, BOOL);

LRESULT CALLBACK EventProcessor (HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam) {
	return(DefWindowProc(hWnd,uMsg,wParam,lParam));
}

unsigned char *convertDisplay(unsigned char *data, int cols, int rows, BOOL RGB) {
	int 			i,j;
	unsigned char	*convert = NULL;

	convert = (unsigned char *) malloc(rows*cols*2);

	j = 0;
	for (i = 0; i < rows*cols*(RGB ? 3 : 1); i+=(RGB ? 3 : 1)) {
		convert[j+1] = ((data[i] & 0xF8) >> 1) + ((data[i+(RGB ? 1 : 0)] & 0xC0) >> 6);
		convert[j] = ((data[i+(RGB ? 1 : 0)] & 0x38) << 2) + ((data[i+(RGB ? 2 : 0)] & 0xF8) >> 3);
		j+=2;
	}

	return convert;
}

int WINAPI WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASS		wc;
	HWND			WindowHandle = NULL;
	int				ROWS,COLS,BYTES,i;
	unsigned char	header[80];
	unsigned char	*displaydata,*imagedata;
	BITMAPINFO		*bm_info = NULL;
	HDC				hDC;
	FILE			*fp;
	BOOL			RGB;
	char			FILENAME[20];

	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= (WNDPROC)EventProcessor;
	wc.cbClsExtra		= wc.cbWndExtra=0;
	wc.hInstance		= hInstance;
	wc.hIcon			= NULL;
	wc.lpszMenuName		= NULL;
	wc.hCursor			= LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName	= "Image Window Class";
	
	if (RegisterClass(&wc) == 0)
		exit(0);

	if (MessageBox(NULL,"Want greyscale?","Greyscale", MB_YESNO) == IDYES)
		strcpy(FILENAME,"bridge.ppm");
	else
		strcpy(FILENAME,"retina.ppm");

	fp = fopen(FILENAME,"rb");

	if (fp == NULL) {
		errno = ERROR_FILE_NOT_FOUND;
		exit(errno);
	}

	fscanf(fp, "%s %d %d %d", header, &COLS, &ROWS, &BYTES); // parse header
	
	header[79] = fgetc(fp); // account for extra whitespace

	if (strcmp((const char *)&header, "P6") < 0) {
		RGB = FALSE;
	} else {
		RGB = TRUE;
	}

	sprintf((char *)header,"%s %d %d %d\n",
		header,
		COLS,
		ROWS,
		BYTES
	); // sanity check

	imagedata = (unsigned char *) malloc(ROWS*COLS* (RGB ? 3 : 1) ); // dynamically allocate memory

	fread(imagedata, 1, ROWS*COLS*(RGB ? 3 : 1), fp); // read in image

	fclose(fp);

	displaydata = (RGB ? convertDisplay(imagedata, COLS, ROWS, RGB) : imagedata);

	WindowHandle = CreateWindow("Image Window Class","ECE468 Lab1",
								WS_OVERLAPPEDWINDOW,
								10,10,COLS,ROWS,
								NULL,NULL,hInstance,NULL);
	
	if (WindowHandle == NULL) {
		MessageBox(NULL,"No window","Try again",MB_OK | MB_APPLMODAL);
		exit(0);
	}

	ShowWindow (WindowHandle, SW_SHOWNORMAL);

	bm_info=(BITMAPINFO *)calloc(1,sizeof(BITMAPINFO) + 256*sizeof(RGBQUAD));
	hDC=GetDC(WindowHandle);

	//if (RGB) {
	bm_info->bmiHeader.biBitCount = (RGB ? 16 : 8);
	//} else {
	//	bm_
	//}

	/* ... set up bmiHeader field of bm_info ... */
	bm_info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bm_info->bmiHeader.biWidth = COLS;
	bm_info->bmiHeader.biHeight = (RGB ? -ROWS : -ROWS);
	bm_info->bmiHeader.biPlanes = 1;
	bm_info->bmiHeader.biCompression = BI_RGB;
	bm_info->bmiHeader.biSizeImage = 0;
	bm_info->bmiHeader.biClrUsed = 0;
	bm_info->bmiHeader.biClrImportant = 0;
	
	for (i=0; i<256; i++) {
		bm_info->bmiColors[i].rgbBlue=bm_info->bmiColors[i].rgbGreen=bm_info->bmiColors[i].rgbRed=i;
		bm_info->bmiColors[i].rgbReserved=0;
	}

	i = SetDIBitsToDevice(
		hDC,0,0,COLS,ROWS,0,0,
		0, /* first scan line */
		ROWS, /* number of scan lines */
		displaydata,bm_info,DIB_RGB_COLORS
	);
	
	ReleaseDC(WindowHandle,hDC);
	free(bm_info);
	if (RGB) free(imagedata);
	free(displaydata);
	MessageBox(NULL, "Press OK to continue", "", MB_OK | MB_APPLMODAL);
}
