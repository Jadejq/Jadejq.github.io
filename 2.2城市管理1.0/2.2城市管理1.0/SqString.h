#pragma once
#include <iostream>
#include <stdlib.h>
#define MaxSize 100

using namespace std;
typedef struct
{
	char data[MaxSize];
	int length;			//´®³¤
} SqString;
void StrAssign(SqString& s, char cstr[]);
void DestroyStr(SqString& s);
void StrCopy(SqString& s, SqString t);
bool StrEqual(SqString s, SqString t);
int StrLength(SqString s);
SqString Concat(SqString s, SqString t);
SqString SubStr(SqString s, int i, int j);
SqString InsStr(SqString s1, int i, SqString s2);
SqString DelStr(SqString s, int i, int j);
SqString RepStr(SqString s, int i, int j, SqString t);
void DispStr(SqString s);