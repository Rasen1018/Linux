#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <limits.h>                     /* USHRT_MAX 상수를 위해서 사용한다. */
#include <math.h>
#include <iostream>

#include "bmpHeader.h"

/* 이미지 데이터의 경계 검사를 위한 매크로 */
#define LIMIT_UBYTE(n) ((n)>UCHAR_MAX)?UCHAR_MAX:((n)<0)?0:(n)
#define widthbytes(bits) (((bits)+31)/32*4)

typedef unsigned char ubyte;

//Cuda kernel for converting RGB image into a GreyScale image
__global__ void convertBlur(ubyte *in, ubyte *out, int rows, int cols, int elemSize) {
    int x = threadIdx.x + blockIdx.x * blockDim.x;
    int y = threadIdx.y + blockIdx.y * blockDim.y;
    int z = threadIdx.z + blockIdx.z * blockDim.z;
    
    // define the kernel
    float kernel[3][3] = { {1/9.0, 1/9.0, 1/9.0},
                           {1/9.0, 1/9.0, 1/9.0},
                           {1/9.0, 1/9.0, 1/9.0} };

    int offset = (x + y * cols) * elemSize;
	//for(int z = 0; z < elemSize; z++) {
    if((x > 0 && x < (cols - 1)) && ((y > 0) && (y < (rows-1)))) {
            float sum = 0.0;
            for(int i = -1; i < 2; i++) {
                for(int j = -1; j < 2; j++) {
                    //sum += in[((x-i)+(y-j)*cols)*elemSize+z];
		    sum += kernel[i+1][j+1]*in[((x-i)+(y-j)*cols)*elemSize+z];
                }
            }
            //out[offset+z] = sum/9.;
            out[offset+z] = sum;
    } else if (y == 0) {
        float sum = 0.0;
        ubyte arr[9];
	if(x == 0) {
            arr[0] = arr[1] = arr[3] = arr[4] = in[x*elemSize+z];
	    arr[2] = arr[5] = in[(x+1)*elemSize+z];
	    arr[6] = arr[7] = in[(x+(y+1)*cols)*elemSize+z];
	    arr[8] = in[((x+1)+(y+1)*cols)*elemSize+z];
	} else if(x == (cols-1)) {
            arr[1] = arr[2] = arr[4] = arr[5] = in[x*elemSize+z];
	    arr[0] = arr[3] = in[(x-1)*elemSize+z];
	    arr[6] = in[((x-1)+(y+1)*cols)*elemSize+z];
	    arr[7] = arr[8] = in[(x+(y+1)*cols)*elemSize+z];
        } else {
            arr[3] = arr[0] = in[(x-1)*elemSize+z];
            arr[4] = arr[1] = in[x*elemSize+z];
            arr[5] = arr[2] = in[(x+1)*elemSize+z];
	    arr[6] = in[((x-1)+(y+1)*cols)*elemSize+z];
	    arr[7] = in[(x+(y+1)*cols)*elemSize+z];
	    arr[8] = in[((x+1)+(y+1)*cols)*elemSize+z];
	}

	for(int i = 0; i < 9; i++)
	    sum += arr[i];
        out[offset+z] = sum/9;
    } else if (y == (rows - 1)) {
        float sum = 0.0;
        ubyte arr[9];
	if(x == 0) {
            arr[6] = arr[7] = \
	    arr[3] = arr[4] = in[(x  +(y  )*cols)*elemSize+z];
	    arr[8] = arr[5] = in[(x+1+(y  )*cols)*elemSize+z];
	    arr[0] = arr[1] = in[(x  +(y-1)*cols)*elemSize+z];
	    arr[2] = in[((x+1)+(y-1)*cols)*elemSize+z];
	} else if(x == (cols-1)) {
            arr[7] = arr[8] = \
            arr[4] = arr[5] = in[(x  +(y  )*cols)*elemSize+z];
	    arr[6] = arr[3] = in[(x-1+(y  )*cols)*elemSize+z];
	    arr[1] = arr[2] = in[(x  +(y-1)*cols)*elemSize+z];
	    arr[0] = in[((x-1)+(y-1)*cols)*elemSize+z];
        } else {
            arr[3] = arr[6] = in[(x-1+y*cols)*elemSize+z];
            arr[4] = arr[7] = in[(x  +y*cols)*elemSize+z];
            arr[5] = arr[8] = in[(x+1+y*cols)*elemSize+z];
	    arr[0] = in[((x-1)+(y-1)*cols)*elemSize+z];
	    arr[1] = in[((x  )+(y-1)*cols)*elemSize+z];
	    arr[2] = in[((x+1)+(y-1)*cols)*elemSize+z];
	}

	for(int i = 0; i < 9; i++)
	    sum += arr[i];
        out[offset+z] = sum/9;
    } else if (x == 0) {
        float sum = 0.0;
        ubyte arr[9];
        arr[0] = arr[1] = in[(x+(y-1)*cols)*elemSize+z];
        arr[3] = arr[4] = in[(x+(y  )*cols)*elemSize+z];
        arr[6] = arr[7] = in[(x+(y+1)*cols)*elemSize+z];
	arr[2] = in[((x+1)+(y-1)*cols)*elemSize+z];
	arr[5] = in[((x+1)+(y  )*cols)*elemSize+z];
	arr[8] = in[((x+1)+(y+1)*cols)*elemSize+z];
	for(int i = 0; i < 9; i++)
	    sum += arr[i];
        out[offset+z] = sum/9;
    } else if (x == (cols-1)) {
        float sum = 0.0;
        ubyte arr[9];
        arr[1] = arr[2] = in[(x+(y-1)*cols)*elemSize+z];
        arr[4] = arr[5] = in[(x+(y  )*cols)*elemSize+z];
        arr[7] = arr[8] = in[(x+(y+1)*cols)*elemSize+z];
	arr[0] = in[((x-1)+(y-1)*cols)*elemSize+z];
	arr[3] = in[((x-1)+(y  )*cols)*elemSize+z];
	arr[6] = in[((x-1)+(y+1)*cols)*elemSize+z];
	for(int i = 0; i < 9; i++)
	    sum += arr[i];
        out[offset+z] = sum/9;
    }
	//}
}

