#ifndef REVOLUTION_H_INCLUDED
#define REVOLUTION_H_INCLUDED

#include "defines.h"

#ifdef __cplusplus
extern "C" {
#endif

DLL_PUBLIC int rv_major_version(void);
DLL_PUBLIC int rv_minor_version(void);
DLL_PUBLIC int rv_patch_version(void);

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_H_INCLUDED
