#include "../Inc/game.h"
#include "cprocessing.h"
#include <stdlib.h>

void Death_Screen_Init()
{
	CP_System_SetWindowSize(960,540);
    
}

void Death_Screen_Update()
{
    CP_Color myColor = CP_Color_Create(0, 0, 0, 255);
    // Set the circle color to red
    CP_Settings_Fill(myColor);

    CP_Graphics_ClearBackground(CP_Color_Create(255, 0, 0, 255));

    // set the rectangle drawing mode to CENTER
    CP_Settings_RectMode(CP_POSITION_CENTER);

    // draw a rectangle at the center of the screen, half the size of the screen



    
    CP_Settings_TextSize(100.0f);
    CP_Font_DrawText("YOU WENT EXTINCT!", CP_System_GetWindowWidth()/10.0f, CP_System_GetWindowHeight() / 2.0f );
    CP_Settings_TextSize(50.0f);
    CP_Font_DrawText("Press Q to play again", CP_System_GetWindowWidth() / 3.5f, CP_System_GetWindowHeight() / 1.5f);
    if (CP_Input_KeyTriggered(KEY_Q)) {
        CP_Engine_SetNextGameStateForced(game_init, game_update, game_exit);
    }
   
   


}

void Death_Screen_Exit()
{

}