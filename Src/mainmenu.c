#include "../Inc/game.h"
#include "utils.h"
#include "cprocessing.h"
#include <stdlib.h>

// Default window_size settings (540 x 960)
int width = 540;
int height = 960;
static selection;
static main;
static return_true_false;

void Main_Menu_Init()
{
	// Change main to 0 to display startups
	main = 1; 
	return_true_false = 0;
	selection = 0;
	CP_System_SetWindowSize(width, height);
}

void Main_Menu_Update()
{
	// Create basic color and reset background
	CP_Color Blue = CP_Color_Create(0, 0, 255, 255);
	CP_Color White = CP_Color_Create(255, 255, 255, 255);
	CP_Color Black = CP_Color_Create(0, 0, 0, 255);
	CP_Graphics_ClearBackground(Black);

	// Mouse Position
	float click_x = (float)CP_Input_GetMouseWorldX();
	float click_y = (float)CP_Input_GetMouseWorldY();
	
	// UI Positioning
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

	// Startup Screen (Digipen display & team logo)
	if (main == 0) {

	}
	// Main Menu (Logic)
	if (main == 1) {
		if (CP_Input_MouseClicked(MOUSE_BUTTON_LEFT) && selection == 0) {
			return_true_false = optionClicked(start_x, start_y, rectangle_width, rectangle_height, click_x, click_y);
			if (return_true_false == 1) selection = 1;
			return_true_false = optionClicked(leaderboard_x, leaderboard_y, square_side, rectangle_height, click_x, click_y);
			if (return_true_false == 1) selection = 2;
			return_true_false = optionClicked(credit_x, credit_y, rectangle_width, rectangle_height, click_x, click_y);
			if (return_true_false == 1) selection = 3;
			return_true_false = optionClicked(option_x, option_y, square_side, rectangle_height, click_x, click_y);
			if (return_true_false == 1) selection = 4;
			return_true_false = optionClicked(quit_x, quit_y, rectangle_width, rectangle_height, click_x, click_y);
			if (return_true_false == 1) selection = 5;
		}

		//UI Drawing (Box) + Text
		if (selection == 0) {
			CP_Settings_Fill(White);
			CP_Graphics_DrawRect(start_x, start_y, rectangle_width, rectangle_height);
			CP_Graphics_DrawRect(leaderboard_x, leaderboard_y, square_side, rectangle_height);
			CP_Graphics_DrawRect(credit_x, credit_y, rectangle_width, rectangle_height);
			CP_Graphics_DrawRect(option_x, option_y, square_side, rectangle_height);
			CP_Graphics_DrawRect(quit_x, quit_y, rectangle_width, rectangle_height);
		}

		// Begin Game
		if (selection == 1) {
			CP_Engine_SetNextGameState(game_init, game_update, game_exit);
		}

		// Leaderboard
		if (selection == 2) {
			CP_Graphics_DrawRect(width * 0.2, height * 0.4, width * 0.6, height * 0.5);
		}

		// Option
		if (selection == 3) {
			CP_Graphics_DrawRect(width * 0.2, height * 0.4, width * 0.6, height * 0.5);
			// Switch statement to let user choose resolution
		}

		// Credit
		if (selection == 4) {
			CP_Graphics_DrawRect(width * 0.2, height * 0.4, width * 0.6, height * 0.5);
		}

		// Exit
		if (selection == 5) {
			CP_Engine_Terminate();
		}
	}
}

void Main_Menu_Exit()
{

}
