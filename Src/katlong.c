#include "cprocessing.h"
#include <stdio.h>
#include "leaderboard.h"


int read_leaderboard(char name1[], char score1[], char name2[], char score2[], char name3[], char score3[]) {
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
	if (sscanf_s(input, "Name: %3s, Score: %3s\n", name1, 4, score1, 4) == 0) {
		fclose(leaderboard);
		return 1;
	}

	if (fgets(input, 256, leaderboard) == NULL) {
		fclose(leaderboard);
		return 1;
	}
	if (sscanf_s(input, "Name: %3s, Score: %3s\n", name2, 4, score2, 4) == 0) {
		fclose(leaderboard);
		return 1;
	}
	
	if (fgets(input, 256, leaderboard) == NULL) {
		fclose(leaderboard);
		return 1;
	}
	if (sscanf_s(input, "Name: %3s, Score: %3s\n", name3, 4, score3, 4) == 0) {
		fclose(leaderboard);
		return 1;
	}
	
	fclose(leaderboard);
	return 0;
	
}
void write_leaderboard(char name, char score) {//name and score is input from main game, when user finish game and is writing new result in
	FILE* leaderboard;
	char new_stats[256];
	name = 'abc', score = '123'; //placeholder for score
	sprintf_s(new_stats, 256, "Name: %c, Score: %i\n", name, score);
	fopen_s(&leaderboard, "..\\..\\Assets\\leaderboard.txt", "a"); //append option always writes data at the end of file, file created if dont exist
	if (leaderboard != NULL) {
		fwrite(new_stats, sizeof(char), strlen(new_stats), leaderboard);
		fclose(leaderboard);
	}
	//ToDO: ensure name can be accepted as a string
}



/*
void sort_leaderboard(char name_1[], char score_1[], char name_2[], char score_2[], char name_3[], char score_3[]) {
	//length is all 4, 3 char + 1 null character
	int score_first, score_second, score_third;
	sscanf_s(score_1, "%d", &score_first);
	sscanf_s(score_2, "%d", &score_second);
	sscanf_s(score_3, "%d", &score_third);

	if (score_second > score_first) {
		;
	}
}
*/








//swap left and right hand value

//sort top 10 leaderboard value