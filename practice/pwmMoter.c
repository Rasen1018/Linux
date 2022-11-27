#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>

void servoMotorControl(int gpio)
{
	pinMode(gpio, OUTPUT);
	softPwmCreate(gpio, 0, 170);

	for(int i =0; i<=170; i++) {
		softPwmWrite(gpio, i);
		delay(100);
	};

	softPwmWrite(gpio, 0);
}

int main(int argc, char** argv)
{
	int gno = atoi(argv[1]);
	wiringPiSetup( );
	servoMotorControl(gno);
	return 0;
}
