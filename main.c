#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <wiringSerial.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <pthread.h>

#define LEFT_PWM 0//physical num11 
#define LEFT_DIR 1//physical num12
#define LEFT_GND 14//physical num14
#define RIGHT_PWM 3//physical num15
#define RIGHT_DIR 4//physical num16
#define RIGHT_GND 20//physical num20

int main()
{
    int s;
    int c;

	uint8_t schedule = 4; //0;
	uint8_t next_schedule =0;
	uint8_t w_continuity = 0;

 //   printf("main\n");

    if(wiringPiSetup()==-1)
    {
		printf("wiringPiSetup failed");
		return 0;
    }
    printf("wiringPi setup\n");
    if((s = serialOpen("/dev/ttyUSB0",115200))<0)
    {
		fprintf(stderr,"Unable to open serial device : %s\n",strerror(errno));
		return 1;
    }

    pinMode(LEFT_PWM,PWM_OUTPUT);
    pinMode(LEFT_DIR,OUTPUT);
    softPwmCreate(LEFT_PWM,0,100);

    pinMode(RIGHT_PWM,PWM_OUTPUT);
    pinMode(RIGHT_DIR,OUTPUT);
    softPwmCreate(RIGHT_PWM,0,100);

    for(;;)
    {
		switch(schedule)
		{
			/*
		case 0:
			puts("schedule 0");
			delay(100);
			schedule=3;
			next_schedule=1;
			break;
		case 1:
			puts("schedule 1");
			delay(100);
			schedule=3;
			next_schedule=2;
			break;
		case 2:
			puts("schedule 2");
			delay(100);
			schedule=3;
			next_schedule=0;
			break;
		case 3://buffer checking
			puts("schedule 3");
			puts("chekcing buffer.");
			if(serialDataAvail(s) == -1)
			{
				puts("serial data error.");
			}
			if(serialDataAvail(s) == 0)
			{
				puts("buffer is empty.");
			}
			else
			{
				c = serialGetchar(s);
				putchar(c);
				if( c == 109)//'m'
				{
					puts("changing mode.");
					schedule=4;
					break;
				}
			}
		//	k++;
			schedule=next_schedule;
			break;
		*/
		case 4:

		//	puts("Now in remote control mode.");

			if(serialDataAvail(s) ==-1)
			{
				puts("Something wrong...");
				delay(100);	   
			}
			else if(serialDataAvail(s) == 0)
			{
				puts("waiting...");
				delay(100);
			}else
			{
				c = serialGetchar(s);

				if(c == 100)//'d'
				{
					putchar(c);
					printf(": right\n");
					w_continuity = 0;
					fflush(stdout);
					softPwmWrite(LEFT_PWM,40);
					digitalWrite(LEFT_DIR,HIGH);
					softPwmWrite(RIGHT_PWM,40);
					digitalWrite(RIGHT_DIR,HIGH);		
				}
				else if(c == 97)//'a'
				{
					putchar(c);
					printf(": left\n");
					w_continuity = 0;
					fflush(stdout);
					softPwmWrite(LEFT_PWM,40);
					digitalWrite(LEFT_DIR,LOW);
					softPwmWrite(RIGHT_PWM,40);
					digitalWrite(RIGHT_DIR,LOW);
				}
				else if(c == 115)//'s'
				{
					putchar(c);
					printf(": backward\n");
					w_continuity = 0;
					fflush(stdout);
					softPwmWrite(LEFT_PWM,40);
					digitalWrite(LEFT_DIR,LOW);
					softPwmWrite(RIGHT_PWM,40);
					digitalWrite(RIGHT_DIR,HIGH);
				}
				else if(c == 119)//'w'
				{
					putchar(c);
					printf(": forward\n");
				
					w_continuity++;
					fflush(stdout);
					if(w_continuity<15)
					{
						softPwmWrite(LEFT_PWM,40);
		 				digitalWrite(LEFT_DIR,HIGH);
						softPwmWrite(RIGHT_PWM,40);
						digitalWrite(RIGHT_DIR,LOW);
						puts("duty 40");
					}
					else if(w_continuity >= 15 && w_continuity<30) 
					{
						softPwmWrite(LEFT_PWM, 60);
						digitalWrite(LEFT_DIR, HIGH);
						softPwmWrite(RIGHT_PWM, 60);
						digitalWrite(RIGHT_DIR, LOW);
						puts("duty 60");
					}
					else if (w_continuity >= 30) 
					{
						softPwmWrite(LEFT_PWM, 70);
						digitalWrite(LEFT_DIR, HIGH);
						softPwmWrite(RIGHT_PWM, 70);
						digitalWrite(RIGHT_DIR, LOW);
						puts("duty 70");
						}

				}
				/*else if(c == 109)//'m'
				{
					putchar(c);
					printf(": change mode\n");
					fflush(stdout);
					softPwmWrite(LEFT_PWM,0);
					softPwmWrite(RIGHT_PWM,0);
					schedule=0;		
				}	*/
				
				/*else if(c == 122)//z
				{
					putchar(c);
					printf(": Go faster\n");
					fflush(stdout);
					softPwmWrite(LEFT_PWM,70);
					softPwmWrite(RIGHT_PWM,70);
				}*/ 
				else if(c == 102) //'f'
				{
					puts(": Neutral");
					fflush(stdout);
					w_continuity = 0;
					softPwmWrite(LEFT_PWM,0);
					softPwmWrite(RIGHT_PWM,0);
					
   				}
				else 
				{
					printf("flush\n");
					fflush(stdout);
				}//if(c==?)
			}//if(serialDataAvail)
			break;
		}//switch
	}//for
	return 0;
}//main

