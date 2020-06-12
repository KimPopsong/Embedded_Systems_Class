#include <stdio.h>
#include <stdlib.h>/* for exit */
#include <unistd.h> /* for open/close.. */
#include <fcntl.h> /* for O_RDONLY */
#include <sys/ioctl.h> /* for ioctl*/
#include <sys/types.h> /* for lseek() */
#include <linux/fb.h> /*for fb_var_screeninfo, FBIOGET_VSCREENINFO*/
#define FBDEVFILE "/dev/fb2"

typedef unsigned char ubyte;

//�� ������ 16bit�� ��ȯ�� �ִ� �Լ�
unsigned short makepixel(ubyte r, ubyte g, ubyte b)
{
	return (unsigned short)(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
}

int main() {
	int fbfd;
	int ret;
	int i, j;
	struct fb_var_screeninfo fbvar;
	unsigned short pixel;
	int offset;//frame buffer driver file point�� ��´�.

	fbfd = open(FBDEVFILE, O_RDWR);

	if (fbfd < 0) { perror("fbdevopen"); exit(1); }
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

	for (i = 0; i < 50; i++)
	{
		for (j = 0; j < 50; j++)
		{
			offset = (50 + i) * fbvar.xres * (16 / 8) + (100 + j) * (16 / 8);
			if (lseek(fbfd, offset, SEEK_SET) < 0) { //file point�� �� �տ��� offset��ŭ �ű�perror("fbdevlseek");exit(1);
			}
			pixel = makepixel(0, 255, 0); /*green pixel */
			write(fbfd, &pixel, 2); /*write 2bytes (16bit)*/
		}
	}

	write(fbfd, &pixel, 2); /*write 2byte(16 bit)*/
	close(fbfd);

	return 0;
} // end of main
