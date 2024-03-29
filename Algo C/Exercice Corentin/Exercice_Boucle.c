#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ClearBuffer()
{
	while (getchar() != '\n');
}

int PrimeNumber()
{
	int num, i, x, y;

	printf("choose a number: ");
	scanf_s("%d", &num);
	ClearBuffer();

	y = 1;

	for (i = 0; i < num; i++)
	{
		x = num / y;
		y++;
		printf("%d ", x);

	}

}

int main()
{
	PrimeNumber();
}