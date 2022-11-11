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
	 //placeholder for score
	sprintf_s(new_stats, 256, "Name: %s  Score: %c\n", name, score);
	fopen_s(&leaderboard, "..\\..\\Assets\\leaderboard.txt", "a"); //append option always writes data at the end of file, file created if dont exist
	if (leaderboard != NULL) {
		fwrite(new_stats, sizeof(char), strlen(new_stats), leaderboard);
		fclose(leaderboard);
	}
	//ToDO: ensure name can be accepted as a string
}




int sort_leaderboard(int scorek,int scorem,int pos) {
	int temp = 0;
	if (scorek < scorem) {
		return pos;
	}

		
	
			

}








