#include "../Inc/game.h"
#include "cprocessing.h"
#include <stdlib.h>

CP_Color background, innerBox, white;

float rectCenterX, rectCenterY, rectWidth, rectHeight;

struct names {
    float x;
    float y;
};

struct names kitkat, shaq, ben, zy, ck, dx, gerald, pres;

void Credits_Init()
{
	CP_System_SetWindowSize(960,540);

    background = CP_Color_Create(0, 246, 155, 255);
    innerBox = CP_Color_Create(0, 0, 0, 255);
    white = CP_Color_Create(255, 255, 255, 255);
    
    rectCenterX = CP_System_GetDisplayWidth() / 2;
    rectCenterY = CP_System_GetDisplayHeight() / 2;
    rectWidth = CP_System_GetDisplayWidth() * (9 / 10);
    rectHeight = CP_System_GetDisplayHeight() * (7 / 10);
}

void Credits_Update()
{
    CP_Color myColor = CP_Color_Create(0, 0, 0, 255);
    // Set the circle color to red
    CP_Settings_Fill(myColor);

    CP_Graphics_ClearBackground(background);

    CP_Settings_RectMode(CP_POSITION_CENTER);

    // draw a rectangle at the center of the screen, half the size of the screen
    CP_Settings_TextSize(60.0f);
    CP_Settings_Fill(innerBox);
    CP_Graphics_DrawRect(rectCenterX, rectCenterY, rectWidth, rectHeight);
    

    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Settings_Fill(white);
    //CP_Font_DrawText("Yeo Kat Long", )
}

void Credits_Exit()
{

}