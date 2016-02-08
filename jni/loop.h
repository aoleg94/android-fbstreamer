#ifndef LOOP_H
#define LOOP_H

#include <sys/uio.h>

typedef int (*GetData)(const struct iovec** ptr);
int loop(GetData gd);

#endif // LOOP_H

