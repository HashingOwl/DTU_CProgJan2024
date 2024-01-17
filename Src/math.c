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

const uint8_t SINLUT[512] ={0x0,0x1,0x3,0x4,0x6,0x7,0x9,0xa,0xc,0xe,0xf,0x11,0x12,0x14,0x15,0x17,0x19,0x1a,0x1c,0x1d,0x1f,0x20,0x22,0x24,0x25,0x27,0x28,0x2a,0x2b,0x2d,0x2e,0x30,0x31,0x33,0x35,0x36,0x38,0x39,0x3b,0x3c,0x3e,0x3f,0x41,0x42,0x44,0x45,0x47,0x48,0x4a,0x4b,0x4d,0x4e,0x50,0x51,0x53,0x54,0x56,0x57,0x59,0x5a,0x5c,0x5d,0x5f,0x60,0x61,0x63,0x64,0x66,0x67,0x69,0x6a,0x6c,0x6d,0x6e,0x70,0x71,0x73,0x74,0x75,0x77,0x78,0x7a,0x7b,0x7c,0x7e,0x7f,0x80,0x82,0x83,0x84,0x86,0x87,0x88,0x8a,0x8b,0x8c,0x8e,0x8f,0x90,0x92,0x93,0x94,0x95,0x97,0x98,0x99,0x9b,0x9c,0x9d,0x9e,0x9f,0xa1,0xa2,0xa3,0xa4,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xd9,0xda,0xdb,0xdc,0xdd,0xdd,0xde,0xdf,0xe0,0xe1,0xe1,0xe2,0xe3,0xe3,0xe4,0xe5,0xe6,0xe6,0xe7,0xe8,0xe8,0xe9,0xea,0xea,0xeb,0xeb,0xec,0xed,0xed,0xee,0xee,0xef,0xef,0xf0,0xf1,0xf1,0xf2,0xf2,0xf3,0xf3,0xf4,0xf4,0xf4,0xf5,0xf5,0xf6,0xf6,0xf7,0xf7,0xf7,0xf8,0xf8,0xf9,0xf9,0xf9,0xfa,0xfa,0xfa,0xfb,0xfb,0xfb,0xfb,0xfc,0xfc,0xfc,0xfc,0xfd,0xfd,0xfd,0xfd,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfd,0xfd,0xfd,0xfd,0xfc,0xfc,0xfc,0xfc,0xfb,0xfb,0xfb,0xfb,0xfa,0xfa,0xfa,0xf9,0xf9,0xf9,0xf8,0xf8,0xf7,0xf7,0xf7,0xf6,0xf6,0xf5,0xf5,0xf4,0xf4,0xf4,0xf3,0xf3,0xf2,0xf2,0xf1,0xf1,0xf0,0xef,0xef,0xee,0xee,0xed,0xed,0xec,0xeb,0xeb,0xea,0xea,0xe9,0xe8,0xe8,0xe7,0xe6,0xe6,0xe5,0xe4,0xe3,0xe3,0xe2,0xe1,0xe1,0xe0,0xdf,0xde,0xdd,0xdd,0xdc,0xdb,0xda,0xd9,0xd9,0xd8,0xd7,0xd6,0xd5,0xd4,0xd3,0xd3,0xd2,0xd1,0xd0,0xcf,0xce,0xcd,0xcc,0xcb,0xca,0xc9,0xc8,0xc7,0xc6,0xc5,0xc4,0xc3,0xc2,0xc1,0xc0,0xbf,0xbe,0xbd,0xbc,0xbb,0xba,0xb9,0xb8,0xb7,0xb6,0xb5,0xb3,0xb2,0xb1,0xb0,0xaf,0xae,0xad,0xab,0xaa,0xa9,0xa8,0xa7,0xa6,0xa4,0xa3,0xa2,0xa1,0x9f,0x9e,0x9d,0x9c,0x9b,0x99,0x98,0x97,0x95,0x94,0x93,0x92,0x90,0x8f,0x8e,0x8c,0x8b,0x8a,0x88,0x87,0x86,0x84,0x83,0x82,0x80,0x7f,0x7e,0x7c,0x7b,0x7a,0x78,0x77,0x75,0x74,0x73,0x71,0x70,0x6e,0x6d,0x6c,0x6a,0x69,0x67,0x66,0x64,0x63,0x61,0x60,0x5f,0x5d,0x5c,0x5a,0x59,0x57,0x56,0x54,0x53,0x51,0x50,0x4e,0x4d,0x4b,0x4a,0x48,0x47,0x45,0x44,0x42,0x41,0x3f,0x3e,0x3c,0x3b,0x39,0x38,0x36,0x35,0x33,0x31,0x30,0x2e,0x2d,0x2b,0x2a,0x28,0x27,0x25,0x24,0x22,0x20,0x1f,0x1d,0x1c,0x1a,0x19,0x17,0x15,0x14,0x12,0x11,0xf,0xe,0xc,0xa,0x9,0x7,0x6,0x4,0x3,0x1,};

int32_t sine(int16_t n) {
	if (n>=1024) {
		n %= 1024;
	}
	if (n < 0) {
		n = -n;
		if (n>=512) {
			return SINLUT[n-512];
		}
		else {
			return -((int32_t) SINLUT[n]);
		}
	}
	else {
		if (n<512) {
			return SINLUT[n];
		}
		else {
			return -((int32_t) SINLUT[n-512]);
		}
	}
}

int32_t cosi(int16_t n) {
	return sine(n+256);
}
