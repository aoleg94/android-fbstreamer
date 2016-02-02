#ifndef LOOP_H
#define LOOP_H

typedef int (*GetData)(const void** ptr);
int loop(GetData gd);

#endif // LOOP_H

