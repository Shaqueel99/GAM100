/*----------------------------------------------------------------------------------
All content © 2022 DigiPen Institute of Technology

FILE NAME:                  tutorial.c
AUTHOR:                     Yeo Kat Long <katlong.yeo@digipen.edu>
CO AUTHOR:                  NIL

----------------------------------------------------------------------------------*/

#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "mainmenu.h"

extern int width, height;
static float rectangle_width;
static float rectangle_height;
static CP_Sound tutorial_sound;
static CP_Image tutorial_screen;
static CP_Image return_menu_button;
static CP_Image image_start;
void Tutorial_Screen_Init(void)
{
	tutorial_sound = CP_Sound_Load("Assets/Soundeffects/Dinomenu2.wav");
	CP_Sound_PlayAdvanced(tutorial_sound, 1.0f, 1.0f, FALSE, CP_SOUND_GROUP_2);
	//windows and button size settings
	CP_System_SetWindowSize(width, height);
	rectangle_width = (float)width * 0.4f;
	rectangle_height = (float)height * 0.1f;

	//tutorial text settings
	CP_Settings_TextSize(width / 20.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_BASELINE);

	tutorial_screen = CP_Image_Load("Assets/game_ui/tutorial_screen.png");

	//image load
	return_menu_button = CP_Image_Load("Assets/game_ui/return_button.png");
	image_start = CP_Image_Load("Assets/mainmenu_button/start_button.png");
}
void Tutorial_Screen_Update(void)
{
	//clear and draw background
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	CP_Image_Draw(tutorial_screen, (float)width / 2.0f, (float)height / 2.0f, (float)width, (float)height, 255);


	//exit tutorial button and click logic
	CP_Image_Draw(image_start, width / 2.0f, height / 10.0f * 9.0f, rectangle_width, rectangle_height, 255);
	if (rect_click(width / 2.0f, height / 10.0f * 9.0f, rectangle_width, rectangle_height,
		CP_Input_GetMouseX(), CP_Input_GetMouseY()) == 1 && CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) 
	{
		CP_Engine_SetNextGameStateForced(game_init, game_update, game_exit);
	}
	if (rect_click(width - height * 0.05f, height - height * 0.05f, height * 0.1f, height * 0.1f,
		CP_Input_GetMouseX(), CP_Input_GetMouseY()) == 1 && CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
	{
		CP_Engine_SetNextGameStateForced(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}
	CP_Image_Draw(return_menu_button, width - height * 0.05f, height - height * 0.05f, height * 0.1f, height * 0.1f, 255);
}
void Tutorial_Screen_Exit(void)
{
	//image free
	CP_Image_Free(&tutorial_screen);
	CP_Image_Free(&return_menu_button);
	CP_Image_Free(&image_start);
	CP_Sound_Free(&tutorial_sound);
}