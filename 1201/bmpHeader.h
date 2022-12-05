#ifndef __BMP_FILE_H__
#define __BMP_FILE_H__
#pragma pack(1)

typedef struct {
	unsigned short bfType;			/* BM ǥ��: 2����Ʈ*/
	unsigned int bfSize;			/* ������ ũ��: 4����Ʈ */
	unsigned short bfReserved1;		/* ���� Ȯ���� ���� �ʵ� */
	unsigned short bfReserved2;		/* ���� Ȯ���� ���� �ʵ� */
	unsigned int bfOffBits;			/* ���� �̹��������� ������: ����Ʈ */
} BITMAPFILEHEADER;					/* BMP ���� �����͸� ���� ����ü */

typedef struct {
	unsigned int biSize;			/* �� ����ü�� ũ�� */
	unsigned int biWidth;			/* �̹����� ��(�ȼ� ����) */
	unsigned int biHeight;			/* �̹����� ����(�ȼ� ����) */
	unsigned short biPlanes;		/* ��Ʈ �÷��� ��(�׻� 1) */
	unsigned short biBitCount;		/* �ȼ��� ��Ʈ �� */
	unsigned int biCompression;		/* ���� ���� */
	unsigned int SizeImage;			/* �̹����� ũ��(���� �� ����Ʈ ����) */
	unsigned int biXPelsPerMeter;	/* ���� �ػ� */
	unsigned int biYPelsPerMeter;	/* ���� �ػ� */
	unsigned int biClrUsed;			/* ���� ���Ǵ� ���� �� */
	unsigned int biClrImportant;	/* �߿��� ���� �ε���(0�� ��� ��ü) */
} BITMAPINFOHEADER;					/* BMP �̹��� �����͸� ���� ����ü */

typedef struct {
	unsigned char rgbBlue;			
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;		/* ������(���İ�) ���� ���� ���� ���� */
} RGBQUAD;							/* ���� �ȷ�Ʈ�� ���� ����ü */


#endif /* __BMP_FILE_H__ */