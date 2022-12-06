#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <limits.h>                     /* USHRT_MAX 상수를 위해서 사용한다. */
#include <time.h>

#include "bmpHeader.h"

/* 이미지 데이터의 경계 검사를 위한 매크로 */
#define LIMIT_UBYTE(n) ((n)>UCHAR_MAX)?UCHAR_MAX:((n)<0)?0:(n)

typedef unsigned char ubyte;

int main(int argc, char** argv) 
{
    FILE* fp; 
    BITMAPFILEHEADER bmpHeader;             /* BMP FILE INFO */
    BITMAPINFOHEADER bmpInfoHeader;     /* BMP IMAGE INFO */
    ubyte *inimg;
    int i, z, cnt, elemSize, imageSize;
    srand((unsigned int)time(NULL)); 	// seed값으로 현재시간 부여

    if(argc != 4) {
        fprintf(stderr, "usage : %s count input.bmp output.bmp\n", argv[0]);
        return -1;
    }
    
    /***** read bmp *****/ 
    if((fp=fopen(argv[2], "rb")) == NULL) { 
        fprintf(stderr, "Error : Failed to open file...₩n"); 
        return -1;
    }

    /* BITMAPFILEHEADER 구조체의 데이터 */
    fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);

    /* BITMAPINFOHEADER 구조체의 데이터 */
    fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

    /* 트루 컬러를 지원하지 않으면 표시할 수 없다. */
    if(bmpInfoHeader.biBitCount != 24) {
        perror("This image file doesn't supports 24bit color\n");
        fclose(fp);
        return -1;
    }
    
    elemSize = bmpInfoHeader.biBitCount / 8;
    imageSize = bmpInfoHeader.biWidth * elemSize * bmpInfoHeader.biHeight; 

    /* 이미지의 해상도(넓이 × 깊이) */
    printf("Resolution : %d x %d\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
    printf("Bit Count : %d\n", bmpInfoHeader.biBitCount);     /* 픽셀당 비트 수(색상) */
    printf("Image Size : %d\n", imageSize);
    
    inimg = (ubyte*)malloc(sizeof(ubyte)*imageSize); 
    fread(inimg, sizeof(ubyte), imageSize, fp); 
    
    fclose(fp);
    
    cnt = atoi(argv[1]);
    for(i = 0; i < cnt; i++) { 
        int pos = rand( ) % (bmpInfoHeader.biWidth * bmpInfoHeader.biHeight);
        int value = rand( ) & 0b11111111;
        for(z = 0; z < elemSize; z++) {
            int tmp = inimg[pos*elemSize+z] + value;
            inimg[pos*elemSize+z] = LIMIT_UBYTE(tmp);
        }
     }         
     
    /***** write bmp *****/ 
    if((fp=fopen(argv[3], "wb"))==NULL) { 
        fprintf(stderr, "Error : Failed to open file...₩n"); 
        return -1;
    }
    
    /* BITMAPFILEHEADER 구조체의 데이터 */
    fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);

    /* BITMAPINFOHEADER 구조체의 데이터 */
    fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

    fwrite(inimg, sizeof(unsigned char), imageSize, fp);
    
    free(inimg); 
    
    fclose(fp); 
    
    return 0;
}
