#include <stdio.h>
#include <stdlib.h>
#include "cprocessing.h"
#include "mainmenu.h"
#include "deathscreen.h"
#include "leaderboard.h"
#include "utils.h"
#include <math.h>

#define TRUE 1
#define FALSE 0

extern int width, height;
int gIsPaused; 
int spawn;
int c;
/* Feel free to declare your own variables here */
CP_Color black, blue, purple, green,red, white;
float windows_length, windows_height, radius=20.0f;
//float position_left, position_mid, position_right;

int name = 'a'; 
char score = '0';
CP_Vector current_position, left_position, mid_position, right_position;
float currentElapsedTime=0, totalElapsedTime=0;
float value_y, value_x_left, value_x_mid, value_x_right;

float resumeWidth, resumeHeight, resumeX, resumeY;
float restartWidth, restartHeight, restartX, restartY;
float b2mmWidth, b2mmHeight, b2mmX, b2mmY;

float coin_y;
int points;
int difficulty = 0;
int random = 0;

struct obstacles {
    int boulder;
    int boulder_spawn;
    float value_y;
    int coins;
    int coin_spawn;
};

struct obstacles first, second, third;
int movingleft = 0, movingright = 0;

void game_init(void)
{
    
    spawn = 1;
    // Window_length 960.0f & Window_height 540.0f
    windows_length = width;
    windows_height = height;
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


    
    left_position = CP_Vector_Set(width / 6.0, height / 4.0 * 3.0);
    mid_position = CP_Vector_Set(width / 2.0, height / 4.0 * 3.0);
    right_position = CP_Vector_Set(width / 6.0 * 5.0, height / 4.0 * 3.0);


    current_position = mid_position;

    value_y = -windows_height / 8.0;
    value_x_left = windows_length / 6.0;
    value_x_mid = windows_length / 2.0;
    value_x_right = windows_length / 6.0 * 5.0;



    first.value_y = second.value_y = third.value_y = coin_y= -windows_height / 12.0;

    first.boulder = 0;
    first.coins = 0;
    first.boulder_spawn = 0;
    first.coin_spawn = 0;
    second.boulder = 0;
    second.boulder_spawn = 0;
    third.boulder = 0;
    third.boulder_spawn = 0;

}

