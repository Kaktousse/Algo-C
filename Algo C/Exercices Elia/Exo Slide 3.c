#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include<math.h>

typedef struct String
{
    char* pContent;
    int iLength;
}String;

String Create(const char* str)
{
    String myString; 
    myString.iLength = strlen(str);
    myString.pContent = (char*)malloc(sizeof(char) * myString.iLength); 
    for (int i = 0; i < myString.iLength; i++) {
        myString.pContent[i] = str[i]; 
    }
    return myString; 
}

void PrintString(const String* pStr)
{
    for (int i = 0; i < pStr->iLength; i++) {
        printf("%c", pStr->pContent[i]);
    }
    printf("\n");
}

//

String Concatenate1(const String* pStr1, const String* pStr2)
{
    String newString;

    newString.iLength = pStr1->iLength + pStr2->iLength;
    newString.pContent = (char*)malloc(sizeof(char) * newString.iLength);

    int count = 0;

    for (int i = 0; i < pStr1->iLength; i++) {
        newString.pContent[count] = pStr1->pContent[i];
        count += 1;
    }

    for (int i = 0; i < pStr2->iLength; i++) {
        newString.pContent[count] = pStr2->pContent[i];
        count += 1;
    }

    return newString;
}

String Concatenate2(const char* str1, const char* str2)
{
    String newString;

    newString.iLength = strlen(str1) + strlen(str2);
    newString.pContent = (char*)malloc(sizeof(char) * newString.iLength);

    int count = 0;

    for (int i = 0; i < strlen(str1); i++) {
        newString.pContent[count] = str1[i];
        count += 1;
    }

    for (int i = 0; i < strlen(str2); i++) {
        newString.pContent[count] = str2[i];
        count += 1;
    }

    return newString;
}

String SubString(const String* pStr1, int iStartIndex, int iLength)
{
    String newString;

    newString.iLength = iLength;
    newString.pContent = (char*)malloc(sizeof(char) * newString.iLength);

    for (int i = 0; i < iLength; i++) {
        newString.pContent[i] = pStr1->pContent[iStartIndex+i];
    }


    return newString;
}

String InsertString(const String* pStr1, const String* Pstr2, int iIndex)
{

    if (iIndex > 0 && iIndex < pStr1->iLength) {

        String newString;

        newString.iLength = pStr1->iLength + Pstr2->iLength;
        newString.pContent = (char*)malloc( sizeof(char) * newString.iLength);


        int count = 0;

        for (int i = 0; i < pStr1->iLength ; i++) {
            if (i == iIndex) {
                for (int j = 0; j < Pstr2->iLength; j++) {
                    newString.pContent[count] = Pstr2->pContent[j];
                    count += 1;
                }
            }
            newString.pContent[count] = pStr1->pContent[i];
            count += 1;

        }


        return newString;

    }

    return;
}

int AreEquals(const String* pStr1, const String* pStr2)
{
    if (pStr1->iLength != pStr2->iLength) {
        return 0;
    }
    
    for (int i = 0; i < pStr1->iLength; i++) {
        if (pStr1->pContent[i] != pStr2->pContent[i]) {
            return 0;
        }
    }

    return 1;
}



int TryCastToInt(const String* pStr, int* pResult)
{

    for (int i = 0; i < pStr->iLength; i++) {
        if ( '0' <= pStr->pContent[i] && pStr->pContent[i] <= '9') {
            continue;
        }
        else {
            return 0;
        }
    }
    int count = pow(10, pStr->iLength-1);

    *pResult = 0;

    for (int j = 0; j < pStr->iLength; j++) {
        *pResult += (int)( pStr->pContent[j] - '0')*count ;
        count = count / 10;
    }

    return 1; 
}

void DestroyString(String* pStr)
{
    free(pStr->pContent);
}


int main() {
    
    String str1 = Create("Bonjour");

    PrintString(&str1);

    String str2 = Create("Aurevoir");
    PrintString(&str2);


    String str3 = Concatenate1(&str1, &str2);
    PrintString(&str3);

    String str4 = Concatenate2("AAAA", "BBBBBBB");
    PrintString(&str4);

    String str5 = SubString(&str3, 2, 2);
    PrintString(&str5);

    String str6 = InsertString(&str4, &str1, 8);
    PrintString(&str6);

    String str7 = Create("365");
    PrintString(&str7);
    int Result1;

    String equal1 = Create("CCC"); 
    String equal2 = Create("BBB");

    printf(" %d \n", AreEquals(&equal1, &equal1));
    printf(" %d \n", AreEquals(&equal1, &equal2));
    printf(" %d \n", AreEquals(&equal1, &str1));


    if (TryCastToInt(&str7, &Result1)) {
        printf("%d\n", Result1);
    }

    String str8 = Create("3dd46");
    PrintString(&str8);
    int Result2;


    if (TryCastToInt(&str8, &Result2)) {
        printf("%d\n", Result2);
    }

    DestroyString(&str1);
    DestroyString(&str2);
    DestroyString(&str3);

    return 0;

}