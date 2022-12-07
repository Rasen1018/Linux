#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <limits.h>                     /* USHRT_MAX 상수를 위해서 사용한다. */
#include <unistd.h>

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

    // define the kernel
#if 0
   float kernel[3][3] = { {1/9.0, 1/9.0, 1/9.0},
                           {1/9.0, 1/9.0, 1/9.0},
                           {1/9.0, 1/9.0, 1/9.0} };
#else
   float kernel[3][3] = { {0.0, -1.0, 0.0},
                           {-1.0, 5.0, -1.0},
                           {0.0, -1.0, 0.0} };
#endif
    memset(padimg, 0, sizeof(ubyte)*(imageSize+(bmpInfoHeader.biWidth+bmpInfoHeader.biHeight+2)*6));

	printf("\n%d\n", __LINE__);    
	for(y=0; y < bmpInfoHeader.biHeight; y ++) {
		for(x=0; x < size; x+=elemSize) {
			for(z = 0; z < elemSize; z++) {
				padimg[(x+elemSize)+(y+1)*(size+elemSize*2)+z]=inimg[x+y*size+z];
			}
		}
	}

	printf("\n%d\n", __LINE__);    
	memset(outimg, 0, sizeof(ubyte)*imageSize);
    for(y = 1; y < bmpInfoHeader.biHeight + 1; y++) { 
        for(x = elemSize; x < (bmpInfoHeader.biWidth + 1) * elemSize; x+=elemSize) {
            for(z = 0; z < elemSize; z++) {
                float sum = 0.0;
                for(int i = -1; i < 2; i++) {
                    for(int j = -1; j < 2; j++) {
                        sum += kernel[i+1][j+1]*padimg[(x+i*elemSize)+(y+j)*(size+elemSize*2)+z];
                    }
                }
                //outimg[x+y*size+z] = clip(sum, 0 , 255);
				outimg[(x-elemSize)+(y-1)*size+z] = clip(sum, 0, 255);
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

    //fwrite(inimg, sizeof(ubyte), imageSize, fp); 
    fwrite(outimg, sizeof(ubyte), imageSize, fp);

	printf("\n%d\n", __LINE__);    
    fclose(fp); 
    
    free(inimg); 
    free(outimg);
    
	printf("\n%d\n", __LINE__);    
    return 0;
}
