#ifndef BMP 
#define BMP
typedef unsigned short WORD ;
typedef unsigned long DWORD ;
typedef long LONG;
typedef unsigned char BYTE;
#define BM 0x4D42
typedef struct tagBITMAPFILEHEADER {
	WORD  bfType;//文件类型
	DWORD bfSize;//文件大小
	WORD  bfReserved1;
	WORD  bfReserved2;
	DWORD bfOffBits;//位图数据的实际开始位置
} BITMAPFILEHEADER;//位图文件头格式
typedef struct tagBITMAPINFOHEADER {
	DWORD biSize;
	LONG  biWidth;
	LONG  biHeight;
	WORD  biPlanes;
	WORD  biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG  biXPelsPerMeter;
	LONG  biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER;//位图信息头
typedef struct tagRGBQUAD {
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
} RGBQUAD;//调色板
#endif