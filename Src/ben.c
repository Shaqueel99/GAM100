/*----------------------------------------------------------------------------------
All content © 2022 DigiPen Institute of Technology

FILE NAME:                  ben.c
AUTHOR:                     Loke Wei Zhi, Benjamin <loke.w@digipen.edu>
CO AUTHOR:                  NIL

----------------------------------------------------------------------------------*/

#include "cprocessing.h"
#include <math.h>
#include "utils.h"
#include "deathscreen.h"

#define TRUE 1
#define FALSE 0

extern int height, width;

float retainTime(float* p) {
	float temp = *p;
	return temp;
}

int areaClick(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y) // FOR PAUSE
{
	float topleftx = area_center_x - area_width / 2;
	float btmrightx = area_center_x + area_width / 2;

	float toplefty = area_center_y - area_height / 2;
	float btmrighty = area_center_y + area_height / 2;

	if (click_x > topleftx && click_x < btmrightx && click_y > toplefty && click_y < btmrighty) {
		return 1;
	}
	else {
		return 0;
	}
}


int isRectCollided(float circle_X, float circle_Y, float radius, float rect_X, float rect_Y, float rectWidth, float rectHeight) {
	float temp_X = circle_X;
	float temp_Y = circle_Y;

	// which side of the rect the circle is coming in from
	if (circle_X < (rect_X-rectWidth/2)) temp_X = rect_X - rectWidth/2; //assigning the temp to the LEFT side of the rectangle
	else if (circle_X > (rect_X + rectWidth/2)) temp_X = rect_X + rectWidth/2; // assign to RIGHT side

	if (circle_Y < (rect_Y - rectHeight/2)) temp_Y = rect_Y - rectHeight/2; //assign to top
	else if (circle_Y > (rect_Y + rectHeight/2)) temp_Y = rect_Y + rectHeight/2; //assign to bottom

	//checking the distance
	float distanceX = circle_X - temp_X;
	float distanceY = circle_Y - temp_Y;
	float distTotal = (distanceX * distanceX) + (distanceY * distanceY);
	

	float radiusSquared = radius * radius;

	if (distTotal <= radiusSquared) {
		return 1;
	}
	else {
		return 0;
	}
}

void spawnCheck(struct obstacles* rock, float timeCheck, float totalTime, float speed) {
	rock->boulder = (totalTime > timeCheck && rock->boulder != 2) ? rock->boulder + 1 : rock->boulder;
	rock->boulder_spawn = (rock->boulder == 1) ? TRUE : rock->boulder_spawn;
	rock->value_y += (rock->boulder_spawn == TRUE) ? speed : 0.0f;
}

void spawnRect(struct rectObstacle* rect, float timeCheck, float totalTime, float speed) {
	rect->rect = (totalTime > timeCheck && rect->rect != 2) ? rect->rect + 1 : rect->rect;
	rect->rectSpawn = (rect->rect == 1) ? TRUE : rect->rectSpawn;
	rect->value_y += (rect->rectSpawn == TRUE) ? speed : 0.0f;
}

void invulSpawn(struct obstacles* invul, float timeCheck, float totalTime, float speed) {
	invul->invul = (totalTime > timeCheck && invul->invul != 2) ? invul->invul+ 1 : invul->invul;
	invul->invul_spawn = (invul->invul == 1) ? TRUE : invul->invul_spawn;
	invul->invul_y += (invul->invul_spawn == TRUE) ? speed : 0.0f;
}

void dblPtsSpawn(struct obstacles* dblpts, float timeCheck, float totalTime, float speed) {
	dblpts->pts_boost = (totalTime > timeCheck && dblpts->pts_boost != 2) ? dblpts->pts_boost + 1 : dblpts->pts_boost;
	dblpts->pts_boost_spawn = (dblpts->pts_boost == 1) ? TRUE : dblpts->pts_boost_spawn;
	dblpts->pts_boost_y += (dblpts->pts_boost_spawn == TRUE) ? speed : 0.0f;
}
