#include "../Inc/game.h"
#include "cprocessing.h"
#include <stdlib.h>
#include <stdio.h> //for use of sprintf_s
#include "utils.h"
#include "leaderboard.h"
#include "mainmenu.h"
char playername[3];
char score;
int i = 0;
float windows_length, windows_height;
extern int width, height;
extern float volume;
int submitscore;
static CP_Image deathimage;
static CP_Sound deathsound = NULL;
void Death_Screen_Init()
{
    CP_Sound_StopAll();
    deathsound = CP_Sound_Load("..\\..\\Assets\\Soundeffects\\Dinodeath.wav");
    CP_Sound_PlayAdvanced(deathsound, volume, 1.0f, FALSE, CP_SOUND_GROUP_2);
    submitscore = 1;
    windows_length = (float)width;
    windows_height = (float)height;
    CP_System_SetWindowSize((int)windows_length, (int)windows_height);
    
}

void Death_Screen_Update()
{
    if (submitscore == 1) {
    deathimage = CP_Image_Load("Assets/game_ui/deathscreen1.png");
    CP_Image_Draw(deathimage, (float)width / 2.0f, height / 2.0f, (float)width, (float)height, 255);
    CP_Color myColor = CP_Color_Create(0, 0, 0, 255);
    
    CP_Settings_Fill(myColor);

    CP_Graphics_ClearBackground(CP_Color_Create(255, 0, 0, 255));

    // set the rectangle drawing mode to CENTER
    CP_Settings_RectMode(CP_POSITION_CENTER);

    // draw a rectangle at the center of the screen, half the size of the screen





    CP_Settings_TextSize(width/20.0f);
    if (CP_Input_KeyTriggered(KEY_ENTER)) {
        CP_Engine_SetNextGameStateForced(game_init, game_update, game_exit);
        score = '0'; points = 0;
    }
    if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
        score = '0'; points = 0;
        CP_Engine_SetNextGameStateForced(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
    }
    
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    char namebuffer[16] = { 0 };
    char pointbuffer[16] = { 0 };
    sprintf_s(pointbuffer, _countof(pointbuffer), "%d", points);
    CP_Font_DrawText(pointbuffer, CP_System_GetWindowWidth() / 2.5f, CP_System_GetWindowHeight() / 2.008f);
    CP_Settings_TextSize(width / 25.0f);
    if (i >= 0 && i <= 2) {
        if (nametype(&playername[i]) == 3) { if (i !=2) { ++i; }}
    }
    for (int j = 0; j < 3; j++) { if (CP_Input_KeyTriggered(KEY_BACKSPACE)) { playername[j] = '\0'; i = 0; } }
    sprintf_s(namebuffer, _countof(namebuffer), "%c %c %c", playername[0], playername[1], playername[2]);
    CP_Font_DrawText(namebuffer, CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() / 1.28f);
   
   

        
        CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
        // set the rectangle drawing mode to CENTER
        CP_Settings_RectMode(CP_POSITION_CENTER);
        // draw a rectangle at the center of the screen, half the size of the screen
       // CP_Graphics_DrawRect(CP_System_GetWindowWidth() / 1.97f, CP_System_GetWindowHeight() / 1.10f+15,
       //     CP_System_GetWindowWidth() / 2.2f, CP_System_GetWindowHeight() / 10.0f);
        CP_Settings_TextSize(width / 30.0f);
        CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
       // CP_Font_DrawText("Submit Score", CP_System_GetWindowWidth() / 2.42f, CP_System_GetWindowHeight() / 1.10f + 8);


        float mouseposx = CP_Input_GetMouseX();
        float mouseposy = CP_Input_GetMouseY();
        float area_center_x = CP_System_GetWindowWidth() / 2.2f;
        float area_width = CP_System_GetWindowWidth() / 10.0f;
        float area_center_y = (float)CP_System_GetWindowHeight();
        float area_height = CP_System_GetWindowHeight() / 20.0f;

        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
        {

            if (IsAreaClicked(area_center_x, area_center_y, area_width, area_height, mouseposx, mouseposy) == 1) {
               
                if (playername[0] == '\0') { playername[0] = '?'; }//fallback incase player doesnt input name
                write_leaderboard(playername, points);
                
                submitscore = 0;

            };
        }
    }
    if (submitscore == 0) {

        CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
        deathimage = CP_Image_Load("Assets/game_ui/deathscreen2.png");
        CP_Image_Draw(deathimage, (float)width / 2.0f, height / 2.0f, (float)width, (float)height, 255);
        CP_Settings_TextSize(width / 20.0f);
        if (CP_Input_KeyTriggered(KEY_ENTER)) {
            CP_Engine_SetNextGameStateForced(game_init, game_update, game_exit);
            score = '0'; points = 0;
        }

        CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
        char namebuffer[16] = { 0 };
        char pointbuffer[16] = { 0 };
        sprintf_s(pointbuffer, _countof(pointbuffer), "%d", points);
        CP_Font_DrawText(pointbuffer, CP_System_GetWindowWidth() / 2.5f, CP_System_GetWindowHeight() / 2.008f);
        CP_Settings_TextSize(width / 25.0f);
        if (i >= 0 && i <= 2) {
            if (nametype(&playername[i]) == 3) { if (i != 2) { ++i; } }
        }
        for (int j = 0; j < 3; j++) { if (CP_Input_KeyTriggered(KEY_BACKSPACE)) { playername[j] = '\0'; i = 0; } }
        sprintf_s(namebuffer, _countof(namebuffer), "%c %c %c", playername[0], playername[1], playername[2]);
        CP_Font_DrawText(namebuffer, CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() / 1.28f);

        if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
            score = '0'; points = 0;
            CP_Engine_SetNextGameStateForced(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
            
        }



      
    }
    }


void Death_Screen_Exit()
{
    CP_Image_Free(&deathimage);
    CP_Sound_Free(&deathsound);
}