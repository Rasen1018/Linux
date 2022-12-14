#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include "bmpHeader.h"

// 픽셀값 경계 처리
#define LIMIT_UBYTE(n) ((n) > UCHAR_MAX) ? UCHAR_MAX : ((n) < 0) ? : 0 : n

// rgb가 255가 넘을 경우 255로 최대값 처리
inline unsigned char clip(int value, int min, int max);
inline unsigned char clip(int value, int min, int max) {
	return (value > max ? max : value < min ? min : value);
}

typedef unsigned char ubyte;

int main(int argc, char **argv) {

	// bmp 헤더 선언
	FILE *fp;
	BITMAPFILEHEADER bmpHeader;
	BITMAPINFOHEADER bmpInfo;
	RGBQUAD *palrgb;
	ubyte *inimg, *outimg;

	// argument count 오류시 종료
	if(argc != 3) {
		fprintf(stderr, "please Usage : %s input.bmp output.bmp\n", argv[0]);
	}

	// 파일 오픈시 없으면 종료
	if((fp = fopen(argv[1], "rb")) == NULL) {
		fprintf(stderr, "Error : Failed to open file...\n");
		return -1;
	}

	// bmp 파일에서 헤더로 정보 가져오기
	fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, fp);

	// 트루 컬러가 아닌 이미지면 종료
	if(bmpInfo.biBitCount != 24) {
		perror("this image is not 24 bits.\n");
		fclose(fp);
		return -1;
	}

	int elemSize = bmpInfo.biBitCount / 8;
	int size = bmpInfo.biWidth * elemSize;
	int imgSize = size * bmpInfo.biHeight;

	// 이미지 복사에 필요한 변수들 메모리 할당
	inimg = (ubyte*)malloc(sizeof(ubyte)*imgSize);
	outimg = (ubyte*)malloc(sizeof(ubyte)*imgSize);
	palrgb = (RGBQUAD*)malloc(sizeof(RGBQUAD)*256);

	// bmp 이미지 읽어오기
	fread(inimg, sizeof(ubyte), imgSize, fp);

	// 이미지 복사할 버퍼 0으로 초기화
	memset(outimg, 0, sizeof(ubyte)*imgSize);

	fclose(fp);

	// 이미지 복사
	for (int y = 0 ; y < imgSize; y +=elemSize) {
		*(outimg+y) = *(inimg+y);
		*(outimg+y+1) = *(inimg+y+1);
		*(outimg+y+2) = *(inimg+y+2);
	}

	// 팔레트 추가
	for (int x = 0 ; x < 256; x++) {
		palrgb[x].rgbBlue = palrgb[x].rgbGreen = palrgb[x].rgbRed = x;
		palrgb[x].rgbReserved = 0;
	}

	// bmpheader 값 설정
	bmpInfo.biBitCount = 24;
	bmpInfo.biClrUsed = 0;

	// 팔레트만큼 오프셋 설정
	bmpHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
	bmpHeader.bfSize = bmpHeader.bfOffBits + bmpInfo.SizeImage;

	// 파일 오픈시 없는 파일이면 종료
	if((fp=fopen(argv[2], "wb")) == NULL) {
		fprintf(stderr, "Error : Failed to open file ...\n");
		return -1;
	}

	// 파일 스트림으로 bmpheader, palette, 복사한 이미지 저장
	fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(palrgb, sizeof(RGBQUAD), 256, fp);
	fwrite(outimg, sizeof(ubyte), imgSize, fp);

	fclose(fp);

	// 메모리 제거
	free(inimg);
	free(outimg);
	free(palrgb);

	return 0;
}
