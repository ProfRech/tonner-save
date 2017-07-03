#ifndef _EMFEDITTEST_H_
#define _EMFEDITTEST_H_

#define SUCCESSFUL 0
#define INVALID_PERCENTAGE 1
#define INVALID_INPUT_FILE 2
#define INVALID_OUTPUT_FILE 3

typedef struct {
	char* input;
	char* output;
	unsigned __int8 percentage;
	int expectedResult;
} Test;

typedef int (EditEMFFunc)(char* input, char* output, unsigned __int8 percentage);

int CallFunction(char* i, char* o, int p);
char* ResultText(int r);

#endif // !_EMFEDITTEST_H_


