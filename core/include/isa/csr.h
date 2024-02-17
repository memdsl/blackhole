#ifndef __CSR_H__
#define __CSR_H__

#include <common.h>

word_t getISACSR(int id);
word_t getISACSRData(const char *gpr, bool *success);
const char *getISACSRName(int id);
void printfISACSRData();

#endif
