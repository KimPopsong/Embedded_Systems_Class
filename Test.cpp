#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LCD_D4 2
#define LCD_D5 3
#define LCD_D6 1
#define LCD_D7 4
#define LCD_RS 7
#define LCD_EN 0

#define BT0 23
#define BT1 12
#define BT2 13
#define BT3 10
#define BT4 14
#define BT5 21
#define BT6 11
#define BT7 22
#define BT8 26
#define BT9 27
#define EQL 24
#define PLUS 5
#define MINUS 6

int sum = 0, temp = 0;
int count = 0;
int tmpcnt = 0;
char tmp[40] = { 0 };
int opflag = 1;
char frontop = 0;
int minusflag = 0;

void Print(char c)
{
	printf("%c", c);
}

void Input(char c)
{
	int i;
	char Inval[20] = "InvalidOperation";

	if (c == '+' || c == '-') //when input is + or -
	{
		if ((minusflag == 0) && (c == '-')) //start as -
		{
			opflag = 1;
			minusflag = 1;
			frontop = '-';
		}

		else if (opflag) //start as + or double operation
		{
			printf("Error\n");

			return;
		}

		else //Normal
		{
			opflag = 1;
			minusflag = 1;

			for (i = 0; i < tmpcnt; i++) //make string to int
			{
				temp += (tmp[i] - 48) * pow(10, (tmpcnt - i - 1));
			}

			if (frontop == '+')
			{
				sum += temp;
				temp = 0;
			}

			else if (frontop == '-')
			{
				sum -= temp;
				temp = 0;
			}

			else
			{
				sum += temp;
				temp = 0;
			}

			tmpcnt = 0;

			frontop = c;
		}

		Print(c);
	}

	else if (c == '=') //=
	{
		if (opflag) //start as = or double operation
		{
			printf("Error\n");
			return;
		}

		else //Normal
		{
			Print(c);

			opflag = 1;

			for (i = 0; i < tmpcnt; i++) //make string to int
			{
				temp += (tmp[i] - 48) * pow(10, (tmpcnt - i - 1));
			}

			if (frontop == '+')
			{
				sum += temp;
				temp = 0;
			}

			else if (frontop == '-')
			{
				sum -= temp;
				temp = 0;
			}

			else //no front operation
			{
				printf("Error\n");

				return;
			}

			tmpcnt = 0;

			if (sum < 0)
			{
				Print('-');
				sum *= -1;
			}

			while (sum != 0)
			{
				tmp[tmpcnt] = (sum % 10) + 48;
				tmpcnt++;
				sum /= 10;
			}

			tmpcnt--;

			for (; tmpcnt >= 0; tmpcnt--)
			{
				Print(tmp[tmpcnt]);
			}

			count = 0;
		}
	}

	else //number
	{
		Print(c);
		tmp[tmpcnt] = c;
		tmpcnt++;
		opflag = 0;
		minusflag = 1;
	}
}

int main(int argc, char** argv)
{

	while (1)
	{
		char c;

		scanf(" %c", &c);
		if (c == '0')
		{
			Input('0');
		}

		else if (c == '1')
		{
			Input('1');
		}

		else if (c == '2')
		{
			Input('2');
		}

		else if (c == '3')
		{
			Input('3');
		}

		else if (c == '4')
		{
			Input('4');
		}

		else if (c == '5')
		{
			Input('5');
		}

		else if (c == '6')
		{
			Input('6');
		}

		else if (c == '7')
		{
			Input('7');
		}

		else if (c == '8')
		{
			Input('8');
		}

		else if (c == '9')
		{
			Input('9');
		}

		else if (c == '=')
		{
			Input('=');
		}

		else if (c == '+')
		{
			Input('+');	
		}

		else if (c == '-')
		{
			Input('-');
		}
	}
}