#ifndef REVOLUTION_ARRAY_H_INCLUDED
#define REVOLUTION_ARRAY_H_INCLUDED

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

struct RVArray
{
    double *data;
    size_t size;
};//~ RVArray

#ifdef __cplusplus
}
#endif

#endif
