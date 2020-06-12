#include <stdio.h>
#include <stdlib.h> /*for exit*/
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
int dupleFlag = 0; //a -> b -> c -> a... 반복을 위한 변수
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

void Clear() //화면 초기화
{
	int fbfd;
	int ret;
	int i, j;
	unsigned short black = 0;

	struct fb_var_screeninfo fbvar;
	int offset;//frame buffer driver file point를 얻는다.

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

	fbfd = open(FBDEVFILE, O_RDWR);

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

void initialize_textlcd() //초기화
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
	int offset;//frame buffer driver file point를 얻는다.

	fbfd = open(FBDEVFILE, O_RDWR);

	if (fbfd < 0)
	{
		perror("fbdevopen");
		exit(1);
	}
	// 커널로부터 LCD에 관한 정보를 fbvar에 저장한다. 

	ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &fbvar);

	if (ret < 0)
	{
		perror("fbdevioctl");
		exit(1);
	}

	if (fbvar.bits_per_pixel != 16)
	{ //bpp가 16bit인지 check
		perror(" bpp is not 16\n ");
		exit(1);
	}

	fbfd = open(FBDEVFILE, O_RDWR);

	for (count = 0, i = 0; count < 576; count++, i++) //글자 하나 출력
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

void WriteString() //지금까지 입력 한 문자들을 str에 저장하고, WRITE함수로 한글자씩 보냄.
{
	int i;

	if (cursorMax >= 80) //화면에 80개의 글자를 띄웠다면
	{
		printf("Too Many Alphabets!\n");
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
			if (writealpha >= 280) //한 줄에 열글자가 띄워졌다면
			{
				writeline += 32; //줄바꿈
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
	cursorMax--;

	WriteString();

	return;
}

void MoveCursor(int caseNumber)
{
	return;
}

void Input(int caseNumber)
{
	switch (caseNumber)
	{
	case 0:
	case 2:
		MoveCursor(caseNumber);
		break;

	case 1:
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

	while (1)
	{
		if (!digitalRead(LEFT)) //case 0
		{
			Input(0);
			delay(300);
		}

		else if (!digitalRead(DEL)) //case 1
		{
			Input(1);
			delay(300);
		}

		else if (!digitalRead(RIGHT)) //case 2
		{
			Input(2);
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
	}

	return 0;
}