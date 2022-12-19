#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include "bmpHeader.h"

typedef unsigned char ubyte;

int main(int argc, char **argv) {

	// bmp 헤더 선언
	FILE *fp;
	BITMAPFILEHEADER bmpHeader;
	BITMAPINFOHEADER bmpInfo;
	ubyte *inimg;

	// argument count 오류시 종료
	if(argc != 2) {
		fprintf(stderr, "please Usage : %s input.bmp\n", argv[0]);
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

	// bmp 이미지 읽어오기
	fread(inimg, sizeof(ubyte), imgSize, fp);

	// 이미지 복사
	for (int y = 0 ; y < imgSize; y +=elemSize) {
		inimg[y] = b;
		inimg[y+1] = g;
		inimg[y+2] = r;
		printf("pixel : (%d, %d, %d)", b, g, r);
	}

	fclose(fp);

	// 메모리 제거
	free(inimg);

	return 0;
}
