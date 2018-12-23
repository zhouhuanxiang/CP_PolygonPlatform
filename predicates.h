#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef REAL
#define REAL float                      /* float or double */
#endif

REAL incirclefast(REAL *pa, REAL *pb, REAL *pc, REAL *pd);
