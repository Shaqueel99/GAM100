#include "cprocessing.h"
#include "leaderboard.h"
#include "utils.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mainmenu.h"

CP_Color black,white, purple, green, red;

FILE* leaderboard;
char input[256];
static CP_Image leaderboardimage;
static CP_Image loading;
extern int width, height;
char p_name[999][4];//3 digit name, last letter is null character
int p_score[999][4];
float windows_width, windows_height;
int j = 0; 
float sortTime = 0; float totalsortTime = 0;
CP_Sound leaderboardssound = NULL;
void Leaderboard_Init(void)
{
	CP_Sound_ResumeAll();
	leaderboardssound = CP_Sound_Load("..\\..\\Assets\\Soundeffects\\Dinomenu2.wav");
	CP_Sound_PlayAdvanced(leaderboardssound, 1.0f, 1.0f, FALSE, CP_SOUND_GROUP_2);
	j = 0;
	totalsortTime = 0;
	sortTime = 0;
	fopen_s(&leaderboard, "..\\..\\Assets\\leaderboard.txt", "r");
	black = CP_Color_Create(0, 0, 0, 255);
	white = CP_Color_Create(255, 255, 255, 255);
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
	 if (totalsortTime < 0.7f) {
		 loading = CP_Image_Load("Assets/game_ui/loading.png");
		 CP_Image_Draw(loading, (float)width / 2.0f, height / 2.0f, (float)width, (float)height, 255);
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
	

	
	
	 else {
		 leaderboardimage = CP_Image_Load("Assets/game_ui/leaderboard.png");
		 CP_Image_Draw(leaderboardimage, (float)width / 2.0f, height / 2.0f, (float)width, (float)height, 255);
		 CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_BOTTOM);
		 CP_Settings_TextSize(60.0f);
		 CP_Settings_Fill(white);


		 CP_Settings_TextSize(40.0f);
		 CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_MIDDLE);

		 for (int s = 0; s < sizeof(p_name) / sizeof(p_name[0]); s++) {
			 CP_Font_DrawText(p_name[s], windows_width / 2.0f - windows_width / 16.0f, windows_height / 8.0f * (2.0f + s));
		 }
		 CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
		 for (int s = 0; s < sizeof(p_score) / sizeof(p_score[0]); s++) {
			 char scorebuffer[16] = { 0 };
			 sprintf_s(scorebuffer, _countof(scorebuffer), "%d", *p_score[s]);
			 if (*p_score[s] >= 0 && strcmp(p_name[s], "")) {
				 CP_Font_DrawText(scorebuffer, windows_width / 2.0f + windows_width / 16.0f, windows_height / 8.0f * (2.0f + s));
			 }
		 }
		 if (CP_Input_KeyTriggered(KEY_ESCAPE)) {

			 CP_Engine_SetNextGameStateForced(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		 }
	 }
}

void Leaderboard_Exit(void)
{
	CP_Image_Free(&leaderboardimage);
	CP_Image_Free(&loading);
	CP_Sound_Free(&leaderboardssound);
}
