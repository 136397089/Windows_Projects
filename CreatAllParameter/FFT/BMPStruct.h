#ifndef BMP 
#define BMP
typedef unsigned short WORD ;
typedef unsigned long DWORD ;
typedef long LONG;
typedef unsigned char BYTE;
#define BM 0x4D42
typedef struct tagBITMAPFILEHEADER {
	WORD  bfType;//�ļ�����
	DWORD bfSize;//�ļ���С
	WORD  bfReserved1;
	WORD  bfReserved2;
	DWORD bfOffBits;//λͼ���ݵ�ʵ�ʿ�ʼλ��
} BITMAPFILEHEADER;//λͼ�ļ�ͷ��ʽ
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
} BITMAPINFOHEADER;//λͼ��Ϣͷ
typedef struct tagRGBQUAD {
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
} RGBQUAD;//��ɫ��
#endif