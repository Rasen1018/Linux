#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <limits.h>                     /* USHRT_MAX 상수를 위해서 사용한다. */
#include <unistd.h>
#include <math.h>

#include "bmpHeader.h"

/* 이미지 데이터의 경계 검사를 위한 매크로 */
#define LIMIT_UBYTE(n) ((n)>UCHAR_MAX)?UCHAR_MAX:((n)<0)?0:(n)

typedef unsigned char ubyte;

inline unsigned char clip(int value, int min, int max);
unsigned char clip(int value, int min, int max)
{
	return(value > max? max : value < min? min : value);
}

int main(int argc, char** argv) 
{
    FILE* fp; 
    BITMAPFILEHEADER bmpHeader;             /* BMP FILE INFO */
    BITMAPINFOHEADER bmpInfoHeader;     /* BMP IMAGE INFO */
    RGBQUAD *palrgb;
    ubyte *inimg, *outimg, *padimg;
	ubyte r, g, b;
	float gray;
    int x, y, z, imageSize;

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

    /* 트루 컬러를 지원하면 변환할 수 없다. */
    if(bmpInfoHeader.biBitCount != 24) {
        perror("This image file doesn't supports 24bit color\n");
        fclose(fp);
        return -1;
    }
	printf("\n%d\n", __LINE__);    
    int elemSize = bmpInfoHeader.biBitCount/8;
    int size = bmpInfoHeader.biWidth*elemSize;
	int padSize = (bmpInfoHeader.biWidth + 2) * elemSize;
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

    memset(padimg, 0, sizeof(ubyte)*(imageSize+(bmpInfoHeader.biWidth+bmpInfoHeader.biHeight+2)*6));

	// 원본 이미지를 gray scale해서 padding
	for(y=0; y < bmpInfoHeader.biHeight; y ++) {
		for(x=0; x < size; x+=elemSize) {
			r = (float)inimg[x+y*size+2];
			g = (float)inimg[x+y*size+1];
			b = (float)inimg[x+y*size+0];
#if 0
			gray = (r*0.3F)+(g*0.59F)+(b*0.11F);

			padimg[(x+elemSize)+(y+1)*(size+elemSize*2)]=gray;
			padimg[(x+elemSize)+(y+1)*(size+elemSize*2)+1]=gray;
			padimg[(x+elemSize)+(y+1)*(size+elemSize*2)+2]=gray;
#else
			padimg[(x+elemSize)+(y+1)*(size+elemSize*2)]=((66*r+129*g+25*b+128)>>8)+16;
			padimg[(x+elemSize)+(y+1)*(size+elemSize*2)+1]=((66*r+129*g+25*b+128)>>8)+16;
			padimg[(x+elemSize)+(y+1)*(size+elemSize*2)+2]=((66*r+129*g+25*b+128)>>8)+16;
#endif
		}
	}

    for(y = 0; y < bmpInfoHeader.biHeight; y++) { 
        for(z = 0; z < elemSize; z++) {
            padimg[0+(y+1)*padSize+z]=padimg[3+(y+1)*padSize+z];
            padimg[padSize-elemSize+(y+1)*padSize+z]=padimg[size-elemSize+(y+1)*padSize+z];
        }
    }

    for(x = 0; x < bmpInfoHeader.biWidth*elemSize; x++) { 
        padimg[elemSize+x]=padimg[x+padSize+elemSize];
        padimg[elemSize+x+(bmpInfoHeader.biHeight+1)*padSize]=
				padimg[elemSize-padSize+x+(bmpInfoHeader. biHeight+1)*padSize];	
    }

    for(z = 0; z < elemSize; z++) {
       padimg[z]=inimg[z];
       padimg[padSize-elemSize+z]=padimg[size-elemSize+z];
       padimg[(bmpInfoHeader.biHeight+2)*padSize+z]=inimg[(bmpInfoHeader.biHeight-1)*size+z];
       padimg[(bmpInfoHeader.biHeight+2)*padSize+padSize-elemSize+z]=
								inimg[(bmpInfoHeader.biHeight-1)*size+size-elemSize+z];
    }
	printf("\n%d\n", __LINE__);    
	memset(outimg, 0, sizeof(ubyte)*imageSize);

	// 3x3 배열의 중간값으로 픽셀 rgb 채우기
	int xFilter[3][3] = { {-1, 0, 1},
                          {-2, 0, 2},
                          {-1, 0, 1} };
	int yFilter[3][3] = { {1, 2, 1},
                          {0, 0, 0},
                          {-1, -2, -1} };
    for(y = 1; y < bmpInfoHeader.biHeight + 1; y++) { 
        for(x = elemSize; x < padSize; x+=elemSize) {
            for(z = 0; z < elemSize; z++) {
                float hedge = 0.0;
				float vedge = 0.0;

                for(int i = -1; i < 2; i++) {
                    for(int j = -1; j < 2; j++) {
						vedge += xFilter[i+1][j+1]*padimg[(x+i*elemSize)+(y+j)*(size+elemSize*2)+z];
						hedge += yFilter[i+1][j+1]*padimg[(x+i*elemSize)+(y+j)*(size+elemSize*2)+z];
                    }
                }
				outimg[(x-elemSize)+(y-1)*size+z] = LIMIT_UBYTE(sqrt(hedge*hedge+vedge*vedge));
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
