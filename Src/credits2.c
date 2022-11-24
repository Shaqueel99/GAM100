#include "../Inc/game.h"
#include "mainmenu.h"
#include "cprocessing.h"
#include <stdlib.h>

CP_Color background, innerBox, white;
static CP_Image image_background, logo;


extern int width, height;

struct names {
    float x;
    float y;
};
float rectCenterX, rectCenterY, rectWidth, rectHeight;
float rectTopLeftX, rectTopLeftY;
struct names executives, chris, mich, sam, bauer, mel, duba, deek, jas, mic, ray, pras, erik, ang, benE;
//CP_Sound creditssound = NULL;
void Credits2_Init()
{
    CP_Sound_ResumeAll();
    CP_System_SetWindowSize(width, height);
    //creditssound = CP_Sound_Load("..\\..\\Assets\\Soundeffects\\Dinomenu2.wav");
    //CP_Sound_PlayAdvanced(creditssound, 1.0f, 1.0f, FALSE, CP_SOUND_GROUP_2);
    background = CP_Color_Create(0, 246, 155, 255);
    innerBox = CP_Color_Create(0, 0, 0, 155);
    white = CP_Color_Create(255, 255, 255, 255);

    rectCenterX = width / 2.0f;
    rectCenterY = height * 0.48f;
    rectWidth = width * 0.55f;
    rectHeight = height * 0.70f;

    rectTopLeftX = rectCenterX - rectWidth / 2;
    rectTopLeftY = rectCenterY - rectHeight / 2;

    executives.y = height / 4.9f;
    chris.y = height * 0.26f;
    mich.y = height * 0.30f;
    sam.y = height * 0.34f;
    bauer.y = height * 0.38f;
    mel.y = height * 0.42f;
    duba.y = height * 0.46f;
    deek.y = height * 0.50f;
    jas.y = height * 0.54f;
    mic.y = height * 0.58f;
    ray.y = height * 0.62f;
    pras.y = height * 0.66f;
    erik.y = height * 0.70f;
    ang.y = height * 0.74f;
    benE.y = height * 0.78f;
}

void Credits2_Update()
{
    CP_Color myColor = CP_Color_Create(0, 0, 0, 255);
    // Set the circle color to red
    CP_Settings_Fill(myColor);

    CP_Graphics_ClearBackground(background);
    image_background = CP_Image_Load("Assets/Main_Menu.png");
    logo = CP_Image_Load("Assets/DigiPen_Singapore_WEB_RED.png");
    CP_Image_Draw(image_background, (float)width / 2.0f, height / 2.0f, (float)width, (float)height, 255);
    CP_Image_Draw(logo, (float)width * 0.5f, height * 0.90f, (float)width * 0.6f, (float)height * 0.13f, 255);

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
    CP_Font_DrawText("Esc to main menu", rectTopLeftX * 2.50f, rectTopLeftY * 6.2f);

    CP_Settings_TextSize(45.0f);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Settings_Fill(white);
    CP_Font_DrawText("Executives", rectCenterX, executives.y * 1.08f);


    CP_Settings_TextSize(30.0f);
    CP_Font_DrawText("Christopher Comair", rectCenterX, chris.y);
    CP_Font_DrawText("Michele Comair", rectCenterX, mich.y);
    CP_Font_DrawText("Samir Abou Samra", rectCenterX, sam.y);
    CP_Font_DrawText("John Bauer", rectCenterX, bauer.y);
    CP_Font_DrawText("Melvin Gonsalvez", rectCenterX,mel.y);
    CP_Font_DrawText("Dr. Charles Duba", rectCenterX, duba.y);
    CP_Font_DrawText("Johnny Deek", rectCenterX, deek.y);
    CP_Font_DrawText("Jason Chu", rectCenterX, jas.y);
    CP_Font_DrawText("Michael Gats", rectCenterX, mic.y);
    CP_Font_DrawText("Raymond Yan", rectCenterX, ray.y);
    CP_Font_DrawText("Prasanna Ghali", rectCenterX, pras.y);
    CP_Font_DrawText("Dr. Erik Mohrmann", rectCenterX, erik.y);
    CP_Font_DrawText("Angela Kugler", rectCenterX, ang.y);
    CP_Font_DrawText("Ben Ellinger", rectCenterX, benE.y);



    if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
        CP_Engine_SetNextGameStateForced(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
    }
}

void Credits2_Exit()
{
    //CP_Sound_Free(&creditssound);
    CP_Image_Free(&image_background);
    CP_Image_Free(&logo);
}