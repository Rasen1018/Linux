#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <limits.h>                     /* USHRT_MAX 상수를 위해서 사용한다. */
#include <unistd.h>
#include <time.h>

#include "bmpHeader.h"

/* 이미지 데이터의 경계 검사를 위한 매크로 */
#define LIMIT_UBYTE(n) ((n)>UCHAR_MAX)?UCHAR_MAX:((n)<0)?0:(n)

typedef unsigned char ubyte;

inline unsigned char clip(int value, int min, int max);
unsigned char clip(int value, int min, int max)
{
	return(value > max? max : value < min? min : value);
}

int static compare (const void* first, const void* second)
{
	if(*(int*)first > *(int*)second)
		return 1;
	else if (*(int*)first < *(int*)second)
		return -1;
	else
		return 0;
}

void insertion(int a[], int n)
{
	int i, j;
	for (i = 1; i < n; i++) {
		int tmp = a[i];
		for(j = i; j > 0 && a[j-1] > tmp; j --) {
			a[j] = a[j-1];
			a[j] = tmp;
		}
	}
}

int main(int argc, char** argv) 
{
    FILE* fp; 
    BITMAPFILEHEADER bmpHeader;             /* BMP FILE INFO */
    BITMAPINFOHEADER bmpInfoHeader;     /* BMP IMAGE INFO */
    RGBQUAD *palrgb;
    ubyte *inimg, *outimg, *padimg;
    int x, y, z, imageSize;
	srand((unsigned int)time(NULL));

    if(argc != 3) {
        fprintf(stderr, "usage : %s input.bmp output.bmp\n", argv[0]);
        return -1;
    }
    
    /***** read bmp *****/ 
    if((fp=fopen(argv[1], "rb")) == NULL) { 
        fprintf(stderr, "Error : Failed to open file...₩n"); 
        return -1;
    }

    /* BITMAPFILEHEADER 구조체의 데이터 */
    fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);

    /* BITMAPINFOHEADER 구조체의 데이터 */
    fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	printf("Bit Count : %d\n", bmpInfoHeader.biBitCount);

    /* 트루 컬러를 지원하면 변환할 수 없다. */
    if(bmpInfoHeader.biBitCount != 24) {
        perror("This image file doesn't supports 24bit color\n");
        fclose(fp);
        return -1;
    }
	printf("\n%d\n", __LINE__);    
    int elemSize = bmpInfoHeader.biBitCount/8;
    int size = bmpInfoHeader.biWidth*elemSize;
    imageSize = size * bmpInfoHeader.biHeight; 

    /* 이미지의 해상도(넓이 × 깊이) */
    printf("Resolution : %d x %d\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
    printf("Bit Count : %d\n", bmpInfoHeader.biBitCount);     /* 픽셀당 비트 수(색상) */
    printf("Image Size : %d\n", imageSize);

    inimg = (ubyte*)malloc(sizeof(ubyte)*imageSize); 
	padimg = (ubyte*)malloc(sizeof(ubyte)*(imageSize+(bmpInfoHeader.biWidth+bmpInfoHeader.biHeight+2)*6));
    outimg = (ubyte*)malloc(sizeof(ubyte)*imageSize);
    fread(inimg, sizeof(ubyte), imageSize, fp); 
    
	printf("\n%d\n", __LINE__);    
    fclose(fp);

	// 원본 이미지에 whitenoise 추가
	for(int i = 0; i < 3000; i++) {
		int pos = rand()%(bmpInfoHeader.biWidth * bmpInfoHeader.biHeight);
		int value = rand()&0b11111111;
		for(z = 0; z < elemSize; z++) {
			int tmp = inimg[pos*elemSize+z] + value;
			inimg[pos*elemSize+z] = LIMIT_UBYTE(tmp);
		}
	}

    memset(padimg, 0, sizeof(ubyte)*(imageSize+(bmpInfoHeader.biWidth+bmpInfoHeader.biHeight+2)*6));

	// noise된 이미지를 padding
	for(y=0; y < bmpInfoHeader.biHeight; y ++) {
		for(x=0; x < size; x+=elemSize) {
			for(z = 0; z < elemSize; z++) {
				padimg[(x+elemSize)+(y+1)*(size+elemSize*2)+z]=inimg[x+y*size+z];
			}
		}
	}

	printf("\n%d\n", __LINE__);    
	memset(outimg, 0, sizeof(ubyte)*imageSize);

	// 3x3 배열의 중간값으로 픽셀 rgb 채우기
	int kernel[9] ={0,};
    for(y = 1; y < bmpInfoHeader.biHeight + 1; y++) { 
        for(x = elemSize; x < (bmpInfoHeader.biWidth) * elemSize; x+=elemSize) {
            for(z = 0; z < elemSize; z++) {
                float sum = 0.0;
				int cnt = 0;

                for(int i = -1; i < 2; i++) {
                    for(int j = -1; j < 2; j++) {
						kernel[cnt++]=padimg[(x+i*elemSize)+(y+j)*(size+elemSize*2)+z];	
                    }
                }
				qsort(kernel, sizeof(kernel)/sizeof(int), sizeof(int), compare);

				outimg[(x-elemSize)+(y-1)*size+z] = clip(kernel[0], 0, 255);
            }
        }
    }         
     
	printf("\n%d\n", __LINE__);    
    /***** write bmp *****/ 
    if((fp=fopen(argv[2], "wb"))==NULL) { 
        fprintf(stderr, "Error : Failed to open file...₩n"); 
        return -1;
    }

	printf("\n%d\n", __LINE__);    
    /* BITMAPFILEHEADER 구조체의 데이터 */
    fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);

    /* BITMAPINFOHEADER 구조체의 데이터 */
    fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

    fwrite(outimg, sizeof(ubyte), imageSize, fp);

	printf("\n%d\n", __LINE__);    
    fclose(fp); 
    
    free(inimg); 
    free(outimg);
    
	printf("\n%d\n", __LINE__);    
    return 0;
}
