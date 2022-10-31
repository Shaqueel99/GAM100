#include "../Inc/game.h"
#include "cprocessing.h"
#include <stdlib.h>

CP_Color background, innerBox, white;

float rectCenterX, rectCenterY, rectWidth, rectHeight;
extern int width, height;

struct names {
    float x;
    float y;
};

struct names kitkat, shaq, ben, zy, ck, dx, gerald, pres, team, instructors, claude;

void Credits_Init()
{
	CP_System_SetWindowSize(width, height);

    background = CP_Color_Create(0, 246, 155, 255);
    innerBox = CP_Color_Create(0, 0, 0, 100);
    white = CP_Color_Create(255, 255, 255, 255);
    
    rectCenterX = CP_System_GetWindowWidth() / 2.0f;
    rectCenterY = CP_System_GetWindowHeight() / 2.0f;
    rectWidth = CP_System_GetWindowWidth() / 1.5f; 
    rectHeight = CP_System_GetWindowHeight() / 1.7f;

    team.y = height / 5.3f;
    kitkat.y = height * 0.25;
    zy.y = height * 0.3;
    shaq.y = height * 0.35;
    ck.y = height * 0.4;
    ben.y = height * 0.45;

    instructors.y = height / 1.9f;
    dx.y = height * 0.57;
    gerald.y = height * 0.62;

    pres.y = CP_System_GetWindowHeight() / 1.4f;
    claude.y = height * 0.76;
}

void Credits_Update()
{
    CP_Color myColor = CP_Color_Create(0, 0, 0, 255);
    // Set the circle color to red
    CP_Settings_Fill(myColor);

    CP_Graphics_ClearBackground(background);

    CP_Settings_RectMode(CP_POSITION_CENTER);

    // draw a rectangle at the center of the screen, half the size of the screen
    CP_Settings_Fill(innerBox);
    CP_Graphics_DrawRect(rectCenterX, rectCenterY, rectWidth, rectHeight);
    

    CP_Settings_TextSize(50.0f);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Settings_Fill(white);
    CP_Font_DrawText("Team", rectCenterX, team.y);
    CP_Font_DrawText("Instructors", rectCenterX, instructors.y);
    CP_Font_DrawText("President", rectCenterX, pres.y);


    CP_Settings_TextSize(35.0f);
    CP_Font_DrawText("Yeo Kat Long", rectCenterX, kitkat.y);
    CP_Font_DrawText("Chua Zhi Yu", rectCenterX, zy.y);
    CP_Font_DrawText("Mohammed Shaqeel", rectCenterX, shaq.y);
    CP_Font_DrawText("Ong Chee Keong", rectCenterX, ck.y);
    CP_Font_DrawText("Benjamin Loke", rectCenterX, ben.y);

    CP_Font_DrawText("Cheng Ding Xiang", rectCenterX, dx.y);
    CP_Font_DrawText("Gerald Yeo", rectCenterX, gerald.y);

    CP_Font_DrawText("Claude Comair", rectCenterX, claude.y);
}

void Credits_Exit()
{

}