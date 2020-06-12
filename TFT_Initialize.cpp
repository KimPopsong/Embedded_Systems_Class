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

int cur = 0;

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

void initialize_textlcd() //CLCD �ʱ�ȭ
{
	pinMode(LEFT, INPUT);
	pinMode(RIGHT, INPUT);
	pinMode(DEL, INPUT);
	pinMode(ABC, INPUT);
	pinMode(DEF, INPUT);
	pinMode(GHI, INPUT);
	pinMode(JKL, INPUT);
	pinMode(MNO, INPUT);
	pinMode(PRS, INPUT);
	pinMode(TUV, INPUT);
	pinMode(WXY, INPUT);
	pinMode(DOTQZ, INPUT);

	pullUpDnControl(LEFT, PUD_UP);
	pullUpDnControl(RIGHT, PUD_UP);
	pullUpDnControl(DEL, PUD_UP);
	pullUpDnControl(ABC, PUD_DOWN);
	pullUpDnControl(DEF, PUD_DOWN);
	pullUpDnControl(GHI, PUD_DOWN);
	pullUpDnControl(JKL, PUD_DOWN);
	pullUpDnControl(MNO, PUD_DOWN);
	pullUpDnControl(PRS, PUD_DOWN);
	pullUpDnControl(TUV, PUD_DOWN);
	pullUpDnControl(WXY, PUD_DOWN);
	pullUpDnControl(DOTQZ, PUD_DOWN);
}

void WRITE(int alpha[])
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
		perror("fbdevopen"); exit(1);
	}
	// Ŀ�ηκ��� LCD�� ���� ������ fbvar�� �����Ѵ�. 

	ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &fbvar);

	if (ret < 0) {
		perror("fbdevioctl");
		exit(1);
	}

	if (fbvar.bits_per_pixel != 16) { //bpp�� 16bit���� check
		perror(" bpp is not 16\n ");
		exit(1);
	}

	fbfd = open(FBDEVFILE, O_RDWR);

	for (count = 0, i = 0; count < 576; count++, i++)
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

			offset = (0 + j) * fbvar.xres * (16 / 8) + (0 + i + cur) * (16 / 8);

			if (lseek(fbfd, offset, SEEK_SET) < 0)
			{ //file point�� �� �տ��� offset��ŭ �ű�perror("fbdevlseek");exit(1);
			}

			write(fbfd, &white, 2); /*write 2bytes (16bit)*/
		}
	}

	cur += 28;

	close(fbfd);
	return;
}

int main()
{
	wiringPiSetup();

	initialize_textlcd();

	printf("%d", digitalRead(LEFT));
	printf("%d", digitalRead(RIGHT));
	printf("%d", digitalRead(DEL));
	printf("%d", digitalRead(ABC));
	printf("%d", digitalRead(DEF));
	printf("%d", digitalRead(GHI));
	printf("%d", digitalRead(JKL));
	printf("%d", digitalRead(MNO));
	printf("%d", digitalRead(PRS));
	printf("%d", digitalRead(TUV));
	printf("%d", digitalRead(WXY));
	printf("%d", digitalRead(DOTQZ));

	return 0;
}