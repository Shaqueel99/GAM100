/*----------------------------------------------------------------------------------
All content © 2022 DigiPen Institute of Technology

FILE NAME:                  zhiyu.c
AUTHOR:                     Chua Zhi Yu <zhiyu.chua@digipen.edu>
CO AUTHOR:                  NIL

----------------------------------------------------------------------------------*/

#include "cprocessing.h"
#include <stdio.h>

// To check if mouse click one of the option
int optionClicked(float top_right_x, float top_right_y, float width, float height, float click_x, float click_y)
{
	float X1 = top_right_x + width;
	float Y1 = top_right_y + height;
	float X2 = top_right_x;
	float Y2 = top_right_y;
	float Xp = click_x;
	float Yp = click_y;

	if (Xp < X1 && Xp > X2 && Yp < Y1 && Yp > Y2) return 1;
	else return 0;
}