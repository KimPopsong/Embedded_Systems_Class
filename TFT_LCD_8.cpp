#include <stdio.h>
#include <stdlib.h> /*for exit*/
#include <time.h>
#include <unistd.h>/*for open/close..*/
#include <fcntl.h> /*for O_RDWR*/
#include <sys/ioctl.h> /*for ioctl*/
#include <linux/fb.h> /*for fb_var_screeninfo, FBIOGET_VSCREENINFO*/
#include <sys/types.h>
#include <wiringPi.h>

#define FBDEVFILE "/dev/fb2"

#define LEFT 1
#define DEL 4
#define RIGHT 5
#define DOTQZ 24
#define ABC 23
#define DEF 27
#define GHI 22
#define JKL 21
#define MNO 26
#define PRS 7
#define TUV 6
#define WXY 11

int writealpha = 0;
int writeline = 0;
int cursorNow = 0;
int cursorMax = 0;
int dupleFlag = 0; //a -> b -> c -> a... �ݺ��� ���� ����
time_t cursorGlobalTime;
char str[90] = { 0 };

int a[] = {
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,
1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

int cursor[] = {
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

void Clear() //ȭ�� �ʱ�ȭ
{
	int fbfd;
	int ret;
	int i, j;
	unsigned short black = 0;

	struct fb_var_screeninfo fbvar;
	int offset;//frame buffer driver file point�� ��´�.

	fbfd = open(FBDEVFILE, O_RDWR);

	if (fbfd < 0)
	{
		perror("fbdevopen");
		exit(1);
	}

	ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &fbvar);

	if (ret < 0)
	{
		perror("fbdevioctl");
		exit(1);
	}

	if (fbvar.bits_per_pixel != 16)
	{
		perror(" bpp is not 16\n ");
		exit(1);
	}

	for (i = 0; i < fbvar.yres; i++)
	{
		for (j = 0; j < fbvar.xres; j++)
		{
			offset = i * fbvar.xres * (16 / 8) + j * (16 / 8);

			if (lseek(fbfd, offset, SEEK_SET) < 0) {}

			write(fbfd, &black, 2); /*write 2bytes (16bit)*/
		}
	}

	close(fbfd);
	return;
}

void initialize_textlcd() //�ʱ�ȭ
{
	pinMode(LEFT, INPUT);
	pinMode(DEL, INPUT);
	pinMode(RIGHT, INPUT);
	pinMode(DOTQZ, INPUT);
	pinMode(ABC, INPUT);
	pinMode(DEF, INPUT);
	pinMode(GHI, INPUT);
	pinMode(JKL, INPUT);
	pinMode(MNO, INPUT);
	pinMode(PRS, INPUT);
	pinMode(TUV, INPUT);
	pinMode(WXY, INPUT);

	pullUpDnControl(LEFT, PUD_UP);
	pullUpDnControl(DEL, PUD_UP);
	pullUpDnControl(RIGHT, PUD_UP);
	pullUpDnControl(DOTQZ, PUD_DOWN);
	pullUpDnControl(ABC, PUD_DOWN);
	pullUpDnControl(DEF, PUD_DOWN);
	pullUpDnControl(GHI, PUD_DOWN);
	pullUpDnControl(JKL, PUD_DOWN);
	pullUpDnControl(MNO, PUD_DOWN);
	pullUpDnControl(PRS, PUD_DOWN);
	pullUpDnControl(TUV, PUD_DOWN);
	pullUpDnControl(WXY, PUD_DOWN);

	writealpha = 0;
	writeline = 0;
	Clear();

	return;
}

void Write(int alpha[])
{
	int fbfd;
	int ret;
	int i, j, count = 0;
	unsigned short white = 65535;

	struct fb_var_screeninfo fbvar;
	int offset;//frame buffer driver file point�� ��´�.

	fbfd = open(FBDEVFILE, O_RDWR);

	if (fbfd < 0)
	{
		perror("fbdevopen");
		exit(1);
	}
	// Ŀ�ηκ��� LCD�� ���� ������ fbvar�� �����Ѵ�. 

	ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &fbvar);

	if (ret < 0)
	{
		perror("fbdevioctl");
		exit(1);
	}

	if (fbvar.bits_per_pixel != 16)
	{ //bpp�� 16bit���� check
		perror(" bpp is not 16\n ");
		exit(1);
	}

	for (count = 0, i = 0; count < 576; count++, i++) //���� �ϳ� ���
	{
		if (alpha[count] == 0)
		{
			continue;
		}

		else
		{
			if (i >= 24)
				i %= 24;

			j = (count / 24);

			offset = (j + writeline) * fbvar.xres * (16 / 8) + (i + writealpha) * (16 / 8);

			if (lseek(fbfd, offset, SEEK_SET) < 0) {}

			write(fbfd, &white, 2); /*write 2bytes (16bit)*/
		}
	}

	close(fbfd);
	return;
}

