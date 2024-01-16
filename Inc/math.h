/*
 * math.h
 *
 *  Created on: Jan 9, 2024
 *      Author: rotho
 */
#define FIX 8
#define FIXMUL(a, b) ((a)*(b)) >> FIX
#define FIXSQUARE(a) FIXMUL(a, a)
#include <stdio.h>

#ifndef MATH_H_
#define MATH_H_

typedef struct {
	int32_t x, y;
}vector_t;

typedef struct {
	int8_t x, y;
}vector8_t;

//Printing
void printFix(int32_t i);

void printVector(vector_t *v);
//Clamping
void clamp(int32_t *x, int32_t min, int32_t max);
void clampVector(vector_t *v, int32_t min, int32_t max);

//Bounds checking
char outOfBounds(int32_t num, int32_t min, int32_t max);

//Conversion
int16_t FIXToint16(int32_t fix);

//Vectors
vector_t addVectors(vector_t *v1, vector_t *v2);
vector_t subtractVectors(vector_t *v1, vector_t *v2);
vector_t multFIXVector(vector_t *v, int32_t k);
uint32_t distFIXSquared(vector_t *p, vector_t *q, uint8_t shift);

//Pseudo random number generation
uint8_t random8bit();
#endif /* MATH_H_ */
