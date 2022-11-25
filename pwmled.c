#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>

void ledPwmControl(int gpio)
{
	pinMode(gpio, OUTPUT);
	softPwmCreate(gpio, 0, 255);

	for(int i = 0; i<10000; i++) {
		softPwmWrite(gpio, i&255);
		delay(5);
	};
	softPwmWrite(gpio, 0);
}
