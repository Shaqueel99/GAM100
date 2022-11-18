#include "cprocessing.h"
#include "utils.h"
#include "game.h"

extern int width, height;
static float rectangle_width;
static float rectangle_height;
static CP_Image image_option_background, image_start, image_dino, image_meat,
image_log, image_boulder, image_heart, image_double_meat, image_invul;
CP_Sound tutorialsound = NULL;
void Tutorial_Screen_Init(void)
{
	tutorialsound = CP_Sound_Load("..\\..\\Assets\\Soundeffects\\Dinomenu2.wav");
	CP_Sound_PlayAdvanced(tutorialsound, 1.0f, 1.0f, FALSE, CP_SOUND_GROUP_2);
	//windows and button size settings
	CP_System_SetWindowSize(width, height);
	rectangle_width = (float)width * 0.4f;
	rectangle_height = (float)height * 0.1f;

	//tutorial text settings
	CP_Settings_TextSize(width / 20.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_BASELINE);

	//image load
	image_option_background = CP_Image_Load("Assets/mainmenu_button/option_background.png");
	image_start = CP_Image_Load("Assets/mainmenu_button/start_button.png");
	image_dino = CP_Image_Load("Assets/game_ui/game_dino.png");
	image_meat = CP_Image_Load("Assets/game_ui/game_meat.png");
	image_log = CP_Image_Load("Assets/game_ui/log_tutorial.png");
	image_boulder = CP_Image_Load("Assets/game_ui/game_boulder.png");
	image_heart = CP_Image_Load("Assets/game_ui/game_heart.png");
	image_double_meat = CP_Image_Load("Assets/game_ui/game_double_meat.png");
	image_invul = CP_Image_Load("Assets/game_ui/game_invul.png");
}
void Tutorial_Screen_Update(void)
{
	//clear and draw background
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	CP_Image_Draw(image_option_background, (float)width / 2.0f, (float)height / 2.0f, (float)width, (float)height, 255);


	//exit tutorial button and click logic
	CP_Image_Draw(image_start, width / 2.0f, height / 10.0f * 8.5f, rectangle_width, rectangle_height, 255);
	if (rect_click(width / 2.0f, height / 10.0f * 8.5f, rectangle_width, rectangle_height,
		CP_Input_GetMouseX(), CP_Input_GetMouseY()) == 1 && CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) 
	{
		CP_Engine_SetNextGameStateForced(game_init, game_update, game_exit);
	}

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
}
void Tutorial_Screen_Exit(void)
{
	//image free
	CP_Sound_Free(&tutorialsound);
	CP_Image_Free(&image_option_background);
	CP_Image_Free(&image_start);
	CP_Image_Free(&image_dino);
	CP_Image_Free(&image_meat);
	CP_Image_Free(&image_log);
	CP_Image_Free(&image_boulder);
	CP_Image_Free(&image_heart);
	CP_Image_Free(&image_double_meat);
	CP_Image_Free(&image_invul);
}