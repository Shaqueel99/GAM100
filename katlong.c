#include "cprocessing.h"
#include <stdio.h>

int read_leaderboard(void) {
	FILE* leaderboard;
	leaderboard = fopen("leaderboard.txt", "r"); //read data from txt, file must exist
	if (leaderboard == NULL) {
		return 0; //return 0 if no file exist yet
	}
	
}
void write_leaderboard(char name, char score) {//name and score is input from main game, when user finish game and is writing new result in
	FILE* leaderboard;
	char new_stats[256];
	name = 'a', score = 'b'; //placeholder for score
	sprintf(new_stats, "Name: %c, Score: %c\n", name, score);
	leaderboard = fopen("leaderboard.txt", "a"); //append option always writes data at the end of file, file created if dont exist
	fwrite(new_stats, sizeof(char), strlen(new_stats), leaderboard);
	fclose(leaderboard);
}
//swap left and right hand value

//sort top 10 leaderboard value