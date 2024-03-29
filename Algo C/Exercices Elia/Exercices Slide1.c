#include <stdio.h>
#include <stdlib.h>
#include <string.h>




void ClearBuffer() {
	while (1) {
		char c = getchar(); 
		if (c == "\n") {
			break;
		}
	}
}


void ChangeValue2(float* tab, int size) {

	for (int count = 0; count < size; count++) {
		tab[count] = 5;
	}

	for (int count = 0; count < size; count++) {
		printf("%f", tab[count]);
	}
}




void ChangeValue1(float* tab, int value, int size) {
	for (int count = 0; count < size; count++) {
		*(tab + count) = 5;
	}

	for (int count = 0; count < size; count++) {
		printf("%f", tab[count]);
	}
}


void ChangeValueInput(int* tab, int size) {
	for (int count = 0; count < size; count++) {
		int value; 
		printf(" \"[%d] => \" ", count);
		scanf_s("%d", &value);
		ClearBuffer();
		printf("\n");
		tab[count] = value;
	}

	for (int count = 0; count < size; count++) {
		printf(" \"[%d] => %d\" \n", count, tab[count]);
	}

}



void Exo1() {

	// Exercices 1 :
	float f1;
	f1 = 5;
	float* fptr = &f1;
	*fptr = 10;
	float f2;
	fptr = &f2;
	*fptr = 20;
	printf("%f %f", f1, f2);
}

void Exo2() {
	//Exercice2 : 
	float* pf1 = (float*)malloc(sizeof(float));
	if (pf1 == NULL) {
		exit(1);
	}

	*pf1 = 5;

	float* pf2 = (float*)malloc(sizeof(float));
	if (pf2 == NULL) {
		exit(1);
	}
	*pf2 = 25.5;

	float* pf3 = (float*)malloc(sizeof(float));
	if (pf3 == NULL) {
		exit(1);
	}

	*pf3 = *pf1;
	*pf1 = *pf2;
	*pf2 = *pf3;

	printf("%f %f", *pf1, *pf2);
	free(pf1);
	free(pf2);
	free(pf3);
}

void Exo3() {


	//Exercice 3 :
	float* tf = (float*)malloc(sizeof(float) * 10);

	if (tf == NULL) {
		exit(1);
	}
	*(tf + 0) = 0;
	*(tf + 1) = 0;
	*(tf + 2) = 0;
	*(tf + 3) = 0;
	*(tf + 4) = 0;
	*(tf + 5) = 0;
	*(tf + 6) = 0;
	*(tf + 7) = 0;
	*(tf + 8) = 0;
	*(tf + 9) = 0;


	ChangeValue1(tf, 5, 10);

	free(tf);
}

void Exo4() {

	// Exercice 4 :

	float tf2[10];
	tf2[0] = 0;
	tf2[1] = 0;
	tf2[2] = 0;
	tf2[3] = 0;
	tf2[4] = 0;
	tf2[5] = 0;
	tf2[6] = 0;
	tf2[7] = 0;
	tf2[8] = 0;
	tf2[9] = 0;

	ChangeValue2(tf2, 10);
}

void Exo5() {


	//Exercice5:

	printf("Choose a size for the tab : ");
	int size;
	scanf_s("%d", &size);
	ClearBuffer();
	int* tabint = (int*)malloc(sizeof(int) * size);
	ChangeValueInput(tabint, size);

}

void Exo6() {

	int size = 1;
	int stop = 0;
	int* tab = (int*)malloc(sizeof(int) * size);
	if (tab == NULL)
		exit(1);

	while (!stop) {

		printf("Entrer une valeur positive a ajouter au tableau ou -1 pour arreter.");
		int value;

		scanf_s("%d", &value);
		ClearBuffer();

		if (value == -1) {
			stop =1;
		}
		else if (value >= 0){

			tab[size - 1] = value;
			size += 1;

			tab = (int*)realloc(tab, sizeof(int) * size);
			if (tab == NULL)
				exit(1);
		}


	}

	for (int count = 0; count < size-1; count++) {
		printf(" \"[%d] => %d\" \n", count, tab[count]);
	}

}

int main() {

	Exo1();
	Exo2();
	Exo3();
	Exo4();
	Exo5();
	Exo6();
}