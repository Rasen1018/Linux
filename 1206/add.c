#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include "bmpHeader.h"

#define BYTE unsigned char

#define widthbytes(bits) (((bits)+31)/32*4)

inline unsigned char clip(int value, int min, int max);
unsigned char clip(int value, int min, int max)
{
    return(value > max? max : value < min? min : value);
}

int main(int argc, char** argv) {
	FILE* fp; 
	RGBQUAD *palrgb;
	BITMAPFILEHEADER bmpHeader;
	BITMAPINFOHEADER infoHeader;
	
	char input[128], output[128];
	
	int x, y, z, size; 
	float srcX, srcY;
	int index; 
	float r,g,b,gray;
	int graysize, imagesize; 
	int index2;
	
	unsigned char *grayimg, *inimg, *outimg;
	
	/* usage a.out in.bmp out.bmp */
	strcpy(input, argv[1]); 
	strcpy(output, argv[2]);
	
	
	if((fp=fopen(input, "rb")) == NULL) { 
		fprintf(stderr, "Error : Failed to open file...₩n"); 
		return -1;
	}
	fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	if(infoHeader.biBitCount != 24) {
		perror("This image file doesn't supports 24bit color\n");
		fclose(fp);
		return -1;
	}

	int elemSize = infoHeader.biBitCount/8;
	size=infoHeader.biWidth * elemSize; 
	graysize = widthbytes(8 * infoHeader.biWidth);
	
		imagesize=infoHeader.biHeight*size;
	
	inimg=(BYTE*)malloc(sizeof(BYTE)*imagesize); 
	outimg=(BYTE*)malloc(sizeof(BYTE)*imagesize); 
	fread(inimg, sizeof(BYTE), imagesize, fp); 
	
	fclose(fp);
#if 1	
	for(y=0; y < infoHeader.biHeight; y++) { 
		for(x=0; x < size; x+=elemSize) {
			for(z=0; z < elemSize; z++) {
				int rgb = inimg[x+y*size+z]; 
				rgb += rand()%256;
				outimg[x+y*size+z]= clip(rgb, 0, 255);
			}
		};
	 };
#else

#endif
	
	//size=widthbytes(infoHeader.biWidth * elemSize); 
	//imagesize=infoHeader.biHeight*size; 
	
	if((fp=fopen(output, "wb"))==NULL) { 
		fprintf(stderr, "Error : Failed to open file...₩n"); 
		return -1;
	}
	
	fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(outimg, sizeof(unsigned char), imagesize, fp);
	
	free(inimg); 
	free(outimg);
	
	fclose(fp); 
	
	return 0;
}
