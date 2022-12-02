#ifndef __BMP_FILE_H__
#define __BMP_FILE_H__
#pragma pack(1)

typedef struct {
	unsigned short bfType;			/* BM 표시: 2바이트*/
	unsigned int bfSize;			/* 파일의 크기: 4바이트 */
	unsigned short bfReserved1;		/* 추후 확장을 위한 필드 */
	unsigned short bfReserved2;		/* 추후 확장을 위한 필드 */
	unsigned int bfOffBits;			/* 실제 이미지까지의 오프셋: 바이트 */
} BITMAPFILEHEADER;					/* BMP 파일 데이터를 위한 구조체 */

typedef struct {
	unsigned int biSize;			/* 현 구조체의 크기 */
	unsigned int biWidth;			/* 이미지의 폭(픽셀 단위) */
	unsigned int biHeight;			/* 이미지의 높이(픽셀 단위) */
	unsigned short biPlanes;		/* 비트 플레인 수(항상 1) */
	unsigned short biBitCount;		/* 픽셀당 비트 수 */
	unsigned int biCompression;		/* 압축 유형 */
	unsigned int SizeImage;			/* 이미지의 크기(압축 전 바이트 단위) */
	unsigned int biXPelsPerMeter;	/* 가로 해상도 */
	unsigned int biYPelsPerMeter;	/* 세로 해상도 */
	unsigned int biClrUsed;			/* 실제 사용되는 색상 수 */
	unsigned int biClrImportant;	/* 중요한 색상 인덱스(0인 경우 전체) */
} BITMAPINFOHEADER;					/* BMP 이미지 데이터를 위한 구조체 */

typedef struct {
	unsigned char rgbBlue;			
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;		/* 투명도(알파값) 등을 위한 예약 공간 */
} RGBQUAD;							/* 색상 팔레트를 위한 구조체 */


#endif /* __BMP_FILE_H__ */