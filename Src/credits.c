/*----------------------------------------------------------------------------------
All content © 2022 DigiPen Institute of Technology

FILE NAME:                  credits.c
AUTHOR:                     Ong Chee Keong <o.cheekeong@digipen.edu>
CO AUTHOR:                  Loke Wei Zhi, Benjamin <loke.w@digipen.edu>

*****************************    Iteration           *******************************

CURRENT ITERATION:          Ong Chee Keong <o.cheekeong@digipen.edu>
PREVIOUS ITERATION:         Loke Wei Zhi, Benjamin <loke.w@digipen.edu>

----------------------------------------------------------------------------------*/

#include "../Inc/game.h"
#include "mainmenu.h"
#include "cprocessing.h"
#include <stdlib.h>
#include "utils.h"

CP_Color background, innerBox, white;
static CP_Image image_background, logo, names;


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

    image_background = CP_Image_Load("Assets/Credits.png");
    logo = CP_Image_Load("Assets/DigiPen_Singapore_WEB_RED.png");
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
    CP_Settings_Fill(myColor);

    CP_Graphics_ClearBackground(background);
    CP_Image_Draw(image_background, rectCenterX, rectCenterY * 1.087f, (float)width, (float)height, 255);
    CP_Image_Draw(logo, (float)width * 0.66f, height * 0.82f, (float)width * 0.6f, (float)height * 0.13f, 255);
   
    if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
        CP_Engine_SetNextGameStateForced(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
    }
    if (rect_click(width - height * 0.05f, height - height * 0.05f, height * 0.1f, height * 0.1f,
        CP_Input_GetMouseX(), CP_Input_GetMouseY()) == 1 && CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
    {
        CP_Engine_SetNextGameStateForced(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
    }
    CP_Image_Draw(return_menu_button, width - height * 0.05f, height - height * 0.05f, height * 0.1f, height * 0.1f, 255);
}

void Credits_Exit()
{
    CP_Image_Free(&return_menu_button);
    CP_Sound_Free(&creditssound);
    CP_Image_Free(&image_background);
    CP_Image_Free(&logo);

}