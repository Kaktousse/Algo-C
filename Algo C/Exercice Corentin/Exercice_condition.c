#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ClearBuffer()
{
	while (getchar() != '\n');
}

int EvenOrUneven()
{
	int x;

	printf("Choose an interger: ");
	scanf_s("%d", &x);
	ClearBuffer();
	if (x % 2 == 1)
		printf("%d is uneven !\n", x);
	else
		printf("%d is even !\n", x);
}

int Compare()
{
	int a, b;

	printf("\nChoose two interger a and b: \n");
	printf("a: ");
	scanf_s("%d", &a);
	ClearBuffer();
	printf("b: ");
	scanf_s("%d", &b);
	ClearBuffer();

	if (a > b)
		printf("%d superior to %d\n", a, b);
	if (a < b)
		printf("%d is inferior to %d\n", a, b);
	if (a == b)
		printf("%d %d are equal\n", a, b);
}

int Order()
{
	int c, d, e;

	printf("\nChoose three interger: \n");
	printf("1: ");
	scanf_s("%d", &c);
	ClearBuffer();
	printf("2: ");
	scanf_s("%d", &d);
	ClearBuffer();
	printf("3: ");
	scanf_s("%d", &e);
	ClearBuffer();

	if (c > d && c > e)
	{
		printf("%d ", c);
		if (d > e)
			printf("%d %d", d, e);
		else if (d < e)
			printf("%d %d", e, d);
	}

	if (d > c && d > e)
	{
		printf("%d ", d);
		if (c > e)
			printf("%d %d", c, e);
		else if (c < e)
			printf("%d %d", e, c);
	}

	if (e > d && e > c)
	{
		printf("%d ", e);
		if (d > c)
			printf("%d %d", d, c);
		else if (d < c)
			printf("%d %d", c, d);
	}

}

int IsInt()
{ 
	float f, g;

	printf("Choose an number: ");
	scanf_s("%f", &f);
	ClearBuffer();

	int i = (int)f;

	g = f - i;

	if ( g > 0 && g < 1 )
		printf("%f is not an integer", f);
	else if( g == 0 )
		printf("%f is an integer", f);
}

int IsInAlphabets()
{
	char c;

	printf("Choose a character: ");
	scanf_s("%c", &c);
	ClearBuffer();

	int dec = (int)c;

	if(dec > 97 && dec < 122 || dec > 65 && dec < 90)
	{
		printf("The character %c is in the alphabets", c);
		if (dec > 97 && dec < 122)
			printf(" and is a lowercase");
		if (dec > 65 && dec < 90)
			printf(" and is a capital letter");
	}
	else
		printf("The character %c is not in the alphabets", c);
}

int TheDate()
{
	int day, month, year, i;

	const char* monthlist[][2] = { {"Janvier", 31}, {"Fevrier", 28}, {"Mars", 31}, {"Avril", 30}, {"Mai", 31}, {"Juin", 30}, {"Juillet", 31}, {"Aout", 31}, {"Septembre", 30}, {"Octobre", 31}, {"Novembre", 30}, {"Decembre", 31} };
	

	printf("Give a date (dd/mm/yyyy): ");
	scanf_s("%d/%d/%d", &day, &month, &year);
	ClearBuffer();

	i = 0;

	if (year % 4 == 0)
	{
		monthlist[1][1] = 29;
		i = 0;
	}
	else
		i = 1;

	if (month <= 12)
	{
		if (day > monthlist[month - 1][1])
		{
			printf("There are only %d in %s", monthlist[month - 1][1], monthlist[month - 1][0]);

			if (i == 1)
				printf(" and this is not a leap year !/");

			return 0;
		}
		else
			printf("%d - %s - %d", day, monthlist[month - 1][0], year);
	}
	else
		printf("this is not a month number");
}

int main()
{
	EvenOrUneven();
	Compare();
	Order();
	IsInt();
	IsInAlphabets();
	TheDate();
}
