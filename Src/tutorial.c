#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "mainmenu.h"

extern int width, height;
static float rectangle_width;
static float rectangle_height;
//static CP_Image image_option_background, image_start, image_dino, image_meat,
//image_log, image_boulder, image_heart, image_double_meat, image_invul;
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
	/*
	image_option_background = CP_Image_Load("Assets/mainmenu_button/option_background.png");
	image_dino = CP_Image_Load("Assets/game_ui/game_dino.png");
	image_meat = CP_Image_Load("Assets/game_ui/game_meat.png");
	image_log = CP_Image_Load("Assets/game_ui/log_tutorial.png");
	image_boulder = CP_Image_Load("Assets/game_ui/game_boulder.png");
	image_heart = CP_Image_Load("Assets/game_ui/game_heart.png");
	image_double_meat = CP_Image_Load("Assets/game_ui/game_double_meat.png");
	image_invul = CP_Image_Load("Assets/game_ui/game_invul.png");
	*/
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
	/*
	//actual tutorial below
	CP_Image_Draw(image_dino, width / 10.0f * 2.0f, height / 10.0f, width * 0.06f, height * 0.05f, 255);
	CP_Font_DrawTextBox("wants to eat", width / 10.0f * 2.5f, height / 40.0f * 4.0f, 500.0f);
	CP_Image_Draw(image_meat, width / 10.0f * 5.5f, height / 10.0f, width * 0.07f, height * 0.04f, 255);
	CP_Font_DrawTextBox("Use WASD to control him!", width / 10.0f * 2.5f, height / 40.0f * 7.0f, 500.0f);
	CP_Font_DrawTextBox("Avoid these obstacles!", width / 10.0f * 2.5f, height / 40.0f * 10.0f, 500.0f);
	CP_Image_Draw(image_log, width / 10.0f, height / 10.0f * 4.5f, width * 0.1f, height * 0.4f, 255);
	CP_Font_DrawTextBox("will insta-kill Dino!", width / 10.0f * 2.5f, height / 40.0f * 18.0f, 500.0f);
	CP_Image_Draw(image_boulder, width / 10.0f * 3.0f, height / 10.0f * 5.5f, width * 0.13f, height * 0.09f, 255);
	CP_Font_DrawTextBox("will deduct a", width / 10.0f * 4.0f, height / 40.0f * 22.0f, 500.0f);
	CP_Image_Draw(image_heart, width / 10.0f * 7.5f, height / 10.0f * 5.5f, width * 0.10f, height * 0.06f, 255);
	CP_Font_DrawTextBox("Powerups below!", width / 10.0f * 2.5f, height / 40.0f * 26.0f, 500.0f);
	CP_Image_Draw(image_double_meat, width / 10.0f * 1.0f, height / 40.0f * 29.0f, width * 0.10f, height * 0.06f, 255);
	CP_Font_DrawTextBox("x3 point gain!", width / 10.0f * 1.8f, height / 40.0f * 29.0f, 500.0f);
	CP_Image_Draw(image_invul, width / 10.0f * 5.0f, height / 40.0f * 29.0f, width * 0.10f, height * 0.06f, 255);
	CP_Font_DrawTextBox("gives 5 sec i-frames!", width / 10.0f * 5.6f, height / 40.0f * 29.0f, 500.0f);
	*/
	if (rect_click(width - height * 0.05f, height - height * 0.05f, height * 0.1f, height * 0.1f,
		CP_Input_GetMouseX(), CP_Input_GetMouseY()) == 1 && CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
	{
		CP_Engine_SetNextGameStateForced(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}
	//CP_Image return_menu_button = CP_Image_Load("Assets/game_ui/return_button.png");
	CP_Image_Draw(return_menu_button, width - height * 0.05f, height - height * 0.05f, height * 0.1f, height * 0.1f, 255);
	//CP_Image_Free(&return_menu_button);
}
void Tutorial_Screen_Exit(void)
{
	//image free
	CP_Image_Free(&tutorial_screen);
	CP_Image_Free(&return_menu_button);
	CP_Image_Free(&image_start);
	CP_Sound_Free(&tutorial_sound);
	/*
	CP_Image_Free(&image_option_background);
	CP_Image_Free(&image_dino);
	CP_Image_Free(&image_meat);
	CP_Image_Free(&image_log);
	CP_Image_Free(&image_boulder);
	CP_Image_Free(&image_heart);
	CP_Image_Free(&image_double_meat);
	CP_Image_Free(&image_invul);
	*/
}