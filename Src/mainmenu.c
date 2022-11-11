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

void Main_Menu_Init()
{
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
	float start_x = (float)width * 0.2;
	float start_y = (float)height * 0.4;
	float leaderboard_x = (float)width * 0.7;
	float leaderboard_y = (float)height * 0.4;
	float credit_x = (float)width * 0.2;
	float credit_y = (float)height * 0.6;
	float option_x = (float)width * 0.7;
	float option_y = (float)height * 0.6;
	float quit_x = (float)width * 0.3;
	float quit_y = (float)height * 0.8;

	float rectangle_width = (float)width * 0.4;
	float rectangle_height = (float)height * 0.1;
	float square_side = (float)height * 0.1;

	// UI Positioning (Option)
	float text_Display_x =  (float)width * 0.5;
	float text_Display_y = (float)height * 0.45;

	// Initial X & Y value to draw rectangle
	float displayPOS_left_arrow_x = (float)width * 0.15;
	float displayPOS_center_x = (float)width * 0.25;
	float displayPOS_right_arrow_x = (float)width * 0.75;
	float displayPOS_y = (float)height * 0.55;

	// Space to draw out the rectangle
	float displayDraw_side_x = (float)width * 0.1;
	float displayDraw_center_x = (float)width * 0.5;
	float displayDraw_y = (float)height * 0.1;

	// Startup Screen (Digipen display & team logo)
	if (main == 0) {
		float currentElapsedTime = CP_System_GetDt();
		int alpha = 255 * totalElapsedTime / fade_in_time;
		// Fade in and out
		if (toggle_time == 0) totalElapsedTime += currentElapsedTime;
		if (toggle_time == 1) totalElapsedTime -= currentElapsedTime;

		// Drawing the School Logo
		CP_Graphics_ClearBackground(Black);
		logo = CP_Image_Load("Assets/DigiPen_BLACK.png");
		CP_Settings_ImageMode(CP_POSITION_CENTER);
		CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP);
		CP_Image_Draw(logo, width / 2, height / 2, width * 0.9, height * 0.4, alpha);

		if (totalElapsedTime > fade_in_time) toggle_time = 1;
		if (totalElapsedTime < 0) main = 1;
	}
	// Main Menu (Logic)
	if (main == 1) {
		if (CP_Input_MouseClicked(MOUSE_BUTTON_LEFT) && selection == 0) {
			return_true_false = optionClicked(start_x, start_y, rectangle_width, rectangle_height, click_x, click_y);

			if (return_true_false == 1) selection = 1;
			return_true_false = optionClicked(leaderboard_x, leaderboard_y, square_side, rectangle_height, click_x, click_y);
			if (return_true_false == 1) selection = 2;
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
			CP_Font_DrawText("Start", start_x + rectangle_width * 0.5, start_y + rectangle_height * 0.5);
			CP_Font_DrawText("LB", leaderboard_x + square_side * 0.5, leaderboard_y + rectangle_height * 0.5);
			CP_Font_DrawText("Credit", credit_x + rectangle_width * 0.5, credit_y + rectangle_height * 0.5);
			CP_Font_DrawText("Set", option_x + square_side * 0.5, option_y + rectangle_height * 0.5);
			CP_Font_DrawText("Quit", quit_x + rectangle_width * 0.5, quit_y + rectangle_height * 0.5);
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
			CP_Settings_TextSize(50.0f * font_scale);
			CP_Settings_Fill(White);
		}

		// Begin Game
		if (selection == 1) {
			CP_Engine_SetNextGameState(game_init, game_update, game_exit);
		}

		// Leaderboard
		if (selection == 2) {
			CP_Graphics_DrawRect(width * 0.2, height * 0.4, width * 0.6, height * 0.5);
		}

		// Credit
		if (selection == 3) {
			CP_Engine_SetNextGameStateForced(Credits_Init, Credits_Update, Credits_Exit);
		}

		// Option
		if (selection == 4) {
			CP_Settings_Fill(White);
			CP_Graphics_DrawRect(width * 0.1, height * 0.4, width * 0.8, height * 0.1);
			CP_Settings_Fill(Blue);
			CP_Settings_TextSize(50.0f * font_scale);
			CP_Font_DrawText("Display Setting", text_Display_x, text_Display_y);
			CP_Settings_Fill(White);
			CP_Graphics_DrawRect(displayPOS_left_arrow_x, displayPOS_y, displayDraw_side_x, displayDraw_y);
			CP_Graphics_DrawRect(displayPOS_center_x, displayPOS_y, displayDraw_center_x, displayDraw_y);
			CP_Graphics_DrawRect(displayPOS_right_arrow_x, displayPOS_y, displayDraw_side_x, displayDraw_y);

			// Click to swap resolution
			if (CP_Input_MouseClicked(MOUSE_BUTTON_LEFT)) {
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
				CP_Font_DrawText("640 x 960", width * 0.5, height * 0.6);
				width = 640;
				height = 960;
				font_scale = 1.0;
				speed_scale = 1.0;
				CP_System_SetWindowSize(width, height);
				break;			
			case 1:
				// iPad Size
				CP_Font_DrawText("1024 x 768", width * 0.5, height * 0.6);
				width = 1024;
				height = 768;
				font_scale = 0.8;
				speed_scale = 1.6;
				CP_System_SetWindowSize(width, height);
				break;
			case 2:
				// iPhone Size				
				CP_Font_DrawText("320 x 480", width * 0.5, height * 0.6);
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
	//CP_Sound_Free(&mySound);
	CP_Image_Free(&logo);
}
