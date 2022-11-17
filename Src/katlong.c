#include "cprocessing.h"
#include <stdio.h>
#include "leaderboard.h"
int p = 0;


int read_leaderboard(char name[], int score[], int order) {
	FILE* leaderboard;
	char input[256];
	fopen_s(&leaderboard, "..\\..\\Assets\\leaderboard.txt", "r"); //read data from txt, file must exist
	if (leaderboard == NULL) {
		return 1;
	}

	if (fgets(input, 256, leaderboard) == NULL) {
		fclose(leaderboard);
		return 1;
	}
	for (p = 0; p < order; p++) {
		fgets(input, 256, leaderboard);
	}
	if (sscanf_s(input, "Name: %3s  Score: %d\n", name, 4, score) == 0) {
		fclose(leaderboard);
		return 1;
	}

	
	
	fclose(leaderboard);
	return 0;
	
}
void write_leaderboard(char name[], char score) {//name and score is input from main game, when user finish game and is writing new result in
	FILE* leaderboard;
	char new_stats[256];
	sprintf_s(new_stats, 256, "Name: %s  Score: %c\n", name, score);
	fopen_s(&leaderboard, "..\\..\\Assets\\leaderboard.txt", "a"); //append option always writes data at the end of file, file created if dont exist
	if (leaderboard != NULL) {
		  
		fwrite(new_stats, sizeof(char), strlen(new_stats), leaderboard);
		fclose(leaderboard);
	
	}
	
	
}




int sort_leaderboard(int scorek,int scorem,int pos) {
	
	if (scorek < scorem) {
		return pos;
	}

	return -1;

}


int rect_click(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
{
	float rectx1corner = area_center_x - area_width;
	float rectx2corner = area_center_x + area_width;
	float playrecty1corner = area_center_y / 1.10f - area_height;
	float playrecty2corner = area_center_y / 1.10f + area_height;


	if (click_x > (area_center_x - area_width / 2) && click_x < (area_center_x + area_width / 2) && click_y >(area_center_y - area_height / 2) && click_y < (area_center_y + area_height / 2)) {
		return 1; //mouse has to be clicked, and mouse arrow position must be in rectangle
	}
	else {
		return 0;
	}
}





