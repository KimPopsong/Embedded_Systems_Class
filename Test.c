#include <stdio.h>
#include <Windows.h>

int main()
{
	printf("%d\n", (unsigned short)(((255 >> 3) << 11) | ((255 >> 2) << 5) | (255 >> 3)));

	system("pause");
	return 0;
}