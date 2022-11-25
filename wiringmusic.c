#include <wiringPi.h>
#include <softTone.h>

#define SPKR 6
#define TOTAL 38

int notes[] = {
	293, 391, 493, 587, 523, 493, 440, 493, 391, 493, 587,
	783, 783, 783, 880, 693, 659, 698, 440, 554, 698, 880,
	783, 698, 659, 698, 783, 698, 659, 587, 523, 493, 523,
	587, 523, 391, 440
};

int musicPlay()
{
	int i;

	softToneCreate(SPKR);

	for(i=0;i<TOTAL;++i) {
		softToneWrite(SPKR, notes[i]);
		delay(400);
	}

	return 0;

}

int main()
{
	wiringPiSetup();
	musicPlay();
	return 0;
}

