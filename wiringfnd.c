#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
int gpiopins[7] = {28, 27, 22, 23, 21, 26, 5};
int fndControl(int num)
{
	int i;
	int number[10][7] = { {1,1,1,1,1,1,0},
							{0,1,1,0,0,0,0},
							{1,1,0,1,1,0,1},
							{1,1,1,1,0,0,1},
							{0,1,1,0,0,1,1},
							{1,0,1,1,0,1,1},
							{0,0,1,1,1,1,1},
							{1,1,1,0,0,0,0},
							{1,1,1,1,1,1,1},
							{1,1,1,1,0,1,1}};
	for (i=0; i<7;i++) {
		pinMode(gpiopins[i], OUTPUT);
	}

	for (i=0;i<7;i++) {
		digitalWrite(gpiopins[i], number[num][i]? HIGH:LOW);
	}

	for(int i=0; i<7; i++) {
		digitalWrite(gpiopins[i], HIGH);
	}

	return 0;
}

int main(int argc, char **argv)
{
	int no;

	if (argc < 2) {
		printf("Usage : %s NO\n", argv[0]);
		return -1;
	}
	no = atoi(argv[1]);
	wiringPiSetup();
	while (1) {
		no = no%10;
		fndControl(no);
		delay(100);
		no++; 
	}
	return 0;
}
