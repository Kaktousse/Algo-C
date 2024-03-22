#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

typedef struct String
{
    char* pContent;
    int iLength;
}String;

String Create(const char* str)
{
    String myString; 
    myString.iLength = sizeof(str);
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

    newString.iLength = sizeof(str1) + sizeof(str2);
    newString.pContent = (char*)malloc(sizeof(char) * newString.iLength);

    int count = 0;

    for (int i = 0; i < sizeof(str1); i++) {
        newString.pContent[count] = str1[i];
        count += 1;
    }

    for (int i = 0; i < sizeof(str2); i++) {
        newString.pContent[count] = str2[i];
        count += 1;
    }

    return newString;
}

String SubString(const String* pStr1, int iStartIndex, int iLength)
{
    String newString;

    newString.iLength = iLength * sizeof(char);
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
        printf("%d", pStr1->iLength);
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

}

int TryCastToInt(const String* pStr, int* pResult)
{

}

void DestroyString(String* pStr)
{

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

    String str6 = InsertString(&str4, &str1, 6);
    PrintString(&str6);

    Destroy(&str1);
    Destroy(&str2);
    Destroy(&str3);

    return 0;

}