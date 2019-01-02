#ifndef PREDICATES_H_
#define PREDICATES_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef REAL
#define REAL double                      /* float or double */
#endif

void exactinit();

REAL orient2d(REAL *pa, REAL *pb, REAL *pc);

REAL orient2dfast(REAL *pa, REAL *pb, REAL *pc);

REAL incircle(REAL *pa, REAL *pb, REAL *pc, REAL *pd);

REAL incirclefast(REAL *pa, REAL *pb, REAL *pc, REAL *pd);

#endif