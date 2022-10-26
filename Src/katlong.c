#include "cprocessing.h"
#include <stdio.h>

int read_leaderboard(void) {
	FILE* leaderboard;
	fopen_s(&leaderboard, "leaderboard.txt", "r"); //read data from txt, file must exist
	if (leaderboard == NULL) {
		return 0; //return 0 if no file exist yet
	}
	return 0;
	
}
void write_leaderboard(char name, char score) {//name and score is input from main game, when user finish game and is writing new result in
	FILE* leaderboard;
	char new_stats[256];
	name = 'a', score = 'b'; //placeholder for score
	sprintf_s(new_stats, 256, "Name: %c, Score: %c\n", name, score);
	fopen_s(&leaderboard, "leaderboard.txt", "ua"); //append option always writes data at the end of file, file created if dont exist
	if (leaderboard != NULL) {
		fwrite(new_stats, sizeof(char), strlen(new_stats), leaderboard);
		fclose(leaderboard);
	}
	//fwrite(new_stats, sizeof(char), strlen(new_stats), leaderboard);
	//fclose(leaderboard);
}
//swap left and right hand value

//sort top 10 leaderboard value