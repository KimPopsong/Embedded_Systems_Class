#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <wiringPi.h>

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
char tmp[50] = { 0 };
int opflag = 1;
int minusflag = 1;
char frontop = 0;

void write4bits(unsigned char command)
{
	digitalWrite(LCD_D4, (command & 1));
	command >>= 1;

	digitalWrite(LCD_D5, (command & 1));
	command >>= 1;

	digitalWrite(LCD_D6, (command & 1));
	command >>= 1;

	digitalWrite(LCD_D7, (command & 1));
	digitalWrite(LCD_EN, 1);
	delayMicroseconds(10);
	digitalWrite(LCD_EN, 0);
	delayMicroseconds(10);
}

void sendDataCmd4(unsigned char data)
{
	write4bits(((data >> 4) & 0x0f));
	write4bits((data & 0x0f));
	delayMicroseconds(100);
}

void putCmd4(unsigned char cmd)
{
	digitalWrite(LCD_RS, 0);
	sendDataCmd4(cmd);
}

void putChar(char c) 
{

	digitalWrite(LCD_RS, 1);

	sendDataCmd4(c);
}

void initialize_textlcd() //CLCD 초기화
{	
	sum = 0;
	temp = 0;
	count = 0;
	tmpcnt = 0;
	tmp[40] = 0;
	opflag = 1;
	frontop = 0;
	minusflag = 1;

	pinMode(LCD_RS, OUTPUT);
	pinMode(LCD_EN, OUTPUT);
	pinMode(LCD_D4, OUTPUT);
	pinMode(LCD_D5, OUTPUT);
	pinMode(LCD_D6, OUTPUT);
	pinMode(LCD_D7, OUTPUT);

	digitalWrite(LCD_RS, 0);
	digitalWrite(LCD_EN, 0);
	digitalWrite(LCD_D4, 0);
	digitalWrite(LCD_D5, 0);
	digitalWrite(LCD_D6, 0);
	digitalWrite(LCD_D7, 0);

	delay(35);

	pinMode(BT0, INPUT);
	pinMode(BT1, INPUT);
	pinMode(BT2, INPUT);
	pinMode(BT3, INPUT);
	pinMode(BT4, INPUT);
	pinMode(BT5, INPUT);
	pinMode(BT6, INPUT);
	pinMode(BT7, INPUT);
	pinMode(BT8, INPUT);
	pinMode(BT9, INPUT);
	pinMode(EQL, INPUT);
	pinMode(PLUS, INPUT);
	pinMode(MINUS, INPUT);

	pullUpDnControl(BT0, PUD_DOWN);
	pullUpDnControl(BT1, PUD_DOWN);
	pullUpDnControl(BT2, PUD_DOWN);
	pullUpDnControl(BT3, PUD_DOWN);
	pullUpDnControl(BT4, PUD_DOWN);
	pullUpDnControl(BT5, PUD_DOWN);
	pullUpDnControl(BT6, PUD_DOWN);
	pullUpDnControl(BT7, PUD_DOWN);
	pullUpDnControl(BT8, PUD_DOWN);
	pullUpDnControl(BT9, PUD_DOWN);
	pullUpDnControl(EQL, PUD_DOWN);
	pullUpDnControl(PLUS, PUD_UP);
	pullUpDnControl(MINUS, PUD_UP);

	putCmd4(0x28);  // 4비트 2줄
	putCmd4(0x28);
	putCmd4(0x28);
	putCmd4(0x0e); // 디스플레이온 커서 온 
	putCmd4(0x02); // 커서 홈 
	delay(2);
	putCmd4(0x01); // 표시 클리어 
	delay(2);
}

void Print(char c)
{
	char ovf[10] = "Overflow";
	int i;

	if (count == 0) //clear
	{
		initialize_textlcd();
		putChar(c);
		count++;
	}

	else if (count == 16) //change line
	{
		putCmd4(0xC0);
		putChar(c);
		count++;
	}

	else if (count <= 31) //write
	{
		putChar(c);
		count++;
	}

	else //overflow
	{
		initialize_textlcd();

		for (i = 0; i < strlen(ovf); i++)
			putChar(ovf[i]);

		delay(2000);

		initialize_textlcd();
	}
}

void Input(char c)
{
	int i;
	char Inval[20] = "InvalidOperation";

	if (c == '+' || c == '-') //when input is + or -
	{
		if ((c == '-') && (minusflag == 1)) //start as -
		{
			minusflag = 0;
			frontop = '-';
			putChar('-');
			count++;
			return;
		}

		else if (opflag) //start as + or double operation
		{
			initialize_textlcd();

			for (i = 0; i < strlen(Inval); i++) //print Error Message
				Print(Inval[i]);

			delay(2000);

			initialize_textlcd();

			return;
		}

		else //Normal
		{
			opflag = 1;
			minusflag = 0;

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
			initialize_textlcd();

			for (i = 0; i < strlen(Inval); i++)
				Print(Inval[i]);

			delay(2000);

			initialize_textlcd();

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
				initialize_textlcd();

				for (i = 0; i < strlen(Inval); i++)
					Print(Inval[i]);

				delay(2000);

				initialize_textlcd();

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
		minusflag = 0;
	}
}

int main(int argc, char** argv) 
{
	wiringPiSetup();

	initialize_textlcd();

	while (1)
	{
		if (digitalRead(BT0))
		{
			Input('0');
			delay(300);
		}

		else if (digitalRead(BT1))
		{
			Input('1');
			delay(300);
		}

		else if (digitalRead(BT2))
		{
			Input('2');
			delay(300);
		}

		else if (digitalRead(BT3))
		{
			Input('3');
			delay(300);
		}

		else if (digitalRead(BT4))
		{
			Input('4');
			delay(300);
		}

		else if (digitalRead(BT5))
		{
			Input('5');
			delay(300);
		}

		else if (digitalRead(BT6))
		{
			Input('6');
			delay(300);
		}

		else if (digitalRead(BT7))
		{
			Input('7');
			delay(300);
		}

		else if (digitalRead(BT8))
		{
			Input('8');
			delay(300);
		}

		else if (digitalRead(BT9))
		{
			Input('9');
			delay(300);
		}

		else if (digitalRead(EQL))
		{
			Input('=');
			delay(300);
		}

		else if (!digitalRead(PLUS))
		{
			Input('+');
			delay(300);
		}

		else if (!digitalRead(MINUS))
		{
			Input('-');
			delay(300);
		}
	}
}