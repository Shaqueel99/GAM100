#include "cprocessing.h"
#include <stdio.h>
#include <utils.h>
#include "../Inc/deathscreen.h"
#include <math.h> //for use of sqrt
int iscirclecollided(float current_positionx, float current_positiony, float value_x, float value_y,float radius,int isgood) {
    float distancex = current_positionx - value_x;
    
    distancex = distancex * distancex;
    float distancey = current_positiony - value_y;
    distancey = distancey * distancey;
    float finaldistance = distancex + distancey;
    float distance = (float)sqrt(finaldistance);
    if (finaldistance < (radius * 100.0) && isgood == 0) {
        CP_Engine_SetNextGameStateForced(Death_Screen_Init, Death_Screen_Update, Death_Screen_Exit);
        return 1;  
    }
    else if (finaldistance < (radius * 100.0) && isgood == 1) {
        return 2;
    }
    return 0; //to avoid 'not all control paths return value' warning
}



int nametype(char playername[]) {
    
    if (CP_Input_KeyTriggered(KEY_A)) { *playername = 'A'; return 3; }
    if (CP_Input_KeyTriggered(KEY_B)) { *playername = 'B'; return 3; }
    if (CP_Input_KeyTriggered(KEY_C)) { *playername = 'C'; return 3; }
    if (CP_Input_KeyTriggered(KEY_D)) { *playername = 'D'; return 3; }
    if (CP_Input_KeyTriggered(KEY_E)) { *playername = 'E'; return 3; }
    if (CP_Input_KeyTriggered(KEY_F)) { *playername = 'F'; return 3; }
    if (CP_Input_KeyTriggered(KEY_G)) { *playername = 'G'; return 3; }
    if (CP_Input_KeyTriggered(KEY_H)) { *playername = 'H'; return 3; }
    if (CP_Input_KeyTriggered(KEY_I)) { *playername = 'I'; return 3; }
    if (CP_Input_KeyTriggered(KEY_J)) { *playername = 'J'; return 3; }
    if (CP_Input_KeyTriggered(KEY_K)) { *playername = 'K'; return 3; }
    if (CP_Input_KeyTriggered(KEY_L)) { *playername = 'L'; return 3; }
    if (CP_Input_KeyTriggered(KEY_M)) { *playername = 'M'; return 3; }
    if (CP_Input_KeyTriggered(KEY_N)) { *playername = 'N'; return 3; }
    if (CP_Input_KeyTriggered(KEY_O)) { *playername = 'O'; return 3; }
    if (CP_Input_KeyTriggered(KEY_P)) { *playername = 'P'; return 3; }
    if (CP_Input_KeyTriggered(KEY_Q)) { *playername = 'Q'; return 3; }
    if (CP_Input_KeyTriggered(KEY_R)) { *playername = 'R'; return 3; }
    if (CP_Input_KeyTriggered(KEY_S)) { *playername = 'S'; return 3; }
    if (CP_Input_KeyTriggered(KEY_T)) { *playername = 'T'; return 3; }
    if (CP_Input_KeyTriggered(KEY_U)) { *playername = 'U'; return 3; }
    if (CP_Input_KeyTriggered(KEY_V)) { *playername = 'V'; return 3; }
    if (CP_Input_KeyTriggered(KEY_W)) { *playername = 'W'; return 3; }
    if (CP_Input_KeyTriggered(KEY_X)) { *playername = 'X'; return 3; }
    if (CP_Input_KeyTriggered(KEY_Y)) { *playername = 'Y'; return 3; }
    if (CP_Input_KeyTriggered(KEY_Z)) { *playername = 'Z'; return 3; }
  
    return 0; //to avoid 'not all control paths return value' warning

}
int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
{
    float rectx1corner = area_center_x - area_width;
    float rectx2corner = area_center_x + area_width;
    float playrecty1corner = area_center_y / 1.10f - area_height;
    float playrecty2corner = area_center_y / 1.10f + area_height;
    

    if (click_x < rectx2corner && click_x > rectx1corner && click_y < playrecty2corner && click_y > playrecty1corner) {
        return 1;
    }




    return 0;
}
