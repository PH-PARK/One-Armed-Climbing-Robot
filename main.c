#include <stidio.h>
#include <string.h>
#include <errno.h>
#include <wiringSerial.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <pthread.h>

#define LEFT_PWM 0//physical num11
#define LEFT_DIR 1//physical num12
#define RIGHT_PWM 3//physical num15
#define RIGHT_DIR 4//physical num16


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
	pinMode(RIGHT_PWM, PWM_OUTPUT);
	softPwmCreate(LEFT_PWM, 0, 100);

	pinMode(LEFT_DIR, OUTPUT);
	pinMode(RIGHT_DIR, OUTPUT);
	softPwmCreate(RIGHT_PWM, 0, 100);

	for (;;)
	{	
		if (serialDataAvail(s) != -1)
		{
			c = serailGethar(s);
			if (c == 97)// 'a'
			{
				putchar(c); \
				printf(": left\n");
				fflush(stdout);
				softPwmWrite(LEFT_PWM, 0);
				digitalWrite(LEFT_DIR, HIGH);
				softPwmWrite(RIGHT_PWM, 70);
				digitalWrite(RIGHT_DIR, HIGH);
			}
			else if (c == 100)//'d'
			{
				putchar(c);
				printf(": right\n");
				fflust(stdout);
				softPwmWrite(LEFT_PWM, 70);
				digitalWrite(LEFT_DIR, LOW);
				softPwmWrite(RIGHT_PWM, 0);
				digitalWrite(RIGHT_DIR, LOW);
			}
			else if (c == 115)//'s'
			{
				putchar(c);
				printf(": backword\n");
				fflust(stdout);
				softPwmWrite(LEFT_PWM, 0);
				digitalWrite(LEFT_DIR, HIGH);
				softPwmWrite(RIGHT_PWM, 0);
				digitalWrite(RIGHT_DIR, LOW);
			}
			else if (c == 119)//'w'
			{
				putchar(c);
				printf(": forward\n");
				fflust(stdout);
				softPwmWrite(LEFT_PWM, 70);
				digitalWrite(LEFT_DIR, LOW);
				softPwmWrite(RIGHT_PWM, 70);
				digitalWrite(RIGHT_DIR, HIGH);
			}
			else if (c == 109)//'m'
			{
				putchar(c);
				printf(": change mode\n");
				fflush(stdout);
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
			}//if(c==)
		}//if(serialDataAvail(s)
	}//for (;;)
}//main