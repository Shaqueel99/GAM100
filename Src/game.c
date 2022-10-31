#include <stdio.h>
#include "cprocessing.h"
#include "mainmenu.h"
#include "deathscreen.h"
#include "utils.h"
#include <math.h>

#define TRUE 1
#define FALSE 0

int gIsPaused; 

/* Feel free to declare your own variables here */
CP_Color black, blue, purple, green,red, white;
float windows_length, windows_height, radius=20.0f;
//float position_left, position_mid, position_right;


CP_Vector current_position, left_position, mid_position, right_position;
float currentElapsedTime=0, totalElapsedTime=0;
float value_y, value_x_left, value_x_mid, value_x_right;

float resumeWidth, resumeHeight, resumeX, resumeY;
float restartWidth, restartHeight, restartX, restartY;
float b2mmWidth, b2mmHeight, b2mmX, b2mmY;

struct obstacles {
    int boulder;
    int boulder_spawn;
    float value_y;
};

struct obstacles first, second, third;
int movingleft = 0, movingright = 0;

int iscirclecollided(float current_positionx, float current_positiony, float value_x, float value_y) {
    float distancex = current_positionx - value_x;
    distancex = distancex * distancex;
    float distancey = current_positiony - value_y;
    distancey = distancey * distancey;
    float finaldistance = distancex + distancey;
    float distance = sqrt(finaldistance);
    if (finaldistance < (radius * 100.0)) {
        totalElapsedTime = 0;
        CP_Engine_SetNextGameStateForced(Death_Screen_Init, Death_Screen_Update, Death_Screen_Exit);
    }
}

void game_init(void)
{
    windows_length = 960.0f;
    windows_height = 540.0f;
    radius = 20.0f;
    CP_System_SetWindowSize(windows_length, windows_height);


    /* We start unpaused */
    gIsPaused = FALSE;

    /* Initialization of your other variables here */
    black = CP_Color_Create(0, 0, 0, 255);
    blue = CP_Color_Create(0, 255, 255, 255);
    purple = CP_Color_Create(76, 0, 153, 255);
    green = CP_Color_Create(144, 238, 144, 255);
    red = CP_Color_Create(255, 0, 0, 255);
    white = CP_Color_Create(255, 255, 255, 255);

    resumeWidth = windows_length / 4;
    resumeHeight = windows_height / 7.7f;
    resumeX = CP_System_GetDisplayWidth() / 6.0f;
    resumeY = CP_System_GetDisplayHeight() / 6.0f;

    restartWidth = windows_length / 4;
    restartHeight = windows_height / 7.7f;
    restartX = CP_System_GetDisplayWidth() / 3.0f;
    restartY = CP_System_GetDisplayHeight() / 6.0f;

    b2mmWidth = windows_length / 3;
    b2mmHeight = windows_height / 7.7f;
    b2mmX = CP_System_GetDisplayWidth() / 4.0f;
    b2mmY = CP_System_GetDisplayHeight() / 3.0f;

    /*
position_left_x = windows_length / 6.0;
position_left_y = windows_height / 4.0 * 3.0;
position_mid_x = windows_length / 2.0;
position_mid_y = windows_height / 4.0 * 3.0;
position_right_x = windows_length / 6.0 * 5.0;
position_right_y = windows_height / 4.0 * 3.0;
*/

    left_position = CP_Vector_Set(windows_length / 6.0, windows_height / 4.0 * 3.0);
    mid_position = CP_Vector_Set(windows_length / 2.0, windows_height / 4.0 * 3.0);
    right_position = CP_Vector_Set(windows_length / 6.0 * 5.0, windows_height / 4.0 * 3.0);


    current_position = mid_position;

    value_y = -windows_height / 8.0;
    value_x_left = windows_length / 6.0;
    value_x_mid = windows_length / 2.0;
    value_x_right = windows_length / 6.0 * 5.0;



    first.value_y = second.value_y = third.value_y = -windows_height / 12.0;

    first.boulder = 0;
    first.boulder_spawn = 0;
    second.boulder = 0;
    second.boulder_spawn = 0;
    third.boulder = 0;
    third.boulder_spawn = 0;

}

