#include <stdlib.h>
#include "cprocessing.h"
#include "../Inc/game.h"
#include "utils.h"
#include "credits.h"
#include "leaderboard.h"
#include "tutorial.h"

// Global Variable
// Default window_size settings (640 x 960)
int width = 640;
int height = 960;
float font_scale = 1.0f;
float speed_scale = 1.0f;
float volume = 0.5f;
 
// Private Variable
// Initial Value that will change
// Change main to 0 to display startups
static int main = 0;
static int swap_logo = 0;
static int toggle_time = 0;
static float totalElapsedTime;
static float fade_in_time;

static int delay;
static float totalDelayedTime;
static int selection;
static int return_true_false = 0;
static int display_option = 0;
static int tutorial_toggle = 1;

static CP_Image logo, dinotech_logo;
static CP_Image image_mainmenu, image_start, image_leaderboard, image_credit, image_option, image_quit;
static CP_Image image_option_background, image_display_button, image_display_0, image_display_1, image_display_2, image_arrow_left, image_arrow_right;

CP_Sound mySound = NULL;

void Main_Menu_Init()
{
	mySound = CP_Sound_Load("..\\..\\Assets\\Soundeffects\\Dinomenu2.wav");
	// Initial Value
	selection = 0;
	totalElapsedTime = 0.0f;
	totalDelayedTime = 0.0f;
	fade_in_time = 2.0f;	
	delay = 1;
	CP_System_SetWindowSize(width, height);
}

