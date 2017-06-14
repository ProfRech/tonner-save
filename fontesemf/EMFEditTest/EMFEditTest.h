#ifndef _EMFEDITTEST_H_
#define _EMFEDITTEST_H_

typedef int (EditEMFFunc)(char* input, char* output, unsigned percentage);

int CallFunction(char* i, char* o, int p);
char* ResultText(int r);

#endif // !_EMFEDITTEST_H_


