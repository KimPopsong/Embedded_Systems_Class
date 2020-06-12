#include <stdio.h>
#include <stdlib.h> /*for exit*/
#include <unistd.h>/*for open/close..*/
#include <fcntl.h> /*for O_RDWR*/
#include <sys/ioctl.h> /*for ioctl*/
#include <linux/fb.h> /*for fb_var_screeninfo, FBIOGET_VSCREENINFO*/
#include <sys/types.h>

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
		perror("fbdevopen"); exit(1);
	}
	// 커널로부터 LCD에 관한 정보를 fbvar에 저장한다. 

	ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &fbvar);

	if (ret < 0) {
		perror("fbdevioctl");
		exit(1);
	}

	if (fbvar.bits_per_pixel != 16) { //bpp가 16bit인지 check
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

			offset = (0 + j) * fbvar.xres * (16 / 8) + (0 + i + writealpha) * (16 / 8);

			if (lseek(fbfd, offset, SEEK_SET) < 0)
			{ //file point를 맨 앞에서 offset만큼 옮김perror("fbdevlseek");exit(1);
			}

			write(fbfd, &white, 2); /*write 2bytes (16bit)*/
		}
	}

	writealpha += 28;

	close(fbfd);
	return;
}

int main()
{
	Write(a);
	Write(a);
	Write(a);
	Write(a);
	Write(a);
	Write(a);
	Write(a);
	Write(a);
	Write(a);
	Write(a);

	return 0;
}