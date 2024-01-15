/*
 * math.c
 *
 *  Created on: Jan 9, 2024
 *      Author: rotho
 */
#include "math.h"
//#include <stdio.h>
#include "30010_io.h" 		// Input/output library for this course

const signed short SINE[512]={};

//Utility
void printFix(int32_t i) {
	 // Prints a signed 16.16 fixed point number
	 if ((i & 0x80000000) != 0) { // Handle negative numbers
		 printf("-");
		 i = ~i + 1;
	 }
	 printf("%ld.%04ld", i >> FIX, 10000 * (uint32_t)(i & 0xFF) >> FIX);
	 // Print a maximum of 4 decimal digits to avoid overflow
}

void printVector(vector_t *v){
	printf("x: ");
	printFix(v -> x);
	printf(" y: ");
	printFix(v -> y);
}
void clampFIX(int32_t *x, int32_t min, int32_t max){
	if(*x < min)
		*x = min;
	if(*x > max)
		*x = max;
}
void clampVector(vector_t *v, int32_t min, int32_t max){
	clampFIX(&(v->x), min, max);
	clampFIX(&(v->y), min, max);
}

//Conversion
int16_t FIXToint16(int32_t fix){
	return fix >> FIX;
}

//Lookup functions
short sinLut(short a){
	return SINE[a & 0x01ff]; //Masking the input by magic to obtain modulus by 512 and dealing with negatives
}

short cosLut(short a){
	return SIN(a + 128);
}

//Vectors
void rotateVector(vector_t *v, short a){
	short sinVal = SIN(a);
	short cosVal = COS(a);
	v -> x = FIXMUL((*v).x, cosVal) - FIXMUL((*v).y, sinVal);
	v -> y = FIXMUL((*v).y, sinVal) + FIXMUL((*v).y, cosVal);

}

vector_t addVectors(vector_t *v1, vector_t *v2){
	vector_t v = {v1->x + v2->x, v1->y + v2->y};
	return v;
}
vector_t subtractVectors(vector_t *v1, vector_t *v2){
	vector_t v = {v1->x - v2->x, v1->y - v2->y};
	return v;
}
vector_t multFIXVector(vector_t *v, int32_t k){
	vector_t vNew = {FIXMUL((*v).x, k), FIXMUL((*v).y, k)};
	return vNew;
}

uint32_t distFIXSquared(vector_t *p, vector_t *q, uint8_t shift){
	return ((q -> x - p -> x) * (q -> x - p -> x) + (q -> y - p -> y) * (q -> y - p -> y)) >> shift;
}
