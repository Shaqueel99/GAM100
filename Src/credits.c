#include "../Inc/game.h"
#include "mainmenu.h"
#include "cprocessing.h"
#include <stdlib.h>
#include "utils.h"

CP_Color background, innerBox, white;
static CP_Image image_background, logo;


extern int width, height;

struct names {
    float x;
    float y;
};
float rectCenterX, rectCenterY, rectWidth, rectHeight;
float rectTopLeftX, rectTopLeftY;
struct names kitkat, shaq, ben, zy, ck, dx, gerald, pres, team, instructors, claude;
CP_Sound creditssound = NULL;
static CP_Image return_menu_button;
void Credits_Init()
{
    return_menu_button = CP_Image_Load("Assets/game_ui/return_button.png");
    CP_Sound_ResumeAll();
	CP_System_SetWindowSize(width, height);
    creditssound = CP_Sound_Load("Assets/Soundeffects/Dinomenu2.wav");
    CP_Sound_PlayAdvanced(creditssound, 1.0f, 1.0f, FALSE, CP_SOUND_GROUP_2);
    background = CP_Color_Create(0, 246, 155, 255);
    innerBox = CP_Color_Create(0, 0, 0, 155);
    white = CP_Color_Create(255, 255, 255, 255);
    
    rectCenterX = width / 2.0f;
    rectCenterY = height * 0.46f;
    rectWidth = width * 0.55f;
    rectHeight = height * 0.67f;

    rectTopLeftX = rectCenterX - rectWidth / 2;
    rectTopLeftY = rectCenterY - rectHeight / 2;

    team.y = height / 5.3f;
    kitkat.y = height * 0.25f;
    zy.y = height * 0.3f;
    shaq.y = height * 0.35f;
    ck.y = height * 0.4f;
    ben.y = height * 0.45f;

    instructors.y = height / 1.9f;
    dx.y = height * 0.57f;
    gerald.y = height * 0.62f;

    pres.y = height / 1.4f;
    claude.y = height * 0.76f;
}

void Credits_Update()
{
    CP_Color myColor = CP_Color_Create(0, 0, 0, 255);
    // Set the circle color to red
    CP_Settings_Fill(myColor);

    CP_Graphics_ClearBackground(background);
    image_background = CP_Image_Load("Assets/Main_Menu.png");
    logo = CP_Image_Load("Assets/DigiPen_Singapore_WEB_RED.png");
    CP_Image_Draw(image_background, (float)width / 2.0f, height / 2.0f, (float)width, (float)height, 255);
    CP_Image_Draw(logo, (float)width * 0.5f, height * 0.88f, (float)width * 0.6f, (float)height * 0.13f, 255);

    CP_Settings_RectMode(CP_POSITION_CENTER);

    // draw a rectangle at the center of the screen, half the size of the screen
    CP_Settings_Fill(innerBox);
    CP_Graphics_DrawRect(rectCenterX, rectCenterY, rectWidth, rectHeight);

    CP_Settings_TextSize(20.0f);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
    CP_Settings_Fill(white);
    CP_Font_DrawText("All content (c) 2021 ", rectTopLeftX * 1.05f, rectTopLeftY);
    CP_Font_DrawText("DigiPen Institute of Technology Singapore,", rectTopLeftX * 1.05f, rectTopLeftY * 1.15f);
    CP_Font_DrawText("all rights reserved.", rectTopLeftX * 1.05f, rectTopLeftY * 1.30f);

    CP_Settings_TextSize(50.0f);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Settings_Fill(white);
    CP_Font_DrawText("Team", rectCenterX, team.y * 1.08f);
    CP_Font_DrawText("Instructors", rectCenterX, instructors.y);
    CP_Font_DrawText("President", rectCenterX, pres.y);


    CP_Settings_TextSize(35.0f);
    CP_Font_DrawText("Yeo Kat Long", rectCenterX, kitkat.y);
    CP_Font_DrawText("Chua Zhi Yu", rectCenterX, zy.y);
    CP_Font_DrawText("Mohammed Shaqeel", rectCenterX, shaq.y);
    CP_Font_DrawText("Ong Chee Keong", rectCenterX, ck.y);
    CP_Font_DrawText("Benjamin Loke", rectCenterX, ben.y);

    CP_Font_DrawText("Cheng Ding Xiang", rectCenterX, dx.y);
    CP_Font_DrawText("Gerald Wong", rectCenterX, gerald.y);

    CP_Font_DrawText("Claude Comair", rectCenterX, claude.y);
    
    if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
        CP_Engine_SetNextGameStateForced(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
    }
    if (rect_click(width - height * 0.05f, height - height * 0.05f, height * 0.1f, height * 0.1f,
        CP_Input_GetMouseX(), CP_Input_GetMouseY()) == 1 && CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
    {
        CP_Engine_SetNextGameStateForced(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
    }
    //CP_Image return_menu_button = CP_Image_Load("Assets/game_ui/return_button.png");
    CP_Image_Draw(return_menu_button, width - height * 0.05f, height - height * 0.05f, height * 0.1f, height * 0.1f, 255);
    //CP_Image_Free(&return_menu_button);
}

void Credits_Exit()
{
    CP_Image_Free(&return_menu_button);
    CP_Sound_Free(&creditssound);
    CP_Image_Free(&image_background);
    CP_Image_Free(&logo);
}