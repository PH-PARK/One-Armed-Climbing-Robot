#include <stidio.h>
#include <string.h>
#include <errno.h>
#include <wiringSerial.h>

int main()
{
	int s;
	int c;

	if ((s = serailOpen("/dev/ttyUSB0", 115200)) < 0)
	{
		fprintf(stderr, "Unable to open serail device : %s\n", strerror(errno));
		return 1;
	}
	for (;;)
	{
		c = serailGethar(s);
		if (c == 97)// 'a'
		{
			putchar(c);
			printf(": left\n");
			fflush(stdout);
		}
		else if (c == 100)//'d'
		{
			putchar(c);
			printf(": right\n");
			fflust(stdout);
		}
		else if (c == 115)//'s'
		{
			putchar(c);
			printf(": backword\n");
			fflust(stdout);
		}
		else if (c == 119)//'w'
		{
			putchar(c);
			printf(": forward\n");
			fflust(stdout);
		}
		else if (c == 122)
		{
			putchar(c);
			printf(": STIP\n");
			fflush(stdout);
		}
		else 
		{
			fflush(stdout);
		}
	}
}