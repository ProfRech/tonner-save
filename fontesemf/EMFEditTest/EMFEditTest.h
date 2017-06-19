#ifndef _EMFEDITTEST_H_
#define _EMFEDITTEST_H_

typedef struct {
	char* input;
	char* output;
	int percentage;
	int expectedResult;
} Test;

typedef int (EditEMFFunc)(char* input, char* output, int percentage);

int CallFunction(char* i, char* o, int p);
char* ResultText(int r);

#endif // !_EMFEDITTEST_H_