void game_update(void)
{
    if (gIsPaused == FALSE) {
        //Displaying background
        CP_Color myColor = CP_Color_Create(211, 211, 211, 255);
        CP_Settings_Fill(myColor);
        CP_Graphics_ClearBackground(green);
        CP_Settings_Stroke(black);
        CP_Graphics_DrawLine(windows_length / 3.0, 0.0, windows_length / 3.0, windows_height);
        CP_Graphics_DrawLine(windows_length / 3.0 * 2.0, 0.0, windows_length / 3.0 * 2.0, windows_height);

        //Movement
        if(movingleft==TRUE){ 
            if(current_position.x != left_position.x && current_position.x!=mid_position.x ){ current_position.x -= 20; }
        if(current_position.x == left_position.x && current_position.x == mid_position.x){ movingleft = FALSE; }
        }
        if (movingright == TRUE) {
            if (current_position.x != right_position.x && current_position.x != mid_position.x) { current_position.x += 20; }
            if (current_position.x == right_position.x && current_position.x == mid_position.x) { movingright = FALSE; }
        }
        if (CP_Input_KeyTriggered(KEY_A)) {
            
            if (current_position.x == mid_position.x || current_position.x == right_position.x) { current_position.x -= 20;   movingleft = TRUE;  movingright = FALSE;}
        }
        else if (CP_Input_KeyTriggered(KEY_D)) {
            if (current_position.x == mid_position.x || current_position.x == left_position.x) { current_position.x += 20;   movingright = TRUE;  movingleft = FALSE; }
        
            
        }
        
        
    

        if (CP_Input_KeyTriggered(KEY_L)) {
            CP_Engine_SetNextGameState(Leaderboard_Init, Leaderboard_Update, Leaderboard_Exit);
        }
        

        
        //Displaying player
        CP_Graphics_DrawCircle(current_position.x, current_position.y, radius * 2.0);
        CP_Graphics_DrawTriangleAdvanced(current_position.x, current_position.y - radius, current_position.x - radius / 2.0f, current_position.y, current_position.x + radius / 2.0f, current_position.y, 0.0f);

        //Spawing Boulders
        currentElapsedTime = CP_System_GetDt();
        totalElapsedTime += currentElapsedTime;




        if (totalElapsedTime > 10.0) {
            totalElapsedTime = 0.0;
            ++difficulty;
        }

        if (0 == difficulty) {
            if (CP_Random_RangeInt(0, 1) == 0) {
                //launch first easy segment
            }
            else {
                //launch second easy segment instead
            }
            //random range is set to 0 to 1
            // if range == 0, launch first easy segment
            // if range == 1, launch second easy segment instead
        }
        else if (1 == difficulty) {
            if (CP_Random_RangeInt(0, 1) == 0) {
                //launch first medium segment
            }
            else {
                //launch second medium segment instead
            }
            //random range is set to 0 to 1
            // if range == 0, launch first medium segment
            // if range == 1, launch second medium segment instead
        }
        else if (2 == difficulty/*skip 3 because hard segment is 20seconds long*/) {
            random = CP_Random_RangeInt(0, 2);
            //random range is set to 0, 1 or 2
            if (0 == random) {
                //launch first hard segment
            }
            else if (1 == random) {
                //launch second hard segment instead
            }
            else {
                //launch third hard segment instead
            }
            // if range == 0, launch first hard segment
            // if range == 1, launch second hard segment instead
            // if range == 2, launch third hard segment instead
        }
        else if (0 == difficulty % 2) {
            random = CP_Random_RangeInt(0, 2);
            //random range is set to 0, 1 or 2
            if (0 == random) {
                //launch first hard segment
            }
            else if (1 == random) {
                //launch second hard segment instead
            }
            else {
                //launch third hard segment instead
            }
            // if range == 0, launch first hard segment
            // if range == 1, launch second hard segment instead
            // if range == 2, launch third hard segment instead
        }




        first.boulder = (totalElapsedTime > 3.0 && first.boulder != 2) ? first.boulder + 1 : first.boulder;
        first.boulder_spawn = (first.boulder == 1) ? TRUE : first.boulder_spawn;
        first.value_y += (first.boulder_spawn == TRUE) ? 5.0f : 0.0f;
        CP_Graphics_DrawCircle(value_x_mid, first.value_y, radius * 4.0);


        second.boulder = (totalElapsedTime > 7.0 && second.boulder != 2) ? second.boulder + 1: second.boulder;
        second.boulder_spawn = (second.boulder == 1) ? TRUE : second.boulder_spawn;
        second.value_y += (second.boulder_spawn == TRUE) ? 5.0f : 0.0f;
        CP_Graphics_DrawCircle(value_x_left, second.value_y, radius * 4.0);

        third.boulder = (totalElapsedTime > 10.0 && third.boulder != 2) ? third.boulder + 1: third.boulder;
        third.boulder_spawn = (third.boulder == 1) ? TRUE : third.boulder_spawn;
        third.value_y += (third.boulder_spawn == TRUE) ? 5.0f : 0.0f;
        CP_Graphics_DrawCircle(value_x_right, third.value_y, radius * 4.0);

        first.coins = (totalElapsedTime > 1.0 && first.coins != 2) ? first.coins + 1 : first.coins;
        first.coin_spawn = (first.coins == 1) ? TRUE : first.coin_spawn;
        coin_y += (first.coin_spawn == TRUE) ? 9.0f : 0.0f;
        if (spawn == 1) {CP_Graphics_DrawCircle(value_x_mid, coin_y, radius * 1.0);}

        //Displaying Points
        CP_Settings_TextSize(20.0f);
        CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
        char buffer[16] = { 0 };
        sprintf_s(buffer, _countof(buffer), "%d", points);
        CP_Font_DrawText(buffer, 80, 20);
        CP_Font_DrawText("Points:", 10, 20);
        
    
    
        if (iscirclecollided(current_position.x, current_position.y, value_x_mid, first.value_y, radius) == 1) { totalElapsedTime = 0; 
        score += points;};
        if (iscirclecollided(current_position.x, current_position.y, value_x_left, second.value_y, radius) == 1) { totalElapsedTime = 0; 
        score += points; };
        if (iscirclecollided(current_position.x, current_position.y, value_x_right, third.value_y, radius) == 1) { totalElapsedTime = 0; 
        score += points;  };

        if (spawn == 1) {
            if (iscoincollided(current_position.x, current_position.y, value_x_mid, coin_y, radius) == 1) {points += 1; spawn = 0;}
        }
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
    }

}

void game_exit(void)
{

}