void Main_Menu_Update()
{
	// Create basic color and reset background
	CP_Color green = CP_Color_Create(144, 238, 144, 255);
	CP_Color Blue = CP_Color_Create(0, 0, 255, 255);
	CP_Color White = CP_Color_Create(255, 255, 255, 255);
	CP_Color Black = CP_Color_Create(0, 0, 0, 255);
	CP_Graphics_ClearBackground(green);

	// Mouse Position
	float click_x = (float)CP_Input_GetMouseWorldX();
	float click_y = (float)CP_Input_GetMouseWorldY();
	
	// UI Positioning (Main Menu)
	float start_x = (float)width * 0.2f;
	float start_y = (float)height * 0.5f;
	float leaderboard_x = (float)width * 0.7f;
	float leaderboard_y = (float)height * 0.5f;
	float credit_x = (float)width * 0.2f;
	float credit_y = (float)height * 0.65f;
	float option_x = (float)width * 0.7f;
	float option_y = (float)height * 0.65f;
	float quit_x = (float)width * 0.3f;
	float quit_y = (float)height * 0.8f;

	float rectangle_width = (float)width * 0.4f;
	float rectangle_height = (float)height * 0.1f;
	float square_side = (float)height * 0.1f;

	// UI Positioning (Option)
	float text_Display_x =  (float)width * 0.5f;
	float text_Display_y = (float)height * 0.45f;

	// Initial X & Y value to draw rectangle
	float displayPOS_left_arrow_x = (float)width * 0.15f;
	float displayPOS_center_x = (float)width * 0.25f;
	float displayPOS_right_arrow_x = (float)width * 0.75f;
	float displayPOS_y = (float)height * 0.55f;

	// Space to draw out the rectangle
	float displayDraw_side_x = (float)width * 0.1f;
	float displayDraw_center_x = (float)width * 0.5f;
	float displayDraw_y = (float)height * 0.1f;	

	// Startup Screen (Digipen display & team logo)
	if (main == 0) {
		// Fade in and out
		float currentElapsedTime = CP_System_GetDt();
		float alpha = 255 * (float)totalElapsedTime / (float)fade_in_time;
		if (toggle_time == 0) totalElapsedTime += currentElapsedTime;
		if (toggle_time == 1) totalElapsedTime -= currentElapsedTime;

		// Drawing the School Logo & Team Logo
		CP_Graphics_ClearBackground(Black);
		logo = CP_Image_Load("Assets/DigiPen_BLACK.png");
		dinotech_logo = CP_Image_Load("Assets/DinoTech_Logo_Black.png");
		CP_Settings_ImageMode(CP_POSITION_CENTER);
		CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP);
		if(swap_logo == 0) CP_Image_Draw(logo, (float)width / 2.0f, (float)height / 2.0f, (float)width * 0.9f, (float)height * 0.4f, (int)alpha);
		else if (swap_logo == 1) CP_Image_Draw(dinotech_logo, (float)width / 2.0f, (float)height / 2.0f, (float)width, (float)height * 0.8f, (int)alpha);
		if (totalElapsedTime > fade_in_time) toggle_time = 1;
		if (totalElapsedTime < 0 && swap_logo == 0) {
			toggle_time = 0;
			swap_logo = 1;
		}
		else if (totalElapsedTime < 0 && swap_logo == 1) main = 1;
	}
	// Main Menu (Logic)
	if (main == 1) {
		// Escape to return back to main_menu
		if (CP_Input_KeyTriggered(KEY_ESCAPE)) selection = 0;
		// Delay timer (Mouse click)
		float currentDelayedTime = CP_System_GetDt();
		if (delay == 1) totalDelayedTime += currentDelayedTime;
		if (totalDelayedTime > 0.3f) delay = 0;

		if (selection == 0 && CP_Input_MouseClicked() && delay == 0) {
			
			return_true_false = optionClicked(start_x, start_y, rectangle_width, rectangle_height, click_x, click_y);

			if (return_true_false == 1) selection = 1;
			return_true_false = optionClicked(leaderboard_x, leaderboard_y, square_side, rectangle_height, click_x, click_y);
			if (return_true_false == 1) selection = 2, CP_Sound_PlayAdvanced(mySound, 1.0f, 1.0f, FALSE, CP_SOUND_GROUP_2);
			return_true_false = optionClicked(credit_x, credit_y, rectangle_width, rectangle_height, click_x, click_y);
			if (return_true_false == 1) { selection = 3;}
			
			return_true_false = optionClicked(option_x, option_y, square_side, rectangle_height, click_x, click_y);
			
			if (return_true_false == 1) selection = 4;
			return_true_false = optionClicked(quit_x, quit_y, rectangle_width, rectangle_height, click_x, click_y);
			if (return_true_false == 1) selection = 5;
		}

		//UI Drawing (Main Menu)
		image_mainmenu = CP_Image_Load("Assets/Main_Menu.png");
		image_start = CP_Image_Load("Assets/mainmenu_button/start_button.png");
		image_credit = CP_Image_Load("Assets/mainmenu_button/credit_button.png");
		image_quit = CP_Image_Load("Assets/mainmenu_button/quit_button.png");
		image_option = CP_Image_Load("Assets/mainmenu_button/setting_button.png");
		image_leaderboard = CP_Image_Load("Assets/mainmenu_button/leaderboard_button.png");

		//UI Drawing (Option)
		image_option_background = CP_Image_Load("Assets/mainmenu_button/option_background.png");
		image_display_button = CP_Image_Load("Assets/mainmenu_button/display_setting_button.png");
		image_display_0 = CP_Image_Load("Assets/mainmenu_button/display_setting_0.png");
		image_display_1 = CP_Image_Load("Assets/mainmenu_button/display_setting_1.png");
		image_display_2 = CP_Image_Load("Assets/mainmenu_button/display_setting_2.png");
		image_arrow_left = CP_Image_Load("Assets/mainmenu_button/display_setting_left.png");
		image_arrow_right = CP_Image_Load("Assets/mainmenu_button/display_setting_right.png");
		
		//UI Drawing (Box)
		if (selection == 0) {
			CP_Graphics_ClearBackground(green);
			CP_Image_Draw(image_mainmenu, (float)width / 2.0f, height / 2.0f, (float)width, (float)height, 255);
			CP_Settings_Fill(White);
			CP_Image_Draw(image_start, (float)width * 0.4f, (float)height * 0.55f, rectangle_width, rectangle_height, 255);
			CP_Image_Draw(image_credit , (float)width * 0.4f, (float)height * 0.7f, rectangle_width, rectangle_height, 255);
			CP_Image_Draw(image_quit, (float)width * 0.5f, (float)height * 0.85f, rectangle_width, rectangle_height, 255);
			CP_Image_Draw(image_option, (float)width * 0.775f, (float)height * 0.7f, square_side, rectangle_height, 255); // Small issue with the scaling 
			CP_Image_Draw(image_leaderboard, (float)width * 0.775f, (float)height * 0.55f, square_side, rectangle_height, 255); // Small issue with the scaling 

			/* ~~~~~~~~~~~~~~~~~~~~~~~ For debugging ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			CP_Graphics_DrawRect(start_x, start_y, rectangle_width, rectangle_height);
			CP_Graphics_DrawRect(leaderboard_x, leaderboard_y, square_side, rectangle_height);
			CP_Graphics_DrawRect(credit_x, credit_y, rectangle_width, rectangle_height);
			CP_Graphics_DrawRect(option_x, option_y, square_side, rectangle_height);
			CP_Graphics_DrawRect(quit_x, quit_y, rectangle_width, rectangle_height); 
			~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		}

		// Begin Game
		if (selection == 1) {
			if (tutorial_toggle == 1) {
				tutorial_toggle = 0;
				CP_Engine_SetNextGameStateForced(Tutorial_Screen_Init, Tutorial_Screen_Update, Tutorial_Screen_Exit);
			}
			else if (tutorial_toggle == 0) {
				CP_Engine_SetNextGameStateForced(game_init, game_update, game_exit);
			}
		}

		// Leaderboard
		if (selection == 2) {
			CP_Engine_SetNextGameStateForced(Leaderboard_Init, Leaderboard_Update, Leaderboard_Exit);
		}

		// Credit
		if (selection == 3) {
			CP_Engine_SetNextGameStateForced(Credits_Init, Credits_Update, Credits_Exit);
		}

		// Option
		if (selection == 4) {
			//CP_Settings_Fill(White);
			//CP_Graphics_DrawRect(width * 0.1f, height * 0.4f, width * 0.8f, height * 0.1f);
			//CP_Settings_Fill(Blue);
			//CP_Settings_TextSize(50.0f * font_scale);
			//CP_Font_DrawText("Display Setting", text_Display_x, text_Display_y);
			CP_Image_Draw(image_option_background, (float)width / 2.0f, (float)height / 2.0f, (float)width, (float)height, 255);
			CP_Image_Draw(image_display_button, text_Display_x, text_Display_y, (float)width * 0.8f, (float)height * 0.1f, 255);
			CP_Settings_Fill(White);
			//CP_Graphics_DrawRect(displayPOS_left_arrow_x, displayPOS_y, displayDraw_side_x, displayDraw_y);
			//CP_Graphics_DrawRect(displayPOS_center_x, displayPOS_y, displayDraw_center_x, displayDraw_y);
			//CP_Graphics_DrawRect(displayPOS_right_arrow_x, displayPOS_y, displayDraw_side_x, displayDraw_y);
			CP_Image_Draw(image_arrow_left, (float)width * 0.2f, (float)height * 0.6f, displayDraw_side_x, displayDraw_y, 255);
			CP_Image_Draw(image_arrow_right, (float)width * 0.8f, (float)height * 0.6f, displayDraw_side_x, displayDraw_y, 255);
			
			//sound option start
			CP_Settings_RectMode(CP_POSITION_CENTER);
			CP_Image_Draw(image_display_button, width * 0.5f, height * 0.20f, (float)width * 0.8f, (float)height * 0.1f, 255);
			CP_Graphics_DrawRect(width * 0.2f, height * 0.3f, width * 0.1f, height * 0.1f);
			CP_Graphics_DrawRect(width * 0.8f, height * 0.3f, width * 0.1f, height * 0.1f);

			if (rect_click(width * 0.2f, height * 0.3f, width * 0.1f, height * 0.1f,
				CP_Input_GetMouseX(), CP_Input_GetMouseY()) == 1 && CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)
				&& volume > 0.0f)
			{//lower volume
				volume -= 0.1f;
			}

			if (rect_click(width * 0.8f, height * 0.3f, width * 0.1f, height * 0.1f,
				CP_Input_GetMouseX(), CP_Input_GetMouseY()) == 1 && CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)
				&& volume < 1.0f)
			{//increase volume
				volume += 0.1f;
			}
			//sound option end




			// Click to swap resolution
			if (CP_Input_MouseClicked()) {
				CP_Sound_PlayAdvanced(mySound, volume, 1.0f, FALSE, CP_SOUND_GROUP_2);
				return_true_false = optionClicked(displayPOS_left_arrow_x, displayPOS_y, displayDraw_side_x, displayDraw_y, click_x, click_y);
				if (return_true_false == 1 && display_option == 0) display_option = 2;
				else if (return_true_false == 1 && display_option == 1) display_option = 0;
				else if (return_true_false == 1 && display_option == 2) display_option = 1;
				return_true_false = optionClicked(displayPOS_right_arrow_x, displayPOS_y, displayDraw_side_x, displayDraw_y, click_x, click_y);
				if (return_true_false == 1 && display_option == 0) display_option = 1;
				else if (return_true_false == 1 && display_option == 1) display_option = 2;
				else if (return_true_false == 1 && display_option == 2) display_option = 0;
			}

			// Switch statement to let user choose resolution
			/* To be added (Click to confirm) */
			CP_Settings_Fill(Blue);
			CP_Settings_TextSize(40.0f * font_scale);
			switch (display_option) {
			case 0:
				// iPhone 4S size
				//CP_Font_DrawText("640 x 960", width * 0.5f, height * 0.6f);
				CP_Image_Draw(image_display_0, (float)width * 0.5f, (float)height * 0.6f, (float)width * 0.5f, (float)height * 0.1f, 255);
				width = 640;
				height = 960;
				font_scale = 1.0;
				speed_scale = 1.0;
				CP_System_SetWindowSize(width, height);
				break;			
			case 1:
				// iPad Size
				//CP_Font_DrawText("1024 x 768", width * 0.5f, height * 0.6f);
				CP_Image_Draw(image_display_1, (float)width * 0.5f, (float)height * 0.6f, (float)width * 0.5f, (float)height * 0.1f, 255);
				width = 1024;
				height = 768;
				font_scale = 0.8f;
				speed_scale = 1.6f;
				CP_System_SetWindowSize(width, height);
				break;
			case 2:
				// iPhone Size				
				//CP_Font_DrawText("320 x 480", width * 0.5f, height * 0.6f);
				CP_Image_Draw(image_display_2, (float)width * 0.5f, (float)height * 0.6f, (float)width * 0.5f, (float)height * 0.1f, 255);
				width = 320;
				height = 480;
				font_scale = 0.5;
				speed_scale = 0.5;
				CP_System_SetWindowSize(width, height);
				break;
			}
		}

		// Exit
		if (selection == 5) CP_Engine_Terminate();
	}
}

void Main_Menu_Exit()
{
	CP_Sound_Free(&mySound);
	CP_Image_Free(&logo);
	CP_Image_Free(&dinotech_logo); 
	CP_Image_Free(&image_mainmenu);
	CP_Image_Free(&image_start);
	CP_Image_Free(&image_credit);
	CP_Image_Free(&image_quit);
	CP_Image_Free(&image_option);
	CP_Image_Free(&image_leaderboard);
	CP_Image_Free(&image_option_background);
	CP_Image_Free(&image_display_button);
	CP_Image_Free(&image_display_0);
	CP_Image_Free(&image_display_1);
	CP_Image_Free(&image_display_2);
	CP_Image_Free(&image_arrow_left);
	CP_Image_Free(&image_arrow_right);
}
