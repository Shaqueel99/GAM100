#include "../Inc/game.h"
#include "utils.h"
#include "cprocessing.h"
#include "credits.h"
#include <stdlib.h>

// Global Variable
// Default window_size settings (640 x 960)
int width = 640;
int height = 960;
float font_scale;
float speed_scale;
 
// Private Variable
static int selection;
static int main;
static int return_true_false;
static CP_Image logo;
static float totalElapsedTime;
static float fade_in_time;
static float toggle_time;
static int display_option;

// Initial Value that will change
// Change main to 0 to display startups
main = 1;
font_scale = 1.0;
speed_scale = 1.0;
return_true_false = 0;
display_option = 0;
CP_Sound mySound = NULL;

void Main_Menu_Init()
{
	mySound = CP_Sound_Load("..\\..\\Assets\\Soundeffects\\Dinomenu2.wav");
	// Initial Value
	selection = 0;
	totalElapsedTime = 0;
	fade_in_time = 2;
	toggle_time = 0;
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
	float start_y = (float)height * 0.4f;
	float leaderboard_x = (float)width * 0.7f;
	float leaderboard_y = (float)height * 0.4f;
	float credit_x = (float)width * 0.2f;
	float credit_y = (float)height * 0.6f;
	float option_x = (float)width * 0.7f;
	float option_y = (float)height * 0.6f;
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
		float currentElapsedTime = CP_System_GetDt();
		int alpha = 255 * (int)totalElapsedTime / (int)fade_in_time;
		// Fade in and out
		if (toggle_time == 0) totalElapsedTime += currentElapsedTime;
		if (toggle_time == 1) totalElapsedTime -= currentElapsedTime;

		// Drawing the School Logo
		CP_Graphics_ClearBackground(Black);
		logo = CP_Image_Load("Assets/DigiPen_BLACK.png");
		CP_Settings_ImageMode(CP_POSITION_CENTER);
		CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP);
		CP_Image_Draw(logo, width / 2.0f, height / 2.0f, width * 0.9f, height * 0.4f, alpha);

		if (totalElapsedTime > fade_in_time) toggle_time = 1;
		if (totalElapsedTime < 0) main = 1;
	}
	// Main Menu (Logic)
	if (main == 1) {
		if (selection == 0 && CP_Input_MouseClicked()) {
			
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

		if (CP_Input_KeyTriggered(KEY_ESCAPE)) selection = 0;

		//UI Drawing (Box) + Text
		if (selection == 0) {
			CP_Graphics_ClearBackground(green);
			CP_Settings_Fill(White);
			CP_Graphics_DrawRect(start_x, start_y, rectangle_width, rectangle_height);
			CP_Graphics_DrawRect(leaderboard_x, leaderboard_y, square_side, rectangle_height);
			CP_Graphics_DrawRect(credit_x, credit_y, rectangle_width, rectangle_height);
			CP_Graphics_DrawRect(option_x, option_y, square_side, rectangle_height);
			CP_Graphics_DrawRect(quit_x, quit_y, rectangle_width, rectangle_height);



			// Temporary Placeholder Text (Possible to replace with image instead?)
			CP_Settings_Fill(Blue);
			CP_Font_DrawText("Start", start_x + rectangle_width * 0.5f, start_y + rectangle_height * 0.5f);
			CP_Font_DrawText("LB", leaderboard_x + square_side * 0.5f, leaderboard_y + rectangle_height * 0.5f);
			CP_Font_DrawText("Credit", credit_x + rectangle_width * 0.5f, credit_y + rectangle_height * 0.5f);
			CP_Font_DrawText("Set", option_x + square_side * 0.5f, option_y + rectangle_height * 0.5f);
			CP_Font_DrawText("Quit", quit_x + rectangle_width * 0.5f, quit_y + rectangle_height * 0.5f);
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
			CP_Settings_TextSize(50.0f * font_scale);
			CP_Settings_Fill(White);
		}

		// Begin Game
		if (selection == 1) {
			CP_Engine_SetNextGameStateForced(game_init, game_update, game_exit);
		}

		// Leaderboard
		if (selection == 2) {
			CP_Graphics_DrawRect(width * 0.2f, height * 0.4f, width * 0.6f, height * 0.5f);
		}

		// Credit
		if (selection == 3) {
			CP_Engine_SetNextGameStateForced(Credits_Init, Credits_Update, Credits_Exit);
		}

		// Option
		if (selection == 4) {
			CP_Settings_Fill(White);
			CP_Graphics_DrawRect(width * 0.1f, height * 0.4f, width * 0.8f, height * 0.1f);
			CP_Settings_Fill(Blue);
			CP_Settings_TextSize(50.0f * font_scale);
			CP_Font_DrawText("Display Setting", text_Display_x, text_Display_y);
			CP_Settings_Fill(White);
			CP_Graphics_DrawRect(displayPOS_left_arrow_x, displayPOS_y, displayDraw_side_x, displayDraw_y);
			CP_Graphics_DrawRect(displayPOS_center_x, displayPOS_y, displayDraw_center_x, displayDraw_y);
			CP_Graphics_DrawRect(displayPOS_right_arrow_x, displayPOS_y, displayDraw_side_x, displayDraw_y);
			
			// Click to swap resolution
			if (CP_Input_MouseClicked()) {
				CP_Sound_PlayAdvanced(mySound, 1.0f, 1.0f, FALSE, CP_SOUND_GROUP_2);
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
				CP_Font_DrawText("640 x 960", width * 0.5f, height * 0.6f);
				width = 640;
				height = 960;
				font_scale = 1.0;
				speed_scale = 1.0;
				CP_System_SetWindowSize(width, height);
				break;			
			case 1:
				// iPad Size
				CP_Font_DrawText("1024 x 768", width * 0.5f, height * 0.6f);
				width = 1024;
				height = 768;
				font_scale = 0.8f;
				speed_scale = 1.6f;
				CP_System_SetWindowSize(width, height);
				break;
			case 2:
				// iPhone Size				
				CP_Font_DrawText("320 x 480", width * 0.5f, height * 0.6f);
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
}
