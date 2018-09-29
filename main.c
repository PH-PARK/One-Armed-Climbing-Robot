#include <stidio.h>
#include <string.h>
#include <errno.h>
#include <wiringSerial.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <pthread.h>

#define LEFT_PWM 11
#define LEFT_DIR 12

#define RIGHT_PWM 15
#define RIGHT_DIR 16


int main()
{
	int s;
	int c;

	if (wiringPiSetup() == -1)
	{
		printf("wiringPiSetup failed");
		return 0;
	}

	if ((s = serailOpen("/dev/ttyUSB0", 115200)) < 0)
	{
		fprintf(stderr, "Unable to open serail device : %s\n", strerror(errno));
		return 1;
	}

	pinMode(LEFT_PWM, PWM_OUTPUT);
	softPwmCreate(LEFT_PWM, 0, 100);
	pinMode(RIGHT_PWM, PWM_OUTPUT);
	softPwmCreate(RIGHT_PWM, 0, 100);

	for (;;)
	{
		c = serailGethar(s);
		if (c == 97)// 'a'
		{
			putchar(c);
			printf(": left\n");
			fflush(stdout);
			softPwmWrite(LEFT_PWM, 0);
			softPwmWrite(RIGHT_PWM, 70);
		}
		else if (c == 100)//'d'
		{
			putchar(c);
			printf(": right\n");
			fflust(stdout);
			softPwmWrite(LEFT_PWM, 70);
			softPwmWrite(RIGHT_PWM, 0);
		}
		else if (c == 115)//'s'
		{
			putchar(c);
			printf(": backword\n");
			fflust(stdout);
			softPwmWrite(LEFT_PWM, 0);
			softPwmWrite(RIGHT_PWM, 0);
		}
		else if (c == 119)//'w'
		{
			putchar(c);
			printf(": forward\n");
			fflust(stdout);
			softPwmWrite(LEFT_PWM, 70);
			softPwmWrite(RIGHT_PWM, 70);
		}
		else if (c == 122)//'z'
		{
			putchar(c);
			printf(": STOP\n");
			fflush(stdout);
			softPwmWrite(LEFT_PWM, 0);
			softPwmWrite(RIGHT_PWM, 0);
		}
		else 
		{
			fflush(stdout);
		}
	}
}