void WriteString() //���ݱ��� �Է� �� ���ڵ��� str�� �����ϰ�, WRITE�Լ��� �ѱ��ھ� ����.
{
	int i;

	if (cursorMax >= 81) //ȭ�鿡 80���� ���ڸ� ����ٸ�
	{
		printf("Too many Alphabets!!\n");
		return;
	}

	else
	{
		Clear();

		writeline = 0;
		writealpha = 0;

		for (i = 0; i < cursorMax; i++)
		{
			switch (str[i])
			{
			case 'A':
				Write(a);
				break;
			}

			writealpha += 28;
			if (writealpha >= 280) //�� �ٿ� �����ڰ� ������ٸ�
			{
				writeline += 32; //�ٹٲ�
				writealpha = 0;
			}
		}
	}

	return;
}

void Add(char input)
{
	int i;

	for (i = cursorMax; i >= cursorNow; i--)
	{
		str[i + 1] = str[i];
	}
	str[cursorNow] = input;
	cursorMax++;
	cursorNow++;

	WriteString();

	return;
}

void Delete()
{
	int i;

	for (i = cursorNow; i < cursorMax; i++)
	{
		str[i] = str[i + 1];
	}

	if (cursorMax <= 0)
	{
		printf("You Can't Delete Anymore!!\n");
		return;
	}

	else if (cursorNow == cursorMax)
	{
		cursorNow--;
		cursorMax--;
	}

	else
	{
		cursorMax--;
	}

	WriteString();

	return;
}

void DrawCursor()
{
	static int cursorColorFlag = 0;
	int fbfd;
	int ret;
	int i, j, count = 0;
	unsigned short cursorColor = 65535;

	struct fb_var_screeninfo fbvar;
	int offset;

	fbfd = open(FBDEVFILE, O_RDWR);

	if (fbfd < 0)
	{
		perror("fbdevopen");
		exit(1);
	}

	ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &fbvar);

	if (ret < 0)
	{
		perror("fbdevioctl");
		exit(1);
	}

	if (fbvar.bits_per_pixel != 16)
	{
		perror(" bpp is not 16\n ");
		exit(1);
	}

	if (cursorColorFlag == 0)
	{
		cursorColor = 65535;
		cursorColorFlag = 1;
	}

	else
	{
		cursorColor = 0;
		cursorColorFlag = 0;
	}
	for (count = 0, i = 0; count < 576; count++, i++) //���� �ϳ� ���
	{
		if (cursor[count] == 0)
		{
			continue;
		}

		else
		{
			if (i >= 24)
				i %= 24;

			j = (count / 24);

			offset = (j + ((cursorNow / 10) * 32)) * fbvar.xres * (16 / 8) + (i + (cursorNow % 10) * 28) * (16 / 8);

			if (lseek(fbfd, offset, SEEK_SET) < 0) {}

			write(fbfd, &cursorColor, 2); /*write 2bytes (16bit)*/
		}
	}

	close(fbfd);
}

void MoveCursor(int caseNumber)
{
	if (caseNumber == 0) //Move Left
	{
		WriteString();

		if (cursorNow == 0)
			printf("You Can't Go Left Anymore!!\n");

		else
			cursorNow--;
	}

	else //Move Right
	{
		WriteString();

		if (cursorNow >= cursorMax)
			printf("You Can't Go Right Anymore!!\n");

		else
			cursorNow++;
	}

	return;
}

void ControlCursor()
{
	time_t cursorCurrentTime = time(NULL);

	if ((cursorCurrentTime - cursorGlobalTime) >= 1)
	{
		DrawCursor();
		cursorGlobalTime = time(NULL);
	}

	return;
}

void Input(int caseNumber)
{
	switch (caseNumber)
	{
	case 1: //DEL
		Delete();
		break;

	case 3:

		break;
	case 4:
		Add('A');
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		break;
	case 9:
		break;
	case 10:
		break;

	}

	return;
}

int main()
{
	wiringPiSetup();

	initialize_textlcd();

	cursorGlobalTime = time(NULL);

	while (1)
	{
		if (!digitalRead(LEFT)) //case 0
		{
			MoveCursor(0);
			delay(300);
		}

		else if (!digitalRead(DEL)) //case 1
		{
			Input(1);
			delay(300);
		}

		else if (!digitalRead(RIGHT)) //case 2
		{
			MoveCursor(2);
			delay(300);
		}

		else if (digitalRead(DOTQZ))
		{
			delay(300);
		}

		else if (digitalRead(ABC))
		{
			Input(4);
			delay(300);
		}

		else if (digitalRead(DEF))
		{
			delay(300);
		}

		else if (digitalRead(GHI))
		{
			delay(300);
		}

		else if (digitalRead(JKL))
		{
			delay(300);
		}

		else if (digitalRead(MNO))
		{
			delay(300);
		}

		else if (digitalRead(PRS))
		{
			delay(300);
		}

		else if (digitalRead(TUV))
		{
			delay(300);
		}

		else if (digitalRead(WXY))
		{
			delay(300);
		}

		else
			ControlCursor();
	}

	return 0;
}