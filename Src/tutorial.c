#include "cprocessing.h"
#include "utils.h"
#include "game.h"

extern int width, height;
static float rectangle_width;
static float rectangle_height;
static CP_Image image_dino, image_meat, image_log, image_boulder, image_heart, image_double_meat, image_invul;

void Tutorial_Screen_Init(void)
{
	CP_System_SetWindowSize(width, height);
	rectangle_width = (float)width * 0.4f;
	rectangle_height = (float)height * 0.1f;
	CP_Settings_TextSize(width / 20.0f);
	//CP_Settings_TextSize(width * height / 20000.0f);

	image_dino = CP_Image_Load("Assets/game_ui/game_dino.png");
	image_meat = CP_Image_Load("Assets/game_ui/game_meat.png");
	image_log = CP_Image_Load("Assets/game_ui/game_log.png");
	image_boulder = CP_Image_Load("Assets/game_ui/game_boulder.png");
	image_heart = CP_Image_Load("Assets/game_ui/game_heart.png");
	image_double_meat = CP_Image_Load("Assets/game_ui/game_double_meat.png");
	image_invul = CP_Image_Load("Assets/game_ui/game_invul.png");

	/*
	image_boulder = CP_Image_Load("Assets/game_ui/game_boulder.png");
    image_dino = CP_Image_Load("Assets/game_ui/game_dino.png");
    image_heart = CP_Image_Load("Assets/game_ui/game_heart.png");
    image_invul = CP_Image_Load("Assets/game_ui/game_invul.png");
    image_meat = CP_Image_Load("Assets/game_ui/game_meat.png");
    image_double_meat = CP_Image_Load("Assets/game_ui/game_double_meat.png");
    image_log = CP_Image_Load("Assets/game_ui/game_log.png");
	*/
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
	//exit tutorial button
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Graphics_DrawRect(width / 2.0f, height / 10.0f * 9.0f, rectangle_width, rectangle_height);

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


	//CP_Image_Draw(image_double_meat, value_x_mid, pts_boost_y, width * 0.07f, height * 0.04f, 255);
	//CP_Image_Draw(image_invul, value_x_mid, invul_y, width * 0.05f, height * 0.04f, 255);
	//CP_Image_Draw(image_heart, width * 0.85f, height * 0.05f, width * 0.07f, height * 0.04f, 255);
	
}
void Tutorial_Screen_Exit(void)
{
	CP_Image_Free(image_dino);
	CP_Image_Free(image_meat);
	CP_Image_Free(image_log);
	CP_Image_Free(image_boulder);
	CP_Image_Free(image_heart);
	CP_Image_Free(image_double_meat);
	CP_Image_Free(image_invul);
}