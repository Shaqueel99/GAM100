#include "cprocessing.h"

float retainTime(float* p) {
	float temp = *p;
	return temp;
}

int areaClick(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y) // FOR PAUSE
{
	// TODO
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

void setResSize(float *width, float *height) {
	CP_System_SetWindowSize(*width, *height);
}