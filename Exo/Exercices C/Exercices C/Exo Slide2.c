#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct IntArray {
	int* pContent; 
	int iSize; 
	int iCapacity; 
} IntArray;


void Init(IntArray* pIntArray) {
	pIntArray->iSize = 5; 
	pIntArray->iCapacity = sizeof(int)*pIntArray->iSize ;

	pIntArray->pContent = (int*)malloc(pIntArray->iCapacity);
	if (pIntArray->pContent == NULL) {
		exit(1);
	}

}

void Add(IntArray* pIntArray, int iValue) {
	pIntArray->iSize += 1; 
	pIntArray->iCapacity += sizeof(int);
	pIntArray->pContent = (int*)realloc(pIntArray->pContent,pIntArray->iCapacity);

	if (pIntArray->pContent == NULL) {
		exit(1);
	}
	pIntArray->pContent[pIntArray->iSize - 1] = iValue;
}

void Insert(IntArray* pIntArray, int iValue, int iIndex) {
	if (iIndex > 0 && iIndex < pIntArray->iSize-1) {
		pIntArray->pContent[iIndex] = iValue; 
	}
}

void Remove(IntArray* pIntArray, int iIndex) {

	if (iIndex > 0 && iIndex < pIntArray->iSize - 1) {
		for (int i = iIndex; i < pIntArray->iSize-1; i++) {
			pIntArray->pContent[i] = pIntArray->pContent[i + 1];
		}
	}

	pIntArray->iSize -= 1;
	pIntArray->iCapacity -= sizeof(int);

	pIntArray->pContent = (int*)realloc(pIntArray->pContent, pIntArray->iCapacity);

	if (pIntArray->pContent == NULL) {
		exit(1);
	}
}

int Get(IntArray* pIntArray, int iIndex) {
	if (iIndex > 0 && iIndex < pIntArray->iSize - 1) {
		return pIntArray->pContent[iIndex];
	}
	return 0;
}

void Print(IntArray* pIntArray) {
	printf("\n");
	for (int count = 0; count < pIntArray->iSize; count++) {
		printf("\"[%d] => %d\"\n ", count, pIntArray->pContent[count]);
	}
}

void Destroy(IntArray* pIntArray) {

	free(pIntArray->pContent);
}

//int main() {

//	IntArray Array; 

//	Init(&Array);
//	Print(&Array);

//	Add(&Array, 155);
//	Print(&Array);

//	Add(&Array, 15);
//	Print(&Array);

//	Insert(&Array, 5, 3);
//	Print(&Array);

//	Remove(&Array, 1);
//	Print(&Array);

//	printf("%d",Get(&Array, 4));
	
//	Destroy(&Array);

//	return 0; 
//}