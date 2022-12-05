#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bmpHeader.h"

#define BYTE	unsigned char
#define M_PI	3.141592654
#define BASE	15

#define widthbytes(bits)   (((bits)+31)/32*4)

void main(int argc, char** argv)
{
	FILE *fp;
	RGBQUAD *palrgb;
	BITMAPFILEHEADER bmpHeader;
	BITMAPINFOHEADER infoHeader;

	char input[128], output[128];
 
	int i, j, size, index;

	unsigned char *inimg;
	unsigned char *outimg;
 
	strcpy(input, argv[1]);
	strcpy(output, argv[2]);

	if((fp=fopen(argv[1], "rb")) == NULL) {
		fprintf(stderr, "Error : Failed to open file...\n");
		return -1;
	}

	fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	if(infoHeader.biBitCount ! =24) {
	   perror("This image file doesn't supports 24bit color\n");
	   fclose(fp);
	   return -1;
	}

	palrgb = (RGBQUAD*)malloc(sizeof(RGBQUAD)*infoHeader.biClrUsed);
	fread(palrgb, sizeof(RGBQUAD), infoHeader.biClrUsed, fp);
	  
	imageSize = widthBytes(infoHeader.biBitCount * infoHeader.biWidth)*infoHeader.biHeight;

	inimg = (BYTE*)malloc(sizeof(BYTE)*imagesize);
	outimg = (BYTE*)malloc(sizeof(BYTE)*imagesize);
	fread(inimg, sizeof(BYTE), imagesize, fp);

	fclose(fp);
#if 0
	for(i = 0; i < height; i++) {
		index = i * size; 
		for(j = 0 ; j < width; j++) {
			outimg[index+3*j+0] = inimg[size*i-3*j+0];
			outimg[index+3*j+1] = inimg[size*i-3*j+1];
			outimg[index+3*j+2] = inimg[size*i-3*j+2];
		};
	};
#else
	int pos = 0;
	for(x = 0; x < infoHeader.biWidth*bmpInfoHeader.biHeight/8; x++) {
		for(int i = 7; i >= 0; --i) {
			int num = inimg[x];
			int res = num >> i & 1;
			outimg[pos++]=palrgb[res].rgbBlue;
			outimg[pos++]=palrgb[res].rgbGreen;
			outimg[pos++]=palrgb[res].rgbRed;
		}
	}
#endif
	offset += 256*sizeof(RGBQUAD); 

	if((fp = fopen(output, "wb")) == NULL) {
		fprintf(stderr, "Error : Failed to open file...\n");
		exit(EXIT_FAILURE);
	}

	infoHeader.biBitCount = 24;
	infoHeader.SizeImage = infoHeader.biWidth*infoHeader.biHeight*3;
	infoHeader.biClrUsed = 0;

	bmpHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + infoHeader.SizeImage;
	bmpHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 

	fwrite(&bmpHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	fwrite(outimg, sizeof(unsigned char), infoHeader.SizeImage, fp);
	
	free(inimg);
	free(outimg);

	fclose(fp);

