#include <stdio.h>
#include <wiringPi.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define LED0 10
#define LED1 11
#define LED2 13
#define LED3 14
#define LED4 21
#define LED5 12
#define LED6 26

#define DELAY delay(500)

void On(int n0, int n1, int n2, int n3, int n4, int n5, int n6)
{
	if (n0 == 1)
		digitalWrite(LED0, HIGH);

	if (n1 == 1)
		digitalWrite(LED1, HIGH);

	if (n2 == 1)
		digitalWrite(LED2, HIGH);

	if (n3 == 1)
		digitalWrite(LED3, HIGH);

	if (n4 == 1)
		digitalWrite(LED4, HIGH);

	if (n5 == 1)
		digitalWrite(LED5, HIGH);

	if (n6 == 1)
		digitalWrite(LED6, HIGH);

	return;
}

void Off()
{
	digitalWrite(LED0, LOW);
	digitalWrite(LED1, LOW);
	digitalWrite(LED2, LOW);
	digitalWrite(LED3, LOW);
	digitalWrite(LED4, LOW);
	digitalWrite(LED5, LOW);
	digitalWrite(LED6, LOW);
}

void Choose(int i)
{
	if (i == 0)
	{
		On(1, 1, 1, 1, 1, 1, 0);
		DELAY;

		Off();
		DELAY;
	}

	if (i == 1)
	{
		On(0, 1, 1, 0, 0, 0, 0);
		DELAY;

		Off();
		DELAY;
	}

	if (i == 2)
	{
		On(1, 1, 0, 1, 1, 0, 1);
		DELAY;

		Off();
		DELAY;
	}

	if (i == 3)
	{
		On(1, 1, 1, 1, 0, 0, 1);
		DELAY;

		Off();
		DELAY;
	}

	if (i == 4)
	{
		On(0, 1, 1, 0, 0, 1, 1);
		DELAY;

		Off();
		DELAY;
	}

	if (i == 5)
	{
		On(1, 0, 1, 1, 0, 1, 1);
		DELAY;

		Off();
		DELAY;
	}

	if (i == 6)
	{
		On(1, 0, 1, 1, 1, 1, 1);
		DELAY;

		Off();
		DELAY;
	}

	if (i == 7)
	{
		On(1, 1, 1, 0, 0, 1, 0);
		DELAY;

		Off();
		DELAY;
	}

	if (i == 8)
	{
		On(1, 1, 1, 1, 1, 1, 1);
		DELAY;

		Off();
		DELAY;
	}

	if (i == 9)
	{
		On(1, 1, 1, 1, 0, 1, 1);
		DELAY;

		Off();
		DELAY;
	}

	if (i == 10) //a
	{
		On(1, 1, 1, 0, 1, 1, 1);
		DELAY;

		Off();
		DELAY;
	}

	if (i == 11) //b
	{
		On(0, 0, 1, 1, 1, 1, 1);
		DELAY;

		Off();
		DELAY;
	}

	if (i == 12) //c
	{
		On(1, 0, 0, 1, 1, 1, 0);
		DELAY;

		Off();
		DELAY;
	}

	if (i == 13) //d
	{
		On(0, 1, 1, 1, 1, 0, 1);
		DELAY;

		Off();
		DELAY;
	}

	if (i == 14) //e
	{
		On(1, 0, 0, 1, 1, 1, 1);
		DELAY;

		Off();
		DELAY;
	}

	if (i == 15) //f
	{
		On(1, 0, 0, 0, 1, 1, 1);
		DELAY;

		Off();
		DELAY;
	}
}

int main(int argc, char* argv[])
{
	int i;
	int rnd;
	int n[7] = { 0 };
	int random[16] = { 0 };
	char str[10] = { 0 };

	srand(time(NULL));

	wiringPiSetup();

	pinMode(LED0, OUTPUT);
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED3, OUTPUT);
	pinMode(LED4, OUTPUT);
	pinMode(LED5, OUTPUT);
	pinMode(LED6, OUTPUT);

	if (argc <= 1)
		printf("Set One or More Argument!\n");

	else if(argc > 3)
		printf("Too Many Arguments!\n");

	else if (1 == atoi(argv[1]))
	{
		for (i = 0; i < 16; i++)
		{
			Choose(i);
		}

		for (i = 0; i < 16; i++)
		{
			rnd = rand() % 16;

			if (random[rnd] == 1)
				i--;

			else
			{
				Choose(rnd);
				random[rnd] = 1;
			}
		}
	}

	else if (2 == atoi(argv[1]))
	{
		Off();

		if (argc <= 2)
		{
			printf("Need Two Argument!\n");
			return 0;
		}

		strcpy(str, argv[2]);

		if ((str[0] != 48) || ((str[1] != 120) && (str[1] != 88)))
		{
			printf("Second Argument Must Be Hex Form(like 0x00)\n");
			return 0;
		}

		else if ((strlen(str) == 4) && (str[2] >= 56))
		{
			printf("A number higher than 0x7F is not allowed\n");
			return 0;
		}

		else if (strlen(str) == 3)
		{
			rnd = str[2] - 48;

			if (rnd > 9)
				rnd -= 7;

			for (i = 0; i < 4; i++)
			{
				n[i] = rnd % 2;
				rnd /= 2;
			}

			On(n[0], n[1], n[2], n[3], n[4], n[5], n[6]);
		}

		else if (strlen(str) == 4)
		{
			rnd = str[2] - 48;

			if (rnd > 9)
				rnd -= 7;

			for (i = 4; i < 7; i++)
			{
				n[i] = rnd % 2;
				rnd /= 2;
			}

			rnd = str[3] - 48;

			if (rnd > 9)
				rnd -= 7;

			for (i = 0; i < 4; i++)
			{
				n[i] = rnd % 2;
				rnd /= 2;
			}

			On(n[0], n[1], n[2], n[3], n[4], n[5], n[6]);
		}

		else
		{
			printf("Second Argument Must Be Two Digits.\n");
			return 0;
		}
	}

	else
		printf("Only 1 and 2 are available!\n");

	return 0;
}
