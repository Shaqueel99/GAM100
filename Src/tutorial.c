#include "cprocessing.h"
#include "utils.h"
#include "game.h"

extern int width, height;
static float rectangle_width;
static float rectangle_height;

void Tutorial_Screen_Init(void)
{
	CP_System_SetWindowSize(width, height);
	rectangle_width = (float)width * 0.4f;
	rectangle_height = (float)height * 0.1f;
}
void Tutorial_Screen_Update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	//next button to go actual game
	if (rect_click(width / 2.0f, height / 10.0f * 9.0f, rectangle_width, rectangle_height, CP_Input_GetMouseX(), CP_Input_GetMouseY()) == 1
		&& CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) 
	{
		CP_Engine_SetNextGameStateForced(game_init, game_update, game_exit);
	}
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Graphics_DrawRect(width / 2.0f, height / 10.0f * 9.0f, rectangle_width, rectangle_height);
	
}
void Tutorial_Screen_Exit(void) {

}