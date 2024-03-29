#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ClearBuffer()
{
	while (getchar() != '\n');
}

int CountDigit(int num)
{
	int digit;

	digit = 0;

	while (num > 1)
	{
		num /= 10;
		digit += 1;
	}

	return digit;
}

int main()
{
	int num, a, b, c, reversenum, digit, i;
	int tab[2];

	printf("Choose an interger: ");
	scanf_s("%d", &num);
	ClearBuffer();

	digit = CountDigit(num);
	printf("%d", digit);

	i = 0;

	while (i <digit)
	{
		b = num / 10;
		c = num;
		a = c - b * 10;
		num /= 10;
		tab[i] = a;
		i++;
		printf("\naaaaaaaaaaaaa");
	}



	return 0;
}