/*
 * math.c
 *
 *  Created on: Jan 9, 2024
 *      Author: rotho
 */
#include "math.h"
//#include <stdio.h>
#include "30010_io.h" 		// Input/output library for this course

//Printing
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

//Clamping
void clamp(int32_t *x, int32_t min, int32_t max){
	if(*x < min)
		*x = min;
	if(*x > max)
		*x = max;
}
void clampVector(vector_t *v, int32_t min, int32_t max){
	clamp(&(v->x), min, max);
	clamp(&(v->y), min, max);
}

//Bounds checking
char outOfBounds(int32_t num, int32_t min, int32_t max){
	return (num < min || num > max);
}

//Conversion
int16_t FIXToint16(int32_t fix){
	return fix >> FIX;
}

//Vectors
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

vector_t divideFIXVector(vector_t *v, int32_t k) {
	vector_t vNew = {FIXDIV((*v).x, k), FIXDIV((*v).y, k)};
	return vNew;
}

// MAYBE THIS ISN'T IDEAL, I JUST REALLY NEED A SQUARE ROOT FUNCTION.
// TAKEN FROM https://stackoverflow.com/questions/45148893/square-root-source-code-found-on-the-net
#include <limits.h>
// greatest power of 4 <= a power-of-2 minus 1
#define POW4_LE_POW2M1(n) (  ((n)/2 + 1) >> ((n)%3==0)  )

unsigned isqrt(unsigned num) {
  unsigned res = 0;
  // The second-to-top bit is set: 1 << 30 for 32 bits
  // Needs work to run on unusual platforms where `unsigned` has padding or odd bit width.
  unsigned bit = POW4_LE_POW2M1(UINT_MAX);
  // "bit" starts at the highest power of four <= the argument.
  while (bit > num) { bit >>= 2; }
  while (bit > 0) {
    if (num >= res + bit) {
      num -= res + bit;
      res = (res >> 1) + bit;  // Key difference between this and OP's code
    } else {
      res >>= 1;
    }
    bit >>= 2;
  }
  return res;
}

vector_t normalizeFIXVector(vector_t* v) {
	int32_t mag = isqrt(SQUARE(v->x)+SQUARE(v->y));
	vector_t result = divideFIXVector(v, mag);
	return result;
}

uint32_t dotFIX(vector_t* a, vector_t* b) {
	return (FIXMUL(a->x, b->x) + FIXMUL(a->y, b->y));
}

uint32_t distFIXSquared(vector_t *p, vector_t *q, uint8_t shift){
	return ((q -> x - p -> x) * (q -> x - p -> x) + (q -> y - p -> y) * (q -> y - p -> y)) >> shift;
}

//Pseudo-random number generation
uint16_t P1  = 0x3FF; // Register 1
uint16_t P2  = 0x3FF; // Register 2
uint8_t random8bit(){
    uint16_t FB1 = 0x204; // Feedback locations for register 1
    uint16_t FB2 = 0x3A6; // Feedback locations for register 2
    uint16_t O1  = 0x200; // Output locations for register 1
    uint8_t  O2  = 0x022; // Output locations for register 2
    uint8_t  tmp; // Temporary storage
    uint16_t i; // Iteration variable
    uint8_t output = 0;

    for (i = 0 ; i < 8 ; i++) {
        tmp = (P1 & O1) ^ (P2 & O2); // Extract output bits
        tmp ^= tmp >> 8; tmp ^= tmp >> 4; tmp ^= tmp >> 2; tmp ^= tmp >> 1; // Calculate digit sum (mod 2)
        //tmp is now a single pseudorandom bit at lsb.
        output |= tmp << i; // Return bit 0 of the output

        // HANDLE REGISTER 1
        tmp = P1 & FB1; // Extract feedback bits
        tmp ^= tmp >> 8; tmp ^= tmp >> 4; tmp ^= tmp >> 2; tmp ^= tmp >> 1; // Calculate digit sum (mod 2)
        P1 >>= 1; P1 &= 0xFFFE; P1 = tmp & 0x0001; // Shift bits and add feedback

        // HANDLE REGISTER 2
        tmp = P2 & FB2; // Extract feedback bits
        tmp ^= tmp >> 8; tmp ^= tmp >> 4; tmp ^= tmp >> 2; tmp ^= tmp >> 1; // Calculate digit sum (mod 2)
        P2 >>= 1; P2 &= 0xFFFE; P2 |= tmp & 0x0001; // Shift bits and add feedback
    }
}
