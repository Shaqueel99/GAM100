#include "cprocessing.h"
#include "leaderboard.h"
#include "utils.h"


CP_Color black, blue, purple, green, red;
/*
struct stats {
	char name[3];
	int score;
} p1, p2, p3;
*/

char p1_name[4], p2_name[4], p3_name[4]; //3 digit name, last letter is null character
char p1_score[4], p2_score[4], p3_score[4];
float windows_width, windows_height;

void Leaderboard_Init(void)
{

	black = CP_Color_Create(0, 0, 0, 255);
	blue = CP_Color_Create(0, 255, 255, 255);
	purple = CP_Color_Create(76, 0, 153, 255);
	green = CP_Color_Create(144, 238, 144, 255);
	red = CP_Color_Create(255, 0, 0, 255);


	windows_width = 960.0f;
	windows_height = 540.0f;


	CP_System_SetWindowSize(windows_width, windows_height);
	//while (read_leaderboard(p1_name, p1_score) == 0);
	read_leaderboard(p1_name, p1_score, p2_name, p2_score, p3_name, p3_score);
}

void Leaderboard_Update(void)
{
	CP_Graphics_ClearBackground(purple);

	CP_Settings_Stroke(black);
	CP_Graphics_DrawLine(windows_width / 2.0f, windows_height / 8.0f , windows_width / 2.0f, windows_height);
	CP_Graphics_DrawLine(0.0f, windows_height / 8.0f, windows_width, windows_height / 8.0f);



	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_BOTTOM);
	CP_Settings_TextSize(60.0f);
	CP_Settings_Fill(blue);
	CP_Font_DrawText("LEADERBOARD", windows_width / 2.0f, windows_height / 8.0f);

	CP_Settings_TextSize(40.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText(p1_name, windows_width / 2.0f - windows_width / 16.0f, windows_height / 8.0f * 2.0f);
	CP_Font_DrawText(p2_name, windows_width / 2.0f - windows_width / 16.0f, windows_height / 8.0f * 3.0f);
	CP_Font_DrawText(p3_name, windows_width / 2.0f - windows_width / 16.0f, windows_height / 8.0f * 4.0f);

	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText(p1_score, windows_width / 2.0f + windows_width / 16.0f, windows_height / 8.0f * 2.0f);
	CP_Font_DrawText(p2_score, windows_width / 2.0f + windows_width / 16.0f, windows_height / 8.0f * 3.0f);
	CP_Font_DrawText(p3_score, windows_width / 2.0f + windows_width / 16.0f, windows_height / 8.0f * 4.0f);

}

void Leaderboard_Exit(void)
{

}
