#include <stdio.h>
#include <stdlib.h>
#include "cprocessing.h"
#include "../Inc/deathscreen.h"
#include "leaderboard.h"
#include "utils.h"

#define TRUE 1
#define FALSE 0

int gIsPaused; 
int spawn;
int c;
/* Feel free to declare your own variables here */
CP_Color black, blue, purple, green,red;
float windows_length, windows_height, radius=20.0f;
//float position_left, position_mid, position_right;

int name = 'a'; 
char score = '0';
CP_Vector current_position, left_position, mid_position, right_position;
float currentElapsedTime=0, totalElapsedTime=0;
float value_y, value_x_left, value_x_mid, value_x_right;
float coin_y;
int points;
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

    
    left_position = CP_Vector_Set(windows_length / 6.0, windows_height / 4.0 * 3.0);
    mid_position = CP_Vector_Set(windows_length / 2.0, windows_height / 4.0 * 3.0);
    right_position = CP_Vector_Set(windows_length / 6.0 * 5.0, windows_height / 4.0 * 3.0);


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
  
}

void game_update(void)
{
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

void game_exit(void)
{

}
