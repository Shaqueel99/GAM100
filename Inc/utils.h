#pragma once


//for katlong
void write_leaderboard(char name[], char score);

struct obstacles {
	int boulder;
	int boulder_spawn;
	float value_y;
	int coins;
	int coin_spawn;
};
void read_leaderboard(char name[], int score[],int order);
int sort_leaderboard(int scorek,int scorem,int pos);
//for katlong


//for benjamin
float retainTime(float*);

int areaClick(float, float, float, float, float, float);

struct rectObstacle {
	float value_y;
	int rect;
	int rectSpawn;
	float width;
	float height;
};

struct rectObstacle rect1, rect2, rect3;

int isRectCollided(float, float, float, float, float, float, float);

void spawnCheck(struct obstacles*, float, float, float);

void spawnRect(struct rectObstacle*, float, float, float);
//for ben


//for shaqeel

int iscirclecollided(float current_positionx, float current_positiony, float value_x, float value_y,float radius,int isgood);
int nametype(char playername[]);
int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);
extern char score;
extern int points;
//for shaqeel


//for zhiyu
int optionClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);
//for zhiyu


//for cheekeong
//---insert function declaration here
//for cheekeong