/*
 * math.h
 *
 *  Created on: Jan 9, 2024
 *      Author: rotho
 */
#include <stdio.h>
#define FIXMUL(a, b) ((a>>8)*(b>>8))

#ifndef MATH_H_
#define MATH_H_

typedef struct {
	int32_t x, y;
}vector_t;

//Utility
void printFix(int32_t i);
void printVector(vector_t *v);

//Conversion
int16_t FIXToint16(int32_t fix);

//Lookup functions
short SIN(short a);
short COS(short a);

//Vectors
void rotateVector(vector_t *v, short a);
vector_t addVectors(vector_t *v1, vector_t *v2);
vector_t multFIXVector(vector_t *v, int32_t k);
int32_t distFIXSquared(vector_t *p, vector_t *q);
int32_t FIXSquared(int32_t num);


#endif /* MATH_H_ */