void game_update(void)
{
    if (FALSE == gIsPaused) {

        CP_Color myColor = CP_Color_Create(211, 211, 211, 255);
        CP_Settings_Fill(myColor);
        CP_Graphics_ClearBackground(green);
        CP_Settings_Stroke(black);
        CP_Graphics_DrawLine(windows_length / 3.0, 0.0, windows_length / 3.0, windows_height);
        CP_Graphics_DrawLine(windows_length / 3.0 * 2.0, 0.0, windows_length / 3.0 * 2.0, windows_height);
        if (movingleft == TRUE) {
            if (current_position.x != left_position.x && current_position.x != mid_position.x) { current_position.x -= 20; }
            if (current_position.x == left_position.x && current_position.x == mid_position.x) { movingleft = FALSE; }
        }
        if (movingright == TRUE) {
            if (current_position.x != right_position.x && current_position.x != mid_position.x) { current_position.x += 20; }
            if (current_position.x == right_position.x && current_position.x == mid_position.x) { movingright = FALSE; }
        }
        if (CP_Input_KeyTriggered(KEY_A)) {
            //current_position = (current_position.x == right_position.x) ? mid_position : (current_position.x == mid_position.x) ? left_position : current_position;
            if (current_position.x == mid_position.x || current_position.x == right_position.x) { current_position.x -= 20;   movingleft = TRUE;  movingright = FALSE; }



        }
        else if (CP_Input_KeyTriggered(KEY_D)) {
            //current_position = (current_position.x == left_position.x) ? mid_position : (current_position.x == mid_position.x) ? right_position : current_position;
            if (current_position.x == mid_position.x || current_position.x == left_position.x) { current_position.x += 20;   movingright = TRUE;  movingleft = FALSE; }


        }






        //totalElapsedTime += (gIsPaused == FALSE) ? currentElapsedTime : 0.0f;


        CP_Graphics_DrawCircle(current_position.x, current_position.y, radius * 2.0);
        CP_Graphics_DrawTriangleAdvanced(current_position.x, current_position.y - radius, current_position.x - radius / 2.0f, current_position.y, current_position.x + radius / 2.0f, current_position.y, 0.0f);


        //CP_Graphics_DrawCircle(value_x, value_y, 40.0f);
        if (CP_Input_KeyDown(KEY_Q)) {
            value_y += 5.0f; //this was for testing, can remove
        }


        currentElapsedTime = CP_System_GetDt();
        totalElapsedTime += currentElapsedTime;
        first.boulder = (totalElapsedTime > 3.0 && first.boulder != 2) ? first.boulder + 1 : first.boulder;
        first.boulder_spawn = (first.boulder == 1) ? TRUE : first.boulder_spawn;
        first.value_y += (first.boulder_spawn == TRUE) ? 5.0f : 0.0f;
        CP_Graphics_DrawCircle(value_x_mid, first.value_y, radius * 4.0);

        second.boulder = (totalElapsedTime > 7.0 && second.boulder != 2) ? second.boulder + 1 : second.boulder;
        second.boulder_spawn = (second.boulder == 1) ? TRUE : second.boulder_spawn;
        second.value_y += (second.boulder_spawn == TRUE) ? 5.0f : 0.0f;
        CP_Graphics_DrawCircle(value_x_left, second.value_y, radius * 4.0);

        third.boulder = (totalElapsedTime > 10.0 && third.boulder != 2) ? third.boulder + 1 : third.boulder;
        third.boulder_spawn = (third.boulder == 1) ? TRUE : third.boulder_spawn;
        third.value_y += (third.boulder_spawn == TRUE) ? 5.0f : 0.0f;
        CP_Graphics_DrawCircle(value_x_right, third.value_y, radius * 4.0);

        iscirclecollided(current_position.x, current_position.y, value_x_mid, first.value_y); //placeholders obv
        iscirclecollided(current_position.x, current_position.y, value_x_left, second.value_y);
        iscirclecollided(current_position.x, current_position.y, value_x_right, third.value_y);
    }

    if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
        gIsPaused = !gIsPaused;
    }

    if (gIsPaused == TRUE) {
        float retainedTime = retainTime(&totalElapsedTime);

        CP_Color black = CP_Color_Create(0, 0, 0, 255);
        // Set the circle color to red
        CP_Settings_Fill(black);

        CP_Graphics_ClearBackground(green);

        // set the rectangle drawing mode to CENTER
        CP_Settings_RectMode(CP_POSITION_CENTER);

        // draw a rectangle at the center of the screen, half the size of the screen
        CP_Settings_TextSize(60.0f);
        CP_Graphics_DrawRect(resumeX, resumeY, resumeWidth, resumeHeight); // Resume Button
        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
        CP_Settings_Fill(white);
        CP_Font_DrawText("Resume", resumeX, resumeY);

        CP_Settings_Fill(black);
        CP_Graphics_DrawRect(restartX, restartY, restartWidth, restartHeight); //Restart Button
        CP_Settings_Fill(white);
        CP_Font_DrawText("Restart", restartX, restartY);

        CP_Settings_Fill(black);
        CP_Graphics_DrawRect(b2mmX, b2mmY, b2mmWidth, b2mmHeight);
        CP_Settings_Fill(white);
        CP_Font_DrawText("Main Menu", b2mmX, b2mmY);

        if (CP_Input_MouseClicked(MOUSE_BUTTON_LEFT)) {
            if (areaClick(resumeX, resumeY, resumeWidth, resumeHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
                gIsPaused = FALSE;
            }
            else if (areaClick(restartX, restartY, restartWidth, restartHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
                gIsPaused = FALSE;
                totalElapsedTime = 0;
                currentElapsedTime = 0;
                CP_Engine_SetNextGameStateForced(game_init, game_update, game_update);
            }
            else if (areaClick(b2mmX, b2mmY, b2mmWidth, b2mmHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
                CP_Engine_SetNextGameStateForced(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
            }
        }
/*
        if (areaClick(CP_System_GetDisplayWidth() / 6.0f, CP_System_GetDisplayWidth() / 6.0f, 250.0f, 75.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()) == 1) {
            if (CP_Input_MouseClicked(MOUSE_BUTTON_LEFT)) {
                gIsPaused = FALSE;
            }
        }

        if (areaClick(CP_System_GetDisplayWidth() * 3.0f, CP_System_GetDisplayWidth() / 3.0f, 250.0f, 75.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()) == 1) {
            if (CP_Input_MouseClicked(MOUSE_BUTTON_LEFT)) {
                gIsPaused = FALSE;
                totalElapsedTime = 0;
                currentElapsedTime = 0;
            }
        }

        if (areaClick(CP_System_GetDisplayWidth() / 4.0f, CP_System_GetDisplayWidth() / 3.0f, 60.0f, 30.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()) == 1) {
            if (CP_Input_MouseClicked(MOUSE_BUTTON_LEFT)) {
                CP_Engine_SetNextGameStateForced(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
            }
        } */
    }

   
}

void game_exit(void)
{

}
