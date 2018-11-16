#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <wiringSerial.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <pthread.h>

#define __DEBUG__ //Show mesaages 
#define DEVELOP // Enable develop mode
#define SD // Enable self driving mode
#define RC // Enable remote control mode

#define LEFT_PWM 0//physical num11 
#define LEFT_DIR 1//physical num12
#define LEFT_GND 14//physical num14
#define RIGHT_PWM 3//physical num15
#define RIGHT_DIR 4//physical num16
#define RIGHT_GND 20//physical num20

#define MAX_DUTY 80
#define MIN_DUTY -MAX_DUTY
#define RAGULAR_DUTY 40

void SelfDrivingMode(void);
void RemoteMode(void);
void DevelopMode(void);
void CheckBuffer(void);
void DoOrder(char);

enum {SELF_DRIVING=1, REMOTE_CONTROL, DEVELOP_MODE};
int schedule;
int s = 0;

int main()
{
    int c=0;

#ifdef __DEBUG__
	puts("main");
#endif

    if(wiringPiSetup()==-1)
    {
		printf("wiringPiSetup failed");
		return 0;
    }
#ifdef __DEBUG__
	puts("WiringPi Setup");
#endif

#ifdef RC
    if((s = serialOpen("/dev/ttyUSB0",115200))<0)
    {
		fprintf(stderr,"Unable to open serial device : %s\n",strerror(errno));
		return 1;
    }
#endif

#ifdef __DEBUG__
	puts("Serial Open");
#endif

    pinMode(LEFT_PWM,PWM_OUTPUT);
    pinMode(LEFT_DIR,OUTPUT);
    softPwmCreate(LEFT_PWM,0,100);

    pinMode(RIGHT_PWM,PWM_OUTPUT);
    pinMode(RIGHT_DIR,OUTPUT);
    softPwmCreate(RIGHT_PWM,0,100);

#ifdef __DEBUG__
	puts("Pin configuration");
#endif

#ifdef RC
	schedule = REMOTE_CONTROL; // default mode is RMOTE_CONTROL mode
#endif

#ifdef DEVELOP
	puts("*******************");
	puts("Selct Mode");
	puts("1. Self Driving Mode");
	puts("2. Remote Control Mode");
	puts("3. Develop Mode");
	fputs(" >> ",stdout);
	scanf("%d", &schedule);
#endif

    while(1)
    {
		switch(schedule)
		{
		case SELF_DRIVING:
			SelfDrivingMode();
			break;

		case REMOTE_CONTROL:
			RemoteMode();
			break;
		
		case DEVELOP_MODE:
			DevelopMode();
			break;
		}
	}
	return 0;
}

void SelfDrivingMode()
{
#ifdef __DEBUG__
	puts("Self Driving Mode.");
#endif

	while (1)
	{

		CheckBuffer();
		if (schedule != SELF_DRIVING)
		{
			return;
		}
	}
}

void RemoteMode()
{
	char c = 0;
#ifdef __DEBUG__
	puts("Check Buffer");
#endif
	while (1)
	{
		if (serialDataAvail(s) == -1)
		{
#ifdef __DEBUG__
			puts("Data is not available.");
#endif
			delay(100);
		}
		else if (serialDataAvail(s) == 0)
		{
#ifdef __DEBUG__
			puts("Data not exist");
#endif
			delay(100);
		}
		else
		{
#ifdef __DEBUG__
			puts("Data exist.");
#endif
			c = serialGetchar(s);
#ifdef __DEBUG__
			printf("We got '%c'\n", c);
#endif
			DoOrder(c);
			fflush(stdout);
		}//if(serialDataAvail)
	}
}

void DevelopMode()
{
	char ch = 0;
	int duty = 0;

	puts("*************************");
	puts("Develop Mode");
	printf("MAX_DUTY : %d\n", MAX_DUTY);
	printf("MIN_DUTY : %d\n", MIN_DUTY);
	puts("*************************");

	while (1)
	{
		puts("*******************");
		printf("Duty : %d\n", duty);
		puts("1. Change Duty");
		puts("2. Change Mode");
		puts("*******************");
		fputs(" >> ", stdout);
		scanf("%c", &ch);
		if (ch == 1)
		{
			puts("*******************");
			puts("Duty : ");
			fputs(" >> ", stdout);
			scanf("%d", &duty);
			puts("*******************");

			if (duty < MIN_DUTY)
			{
				puts("duty adjustment");
				duty = MIN_DUTY;
			}
			if (duty > MAX_DUTY)
			{
				puts("duty adjustment");
				duty = MAX_DUTY;
			}

			if (duty < 0)
			{
				softPwmWrite(LEFT_PWM, -duty);
				digitalWrite(LEFT_DIR, LOW);
				softPwmWrite(RIGHT_PWM, -duty);
				digitalWrite(RIGHT_DIR, LOW);
			}
			else
			{
				softPwmWrite(LEFT_PWM, duty);
				digitalWrite(LEFT_DIR, HIGH);
				softPwmWrite(RIGHT_PWM, duty);
				digitalWrite(RIGHT_DIR, HIGH);
			}

		}
		else if (ch == 2)
		{
			puts("*******************");
			puts("Exit Develop Mode");
			puts("*******************");
			schedule = SELF_DRIVING;

			return;
		}
	}
}	

void CheckBuffer()
{
	char c = 0;

#ifdef __DEBUG__
	puts("Check Buffer");
	puts("Send 'm' if you want to chage mode to Rmote Control Mode.");
#endif
	if (serialDataAvail(s) == -1)
	{
		puts("serial data error.");
	}
	if (serialDataAvail(s) == 0)
	{
		puts("Buffer is Empty.");
	}
	else
	{
		c = serialGetchar(s);
#ifdef __DEBUG__
		printf("We got : '%c'\n",c);
#endif
		if (c == 'm')
		{
			puts("change mode to Remote Control Mode.");
			schedule = REMOTE_CONTROL;
			return;
		}
	}
}

void DoOrder(char c)
{
	
	if (c == 'd')
	{
		printf(": right\n");
		softPwmWrite(LEFT_PWM, 40);
		digitalWrite(LEFT_DIR, HIGH);
		softPwmWrite(RIGHT_PWM, 40);
		digitalWrite(RIGHT_DIR, HIGH);
	}
	else if (c == 'a')
	{
		printf(": left\n");
		softPwmWrite(LEFT_PWM, 40);
		digitalWrite(LEFT_DIR, LOW);
		softPwmWrite(RIGHT_PWM, 40);
		digitalWrite(RIGHT_DIR, LOW);
	}
	else if (c == 's')
	{
		printf(": backward\n");
		softPwmWrite(LEFT_PWM, 40);
		digitalWrite(LEFT_DIR, LOW);
		softPwmWrite(RIGHT_PWM, 40);
		digitalWrite(RIGHT_DIR, HIGH);
	}
	else if (c == 'w')
	{
		printf(": forward\n");
		softPwmWrite(LEFT_PWM, 40);
		digitalWrite(LEFT_DIR, HIGH);
		softPwmWrite(RIGHT_PWM, 40);
		digitalWrite(RIGHT_DIR, LOW);
	}	
	else if(c == 'm')
	{
		printf(": change mode\n");
		softPwmWrite(LEFT_PWM,0);
		softPwmWrite(RIGHT_PWM,0);
		schedule = DEVELOP_MODE;
	}	

	else if (c == 'f')
	{
		puts(": Neutral");
		fflush(stdout);
		softPwmWrite(LEFT_PWM, 0);
		softPwmWrite(RIGHT_PWM, 0);
	}
	else
	{
		puts("Undefned OP code");
	}
}