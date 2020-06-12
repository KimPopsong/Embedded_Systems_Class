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
char str[50] = { 0 };

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
}

int main()
{
	wiringPiSetup();

	initialize_textlcd();

	while (1)
	{
		if (!digitalRead(LEFT)) //case 0
		{
			printf("LEFT\n");
			delay(300);
		}

		else if (!digitalRead(DEL)) //case 1
		{
			printf("DEL\n");
			delay(300);
		}

		else if (!digitalRead(RIGHT)) //case 2
		{
			printf("RIGHT\n");
			delay(300);
		}

		else if (digitalRead(DOTQZ))
		{
			printf("DOWQZ\n");
			delay(300);
		}

		else if (digitalRead(ABC))
		{
			printf("ABC\n");
			delay(300);
		}

		else if (digitalRead(DEF))
		{
			printf("DEF\n");
			delay(300);
		}

		else if (digitalRead(GHI))
		{
			printf("GHI\n");
			delay(300);
		}

		else if (digitalRead(JKL))
		{
			printf("JKL\n");
			delay(300);
		}

		else if (digitalRead(MNO))
		{
			printf("MNO\n");
			delay(300);
		}

		else if (digitalRead(PRS))
		{
			printf("PRS\n");
			delay(300);
		}

		else if (digitalRead(TUV))
		{
			printf("TUV\n");
			delay(300);
		}

		else if (digitalRead(WXY))
		{
			printf("WXY\n");
			delay(300);
		}
	}

	return 0;
}