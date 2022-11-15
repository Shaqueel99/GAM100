#include "cprocessing.h"
#include "leaderboard.h"
#include "utils.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mainmenu.h"

CP_Color black, blue, purple, green, red;

FILE* leaderboard;
char input[256];

extern int width, height;
char p_name[999][4];//3 digit name, last letter is null character
int p_score[999][4];
float windows_width, windows_height;
int j = 0; 
float sortTime = 0; float totalsortTime = 0;

void Leaderboard_Init(void)
{
	j = 0;
	totalsortTime = 0;
	sortTime = 0;
	fopen_s(&leaderboard, "..\\..\\Assets\\leaderboard.txt", "r");
	black = CP_Color_Create(0, 0, 0, 255);
	blue = CP_Color_Create(0, 255, 255, 255);
	purple = CP_Color_Create(76, 0, 153, 255);
	green = CP_Color_Create(144, 238, 144, 255);
	red = CP_Color_Create(255, 0, 0, 255);


	windows_width = (float)width;
	windows_height = (float)height;
	if (leaderboard != NULL) {
		while (fgets(input, 256, leaderboard) != NULL) {

			read_leaderboard(p_name[j], p_score[j], j);
			
			j++;
		}
		fclose(leaderboard);
	}


	CP_System_SetWindowSize((int)windows_width, (int)windows_height);


	
	
	
}

void Leaderboard_Update(void)
{
	 sortTime = CP_System_GetDt();
	 totalsortTime += sortTime;
	 if (totalsortTime < 0.5f) {
		 for (int s = 0; s < sizeof(p_score) / sizeof(p_score[0]); s++) {
			 if (*p_score[s] >=0) {
				 if (sort_leaderboard(*p_score[s], *p_score[s + 1], s) == s) {
					 int  tempscore = *p_score[s];
					 char tempname[4];

					 strcpy_s(tempname, 4, p_name[s]);
					 strcpy_s(p_name[s], 4, p_name[s + 1]);
					 *p_score[s] = *p_score[s + 1];
					 *p_score[s + 1] = tempscore;
					 strcpy_s(p_name[s + 1], 4, tempname);
				 }
			 }
		 }
	 }
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
	
	for (int s = 0; s < sizeof(p_name) / sizeof(p_name[0]); s++) {
		CP_Font_DrawText(p_name[s], windows_width / 2.0f - windows_width / 16.0f, windows_height / 8.0f * (2.0f+s));
	}
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
	for (int s = 0; s < sizeof(p_score) / sizeof(p_score[0]); s++) {
		char scorebuffer[16] = { 0};
		sprintf_s(scorebuffer, _countof(scorebuffer), "%d", *p_score[s]);
		if (*p_score[s] >= 0 && strcmp(p_name[s],"")) {
			CP_Font_DrawText(scorebuffer, windows_width / 2.0f + windows_width / 16.0f, windows_height / 8.0f * (2.0f + s));
		}
	}
	if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
		
		CP_Engine_SetNextGameStateForced(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}
}

void Leaderboard_Exit(void)
{

}
