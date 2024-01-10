#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "math.h"
#define FIXMUL(a, b) ((a>>8)*(b>>8))

void newline(){
	printf("\n");
}

int main(void)
{
	//Dummy vectors for testing
	vector_t p = {0x00010000, 0x00010000};
	vector_t q = {128<<16, 128<<16};


	uint8_t collision = 0;
	enum gamestate{menu, pause, boss, playing};

	uart_init(1000000);

	void updatePosition(Body *body, Object *objects[], uint8_t numObj){
		int timeStep = 0x00A0;
		vector_t change = multVector(&((*ball).vel), timeStep);
		vector_t newPos = addVectors(&change, &(ball -> pos));

		if(checkCollisions(body -> pos, objects)){
			body -> vel.x = 0;
			body -> vel.y = 0;
		}
		ball -> pos = newPos;
	}

	char checkCollisions(vector_t pos, Object *objects[], uint8_t numObj){
		for(uint8_t i = 0; i < numObj; i++){
			if(circleCollision(pos, objects[i]->pos, objects[i]->mass)){
				return 1;
			}
		}
		return 0;
	}

	char circleCollision(vector_t *p, vector_t *q, int32_t radius){
		return distFIXSquared(p, q) < radius;
	}
	char rectangleCollision(vector_t *p, int32_t xMinBound, int32_t xMaxBound, int32_t yMinBound, int32_t yMaxBound){
		return p -> x > xMinBound && p -> x < xMaxBound && p -> y > yMinBound && p -> y < yMaxBound;
	}

	while(1){}
}