int main(int argc, char** argv) 
{
    FILE* fp; 
    BITMAPFILEHEADER bmpHeader;             	/* BMP FILE INFO */
    BITMAPINFOHEADER bmpInfoHeader;     	/* BMP IMAGE INFO */
    ubyte *inimg, *outimg;

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
    
    int elemSize = bmpInfoHeader.biBitCount/8.;
    int stride = bmpInfoHeader.biWidth * elemSize;
    //widthbytes(bits) (((bits)+31)/32*4)
    int imageSize = stride * bmpInfoHeader.biHeight; 

    /* 이미지의 해상도(넓이 × 깊이) */
    printf("Resolution : %d x %d\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
    printf("Bit Count : %d(%d:%d)\n", bmpInfoHeader.biBitCount, elemSize, stride);     /* 픽셀당 비트 수(색상) */
    printf("Image Size : %d\n", imageSize);

    inimg = (ubyte*)malloc(sizeof(ubyte)*imageSize); 
    outimg = (ubyte*)malloc(sizeof(ubyte)*imageSize);
    fread(inimg, sizeof(ubyte), imageSize, fp); 
    
    fclose(fp);

    //allocate and initialize memory on device
    ubyte *d_inimg = NULL, *d_outimg = NULL;
    cudaMalloc(&d_inimg, sizeof(ubyte) * imageSize);
    cudaMalloc(&d_outimg, sizeof(ubyte) * imageSize);
    cudaMemset(d_outimg, 0, sizeof(ubyte) * imageSize);
    
    //copy host rgb data array to device rgb data array
    cudaMemcpy(d_inimg, inimg, sizeof(ubyte) * imageSize, cudaMemcpyHostToDevice);

    //define block and grid dimensions
    const dim3 dimGrid((int)ceil((bmpInfoHeader.biWidth/32)), (int)ceil((bmpInfoHeader.biHeight)/4), 1);
    const dim3 dimBlock(32, 4, elemSize);
    
    //execute cuda kernel
    convertBlur<<<dimGrid, dimBlock>>>(d_inimg, d_outimg, bmpInfoHeader.biHeight, bmpInfoHeader.biWidth, elemSize);

    //copy computed blur data array from device to host
    cudaMemcpy(outimg, d_outimg, sizeof(ubyte) * imageSize, cudaMemcpyDeviceToHost);

    cudaFree(d_outimg);
    cudaFree(d_inimg);

    /***** write bmp *****/ 
    if((fp=fopen(argv[2], "wb"))==NULL) { 
        fprintf(stderr, "Error : Failed to open file...₩n"); 
        return -1;
    }

    /* BITMAPFILEHEADER 구조체의 데이터 */
    fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);

    /* BITMAPINFOHEADER 구조체의 데이터 */
    fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

    //fwrite(inimg, sizeof(ubyte), imageSize, fp); 
    fwrite(outimg, sizeof(ubyte), imageSize, fp);

    fclose(fp); 
    
    free(inimg); 
    free(outimg);
    
    return 0;
}

