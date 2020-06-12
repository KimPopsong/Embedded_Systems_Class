#include <stdio.h>
#include <stdlib.h>/* for exit */
#include <unistd.h> /* for open/close.. */
#include <fcntl.h> /* for O_RDONLY */
#include <sys/ioctl.h> /* for ioctl*/
#include <sys/types.h> /* for lseek() */
#include <linux/fb.h> /*for fb_var_screeninfo, FBIOGET_VSCREENINFO*/
#define FBDEVFILE "/dev/fb2"

typedef unsigned char ubyte;

//색 정보를 16bit로 변환해 주는 함수
unsigned short makepixel(ubyte r, ubyte g, ubyte b)
{
	return (unsigned short)(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
}

int main() {
	int fbfd;
	int ret;
	struct fb_var_screeninfo fbvar;
	unsigned short pixel;
	int offset;//frame buffer driver file point를 얻는다.

	fbfd = open(FBDEVFILE, O_RDWR);

	if (fbfd < 0) { perror("fbdevopen"); exit(1); }
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

	//red 점을(0,0)에찍는다.
	if (lseek(fbfd, 0, SEEK_SET) < 0) {   //file pointer를 맨 앞으로 옮김
		perror("fbdevlseek"); exit(1);
	}

	pixel = makepixel(255, 0, 0);  /*red pixel*/

	write(fbfd, &pixel, 2);  /*write 2bytes (16bit)*/

	//green점을(100,50)에 찍는다.
	offset = 50 * fbvar.xres * (16 / 8) + 100 * (16 / 8);
	if (lseek(fbfd, offset, SEEK_SET) < 0) { //file point를 맨 앞에서 offset만큼 옮김perror("fbdevlseek");exit(1);
	}
	pixel = makepixel(0, 255, 0); /*green pixel */
	write(fbfd, &pixel, 2); /*write 2bytes (16bit)*/

	//blue점을 (50,100)에 찍는다.
	offset = 100 * fbvar.xres * (16 / 8) + 50 * (16 / 8);
	if (lseek(fbfd, offset, SEEK_SET) < 0) {  //file pointer를 맨 앞에서 offset만큼 옮김	perror("fbdevlseek");exit(1);
	}
	pixel = makepixel(0, 0, 255); /*blue pixel */

	write(fbfd, &pixel, 2);   /*write 2bytes (16bit)*/
	//white점을(100,100)에 찍는다.
	offset = 100 * fbvar.xres * (16 / 8) + 100 * (16 / 8);
	if (lseek(fbfd, offset, SEEK_SET) < 0) { //file point를 맨 앞에서 offset만큼 옮김	perror("fbdevlseek");exit(1); 
	}
	pixel = makepixel(255, 255, 255); /*white pixel */

	write(fbfd, &pixel, 2); /*write 2byte(16 bit)*/
	close(fbfd);

	return 0;
} // end of main
