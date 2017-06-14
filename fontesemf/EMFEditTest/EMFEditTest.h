#pragma once

typedef int (EditEMFFunc)(char* input, char* output, unsigned percentage);
EditEMFFunc* editEMF;

char* inputs[] =
{
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test4.emf",
	"",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test.emf",
};
char* outputs[] =
{
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test3.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
	"C:\\Users\\Vitor\\git\\tonner-save\\fontesemf\\Test2.emf",
	""
};
int percentages[] =
{
	25,
	25,
	25,
	25,
	-1,
	101,
	25,
	25,
	25
};
int resultExpected[] =
{
	0,
	0,
	0,
	4,
	2,
	2,
	1,
	1,
	3
};


int CallFunction(char* i, char* o, int p);
char* ResultText(int r);
