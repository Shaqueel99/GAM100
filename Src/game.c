#include <stdio.h>
#include <stdlib.h>
#include "cprocessing.h"
#include "mainmenu.h"
#include "deathscreen.h"
#include "leaderboard.h"
#include "utils.h"
#include <math.h>
#include "game.h"
#define TRUE 1
#define FALSE 0

extern int width, height;
extern float speed_scale;
int gIsPaused; 
int spawn, spawn_ptsboost, spawn_invul;
int c;
int selection;
int checker;
/* Feel free to declare your own variables here */
CP_Color black, blue, purple, green,red, white, pause;
float windows_length, windows_height, radius=20.0f;
//float position_left, position_mid, position_right;

int name = 'a'; 
char score = '0';
CP_Vector current_position, left_position, mid_position, right_position;
float currentElapsedTime=0, totalElapsedTime=0;
float value_y, value_x_left, value_x_mid, value_x_right;

float resumeWidth, resumeHeight, resumeX, resumeY;
float restartWidth, restartHeight, restartX, restartY;
float b2mmWidth, b2mmHeight, b2mmX, b2mmY;

float coin_y, pts_boost_y, invul_y;
int points;
int difficulty = 0;
int random = 0;
int current_pts_increase, invulnerable;
float pts_increase_timer, invulnerable_timer;
int multiplier;

static int health;
static int health_toggle;
static int just_got_hit;
static float just_got_hit_timer;


struct obstacles first, second, third, fourth, fifth, sixth, seventh, eighth, ninth, ten;
struct obstacles eleven, twelve, thirt, fourt, fifte, sixte, sevente, eighte, ninete, twoZero;
int movingleft = 0, movingright = 0;
static CP_Sound Startsound = NULL;
static CP_Sound shiftsound = NULL;
static CP_Sound runsound = NULL;
static CP_Sound damagesound = NULL;

void game_init(void)

{
    damagesound = CP_Sound_Load("..\\..\\Assets\\Soundeffects\\Dinodamage.wav");
    shiftsound = CP_Sound_Load("..\\..\\Assets\\Soundeffects\\DinoShifttrim.wav");
    runsound = CP_Sound_Load("..\\..\\Assets\\Soundeffects\\Dinorunningtrim.wav");
    CP_Sound_PlayAdvanced(runsound, 0.5f, 0.6f, TRUE, CP_SOUND_GROUP_2);
    int startsnd=CP_Random_RangeInt(1, 2);
    if (startsnd == 1) {
        Startsound = CP_Sound_Load("..\\..\\Assets\\Soundeffects\\DinoStart2.wav");
        CP_Sound_PlayAdvanced(Startsound, 0.7f, 1.0f, FALSE, CP_SOUND_GROUP_2);
    }
    else {
        Startsound = CP_Sound_Load("..\\..\\Assets\\Soundeffects\\DinoStart1.wav");
        CP_Sound_PlayAdvanced(Startsound, 0.7f, 1.0f, FALSE, CP_SOUND_GROUP_2);
    }

    health = 3;
    health_toggle = 0;
    just_got_hit = 0;
    just_got_hit_timer = 0.0;

    multiplier = 1; //default pts multiplier is 1
    current_pts_increase = 0, invulnerable = 0;
    pts_increase_timer = invulnerable_timer = 0.0f;
    spawn = 1, spawn_ptsboost = 1, spawn_invul = 1;
    // Window_length 960.0f & Window_height 540.0f
    windows_length = width;
    windows_height = height;
    radius = 20.0f;
    CP_System_SetWindowSize(windows_length, windows_height);


    /* We start unpaused */
    gIsPaused = FALSE;

    /* Initialization of your other variables here */
    black = CP_Color_Create(0, 0, 0, 255);
    blue = CP_Color_Create(0, 255, 255, 255);
    purple = CP_Color_Create(76, 0, 153, 255);
    green = CP_Color_Create(144, 238, 144, 255);
    red = CP_Color_Create(255, 0, 0, 255);
    white = CP_Color_Create(255, 255, 255, 255);
    pause = CP_Color_Create(0, 0, 0, 100);

    resumeWidth = width * 0.35;
    resumeHeight = height * 0.1;
    resumeX = CP_System_GetWindowWidth() * 0.3f;
    resumeY = CP_System_GetWindowHeight() * 0.45f;

    restartWidth = width * 0.35;
    restartHeight = height * 0.1;
    restartX = CP_System_GetWindowWidth() * 0.7f;
    restartY = CP_System_GetWindowHeight() * 0.45f;

    b2mmWidth = width * 0.5;
    b2mmHeight = height * 0.1;
    b2mmX = CP_System_GetWindowWidth() * 0.5f;
    b2mmY = CP_System_GetWindowHeight() * 0.6f;

    /*
position_left_x = windows_length / 6.0;
position_left_y = windows_height / 4.0 * 3.0;
position_mid_x = windows_length / 2.0;
position_mid_y = windows_height / 4.0 * 3.0;
position_right_x = windows_length / 6.0 * 5.0;
position_right_y = windows_height / 4.0 * 3.0;
*/


    
    left_position = CP_Vector_Set(width * 0.16, height / 4.0 * 3.0);
    mid_position = CP_Vector_Set(width / 2.0, height / 4.0 * 3.0);
    right_position = CP_Vector_Set(width * 0.83, height / 4.0 * 3.0);


    current_position = mid_position;

    // Boulder spawn postion
    value_y = -windows_height / 8.0;
    value_x_left = windows_length / 6.0;
    value_x_mid = windows_length / 2.0;
    value_x_right = windows_length / 6.0 * 5.0;



    first.value_y = second.value_y = third.value_y = coin_y= pts_boost_y = invul_y = -windows_height / 12.0;
    fourth.value_y = fifth.value_y = sixth.value_y = seventh.value_y = -height / 12.0;
    eighth.value_y = ninth.value_y = ten.value_y = -height / 12.0;
    eleven.value_y = twelve.value_y = thirt.value_y = fourt.value_y = fifte.value_y = -height / 12.0;
    sixte.value_y = sevente.value_y = eighte.value_y = ninete.value_y = -height / 12.0;
    rect1.value_y = rect2.value_y = rect3.value_y = -height / 3.0;

    first.boulder = 0;
    first.coins = 0;
    first.pts_boost = 0;
    first.invul = 0;
    first.boulder_spawn = 0;
    first.coin_spawn = 0;
    first.pts_boost_spawn = 0;
    first.invul_spawn = 0;
    second.boulder = 0;
    second.boulder_spawn = 0;
    third.boulder = 0;
    third.boulder_spawn = 0;
    fourth.boulder = 0;
    fourth.boulder_spawn = 0;
    fifth.boulder = 0;
    fifth.boulder_spawn = 0;
    sixth.boulder = 0;
    sixth.boulder_spawn = 0;
    seventh.boulder = 0;
    seventh.boulder_spawn = 0;
    eighth.boulder = 0;
    eighth.boulder_spawn = 0;
    ninth.boulder = 0;
    ninth.boulder_spawn = 0;
    ten.boulder = 0;
    ten.boulder_spawn = 0;

    rect1.rect = 0;
    rect1.rectSpawn = 0;
    rect2.rect = 0;
    rect2.rectSpawn = 0;
    rect3.rect = 0;
    rect3.rectSpawn = 0;

    selection = 0;
    checker = -1;
}

void game_update(void)
{
    if (gIsPaused == FALSE) {
        //Displaying background
        CP_Color myColor = CP_Color_Create(211, 211, 211, 255);
        CP_Settings_Fill(myColor);
        CP_Graphics_ClearBackground(green);
        CP_Settings_Stroke(black);
        CP_Graphics_DrawLine(windows_length / 3.0, 0.0, windows_length / 3.0, windows_height);
        CP_Graphics_DrawLine(windows_length / 3.0 * 2.0, 0.0, windows_length / 3.0 * 2.0, windows_height);

        //Movement
        
        if (movingleft == TRUE) {
            if (current_position.x != left_position.x && current_position.x != mid_position.x) { current_position.x -= (25 * speed_scale); }
            if (current_position.x <= left_position.x || current_position.x == mid_position.x) { movingleft = FALSE; }
        }
        if (movingright == TRUE) {
            if (current_position.x != right_position.x && current_position.x != mid_position.x) { current_position.x += (25 * speed_scale); }
            if (current_position.x >= right_position.x || current_position.x == mid_position.x) { movingright = FALSE; }
        }
        if (CP_Input_KeyTriggered(KEY_A)) {
            CP_Sound_PlayAdvanced(shiftsound, 0.5f, 1.0f, FALSE, CP_SOUND_GROUP_2);
            if (current_position.x == mid_position.x || current_position.x >= right_position.x) { current_position.x -= (25 * speed_scale);   movingleft = TRUE;  movingright = FALSE; }
        }
        else if (CP_Input_KeyTriggered(KEY_D)) {
            CP_Sound_PlayAdvanced(shiftsound, 0.5f, 1.0f, FALSE, CP_SOUND_GROUP_2);
            if (current_position.x == mid_position.x || current_position.x <= left_position.x) { current_position.x += (25 * speed_scale);   movingright = TRUE;  movingleft = FALSE; }


        }




        if (CP_Input_KeyTriggered(KEY_L)) {
            CP_Engine_SetNextGameState(Leaderboard_Init, Leaderboard_Update, Leaderboard_Exit);
        }



        //Displaying player
        CP_Graphics_DrawCircle(current_position.x, current_position.y, radius * 2.0);
        CP_Graphics_DrawTriangleAdvanced(current_position.x, current_position.y - radius, current_position.x - radius / 2.0f, current_position.y, current_position.x + radius / 2.0f, current_position.y, 0.0f);

        //Spawing Boulders
        currentElapsedTime = CP_System_GetDt();
        totalElapsedTime += currentElapsedTime;




        if (totalElapsedTime > 20.0) {
            totalElapsedTime = 0.0;
            ++difficulty;

            //resets all parameters
            first.value_y = second.value_y = third.value_y = -windows_height / 12.0;
            fourth.value_y = fifth.value_y = sixth.value_y = seventh.value_y = -height / 12.0;
            eighth.value_y = ninth.value_y = ten.value_y = -height / 12.0;
            eleven.value_y = twelve.value_y = thirt.value_y = fourt.value_y = fifte.value_y = -height / 12.0;
            sixte.value_y = sevente.value_y = eighte.value_y = ninete.value_y = -height / 12.0;

            rect1.value_y = rect2.value_y = rect3.value_y = -height / 4.0;

            first.boulder = 0;
            first.coins = 0;
            first.boulder_spawn = 0;
            first.coin_spawn = 0;
            second.boulder = 0;
            second.boulder_spawn = 0;
            third.boulder = 0;
            third.boulder_spawn = 0;
            fourth.boulder = 0;
            fourth.boulder_spawn = 0;
            fifth.boulder = 0;
            fifth.boulder_spawn = 0;
            sixth.boulder = 0;
            sixth.boulder_spawn = 0;
            seventh.boulder = 0;
            seventh.boulder_spawn = 0;
            eighth.boulder = 0;
            eighth.boulder_spawn = 0;
            ninth.boulder = 0;
            ninth.boulder_spawn = 0;
            ten.boulder = 0;
            ten.boulder_spawn = 0;

            rect1.rect = 0;
            rect1.rectSpawn = 0;
            rect2.rect = 0;
            rect2.rectSpawn = 0;
            rect3.rect = 0;
            rect3.rectSpawn = 0;

            selection = 0;
            checker = -1;

        }



        if (0 == difficulty) {
 
            checker = CP_Random_RangeInt(0, 1);
            selection = (checker == 0 && selection == 0) ? selection + 1 : selection;
            selection = (checker == 1 && selection == 0) ? selection - 1 : selection;
            if (selection == 1) {
                //launch first easy segment
                //spawncheck arguments in order
                // (boulderNum, time to appear, totalElapsedTime, SPEED)
                spawnCheck(&first, 2.0, totalElapsedTime, 6.0);
                CP_Graphics_DrawCircle(value_x_mid, first.value_y, radius * 4.0);

                spawnCheck(&second, 3.0, totalElapsedTime, 6.0);
                CP_Graphics_DrawCircle(value_x_left, second.value_y, radius * 4.0);

                spawnCheck(&third, 3.5, totalElapsedTime, 5.0);
                CP_Graphics_DrawCircle(value_x_mid, third.value_y, radius * 4.0);

                spawnCheck(&fourth, 4.0, totalElapsedTime, 5.0);
                CP_Graphics_DrawCircle(value_x_right, fourth.value_y, radius * 4.0);

                spawnCheck(&fifth, 7.0, totalElapsedTime, 6.0);
                CP_Graphics_DrawCircle(value_x_right, fifth.value_y, radius * 4.0);

                spawnCheck(&sixth, 8.0, totalElapsedTime, 10.0);
                CP_Graphics_DrawCircle(value_x_left, sixth.value_y, radius * 4.0);

                spawnCheck(&seventh, 10.0, totalElapsedTime, 11.0);
                CP_Graphics_DrawCircle(value_x_mid, seventh.value_y, radius * 4.0);

                spawnCheck(&eighth, 13.0, totalElapsedTime, 11.0);
                CP_Graphics_DrawCircle(value_x_left, eighth.value_y, radius * 4.0);

                spawnCheck(&ninth, 15.0, totalElapsedTime, 14.0);
                CP_Graphics_DrawCircle(value_x_right, ninth.value_y, radius * 4.0);

                spawnCheck(&ten, 16.0, totalElapsedTime, 12.0);
                CP_Graphics_DrawCircle(value_x_mid, ten.value_y, radius * 4.0);
                // ADD IF INVULNERABLE == 1
                if (invulnerable == 0 && just_got_hit == 0) {
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, first.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, second.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, third.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, fourth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, fifth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, sixth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, seventh.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, eighth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, ninth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, ten.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                }

            }
            else if (selection == -1) {
                //launch second easy segment instead
                /*
                first.boulder = (totalElapsedTime > 2.0 && first.boulder != 2) ? first.boulder + 1 : first.boulder;
                first.boulder_spawn = (first.boulder == 1) ? TRUE : first.boulder_spawn;
                first.value_y += (first.boulder_spawn == TRUE) ? 8.0f : 0.0f;
                */
                spawnCheck(&first, 2.0, totalElapsedTime, 8.0);
                CP_Graphics_DrawCircle(value_x_left, first.value_y, radius * 4.0);

                spawnCheck(&second, 3.0, totalElapsedTime, 10.0);
                CP_Graphics_DrawCircle(value_x_right, second.value_y, radius * 4.0);

                spawnCheck(&third, 4.0, totalElapsedTime, 15.0);
                CP_Graphics_DrawCircle(value_x_mid, third.value_y, radius * 4.0);

                CP_Settings_RectMode(CP_POSITION_CENTER);
                spawnRect(&rect1, 5.5, totalElapsedTime, 15.0);
                CP_Graphics_DrawRect(value_x_right, rect1.value_y, width * 0.15, height * 0.6);

                spawnCheck(&fourth, 5.5, totalElapsedTime, 13.0);
                CP_Graphics_DrawCircle(value_x_left, fourth.value_y, radius * 4.0);

                spawnCheck(&fifth, 8.5, totalElapsedTime, 13.0);
                CP_Graphics_DrawCircle(value_x_right, fifth.value_y, radius * 4.0);

                spawnCheck(&sixth, 10.0, totalElapsedTime, 10.0);
                CP_Graphics_DrawCircle(value_x_mid, sixth.value_y, radius * 4.0);

                spawnCheck(&seventh, 12.0, totalElapsedTime, 7.0);
                CP_Graphics_DrawCircle(value_x_mid, seventh.value_y, radius * 4.0);

                spawnCheck(&eighth, 14.0, totalElapsedTime, 11.0);
                CP_Graphics_DrawCircle(value_x_left, eighth.value_y, radius * 4.0);

                spawnCheck(&ninth, 15.0, totalElapsedTime, 9.0);
                CP_Graphics_DrawCircle(value_x_right, ninth.value_y, radius * 4.0);

                spawnCheck(&ten, 16.0, totalElapsedTime, 12.0);
                CP_Graphics_DrawCircle(value_x_mid, ten.value_y, radius * 4.0);

                if (invulnerable == 0 && just_got_hit == 0) {
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, first.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, second.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, third.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, fourth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, fifth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, sixth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, seventh.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, eighth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, ninth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, ten.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (isRectCollided(current_position.x, current_position.y, radius, value_x_right, rect1.value_y, width * 0.15, height * 0.6) == 1) {
                        just_got_hit = 1;
                    }
                }
            }
            //random range is set to 0 to 1
            // if range == 0, launch first easy segment
            // if range == 1, launch second easy segment instead
        }


        if (1 == difficulty) {

        checker = CP_Random_RangeInt(0, 1);
        selection = (checker == 0 && selection == 0) ? selection + 1 : selection;
        selection = (checker == 1 && selection == 0) ? selection - 1 : selection;
            if (selection == 1) {
                //launch first medium segment
                spawnCheck(&first, 2.5, totalElapsedTime, 8.5);
                CP_Graphics_DrawCircle(value_x_left, first.value_y, radius * 4.0);

                spawnCheck(&second, 3.3, totalElapsedTime, 9.0);
                CP_Graphics_DrawCircle(value_x_right, second.value_y, radius * 4.0);

                spawnCheck(&third, 3.8, totalElapsedTime, 9.0);
                CP_Graphics_DrawCircle(value_x_mid, third.value_y, radius * 4.0);

                spawnCheck(&fourth, 6.0, totalElapsedTime, 10.0);
                CP_Graphics_DrawCircle(value_x_right, fourth.value_y, radius * 4.0);

                spawnCheck(&fifth, 8.0, totalElapsedTime, 11.0);
                CP_Graphics_DrawCircle(value_x_left, fifth.value_y, radius * 4.0);

                spawnCheck(&sixth, 8.0, totalElapsedTime, 11.0);
                CP_Graphics_DrawCircle(value_x_mid, sixth.value_y, radius * 4.0);

                spawnCheck(&seventh, 11.0, totalElapsedTime, 8.0);
                CP_Graphics_DrawCircle(value_x_mid, seventh.value_y, radius * 4.0);

                spawnCheck(&eighth, 13.0, totalElapsedTime, 10.0);
                CP_Graphics_DrawCircle(value_x_right, eighth.value_y, radius * 4.0);

                spawnCheck(&ninth, 15.0, totalElapsedTime, 14.0);
                CP_Graphics_DrawCircle(value_x_left, ninth.value_y, radius * 4.0);

                spawnCheck(&ten, 16.0, totalElapsedTime, 12.0);
                CP_Graphics_DrawCircle(value_x_mid, ten.value_y, radius * 4.0);

                if (invulnerable == 0 && just_got_hit == 0) {
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, first.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, second.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, third.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, fourth.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, fifth.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, sixth.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, seventh.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, eighth.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, ninth.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, ten.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                }
            }
            else if (selection == -1) {
                //launch second medium segment instead
                spawnCheck(&first, 2.8, totalElapsedTime, 16.0);
                CP_Graphics_DrawCircle(value_x_left, first.value_y, radius * 4.0);

                spawnCheck(&second, 3.6, totalElapsedTime, 14.0);
                CP_Graphics_DrawCircle(value_x_right, second.value_y, radius * 4.0);

                spawnCheck(&third, 3.6, totalElapsedTime, 14.0);
                CP_Graphics_DrawCircle(value_x_mid, third.value_y, radius * 4.0);

                spawnCheck(&fourth, 6.2, totalElapsedTime, 13.0);
                CP_Graphics_DrawCircle(value_x_right, fourth.value_y, radius * 4.0);

                spawnCheck(&fifth, 7.0, totalElapsedTime, 17.0);
                CP_Graphics_DrawCircle(value_x_left, fifth.value_y, radius * 4.0);

                spawnCheck(&sixth, 7.8, totalElapsedTime, 14.0);
                CP_Graphics_DrawCircle(value_x_mid, sixth.value_y, radius * 4.0);

                spawnCheck(&seventh, 11.0, totalElapsedTime, 8.0);
                CP_Graphics_DrawCircle(value_x_right, seventh.value_y, radius * 4.0);

                spawnCheck(&eighth, 13.0, totalElapsedTime, 10.0);
                CP_Graphics_DrawCircle(value_x_left, eighth.value_y, radius * 4.0);

                spawnCheck(&ninth, 15.0, totalElapsedTime, 14.0);
                CP_Graphics_DrawCircle(value_x_mid, ninth.value_y, radius * 4.0);

                spawnCheck(&ten, 16.0, totalElapsedTime, 12.0);
                CP_Graphics_DrawCircle(value_x_right, ten.value_y, radius * 4.0);

                if (invulnerable == 0 && just_got_hit == 0) {
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, first.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, second.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, third.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, fourth.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, fifth.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, sixth.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, seventh.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, eighth.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, ninth.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, ten.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                }
            }
            //random range is set to 0 to 1
            // if range == 0, launch first medium segment
            // if range == 1, launch second medium segment instead
        }
        else if (2 == difficulty /*skip 3 because hard segment is 20seconds long*/) {
        checker = CP_Random_RangeInt(0, 1);
        selection = (checker == 0 && selection == 0) ? selection + 1 : selection;
        selection = (checker == 1 && selection == 0) ? selection - 1 : selection;
            //random range is set to 0, 1 or 2
            if (1 == selection) {
                //launch first hard segment
                
                spawnCheck(&first, 1.5, totalElapsedTime, 20.0);
                CP_Graphics_DrawCircle(value_x_left, first.value_y, radius * 4.0);

                spawnCheck(&second, 1.5, totalElapsedTime, 20.0);
                CP_Graphics_DrawCircle(value_x_right, second.value_y, radius * 4.0);

                spawnCheck(&third, 2.0, totalElapsedTime, 20.0);
                CP_Graphics_DrawCircle(value_x_mid, third.value_y, radius * 4.0);

                spawnCheck(&fourth, 3.7, totalElapsedTime, 20.0);
                CP_Graphics_DrawCircle(value_x_left, fourth.value_y, radius * 4.0);

                spawnCheck(&fifth, 3.7, totalElapsedTime, 20.0);
                CP_Graphics_DrawCircle(value_x_right, fifth.value_y, radius * 4.0);

                spawnCheck(&sixth, 4.5, totalElapsedTime, 20.0);
                CP_Graphics_DrawCircle(value_x_left, sixth.value_y, radius * 4.0);

                spawnCheck(&seventh, 4.5, totalElapsedTime, 20.0);
                CP_Graphics_DrawCircle(value_x_mid, seventh.value_y, radius * 4.0);

                spawnCheck(&eighth, 6.0, totalElapsedTime, 20.0);
                CP_Graphics_DrawCircle(value_x_right, eighth.value_y, radius * 4.0);

                spawnCheck(&ninth, 6.5, totalElapsedTime, 20.0);
                CP_Graphics_DrawCircle(value_x_mid, ninth.value_y, radius * 4.0);

                spawnCheck(&ten, 7.0, totalElapsedTime, 20.0);
                CP_Graphics_DrawCircle(value_x_left, ten.value_y, radius * 4.0);

                if (invulnerable == 0 && just_got_hit == 0) {
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, first.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, second.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, third.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, fourth.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, fifth.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, sixth.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, seventh.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, eighth.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, ninth.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, ten.value_y, radius, 0) == 1) {
                        totalElapsedTime = 0;
                        score += points;
                    };
                }
            }
            else if (selection == -1) {
                //launch second hard segment instead

            }
            else {
                //launch third hard segment instead
            }
            // if range == 0, launch first hard segment
            // if range == 1, launch second hard segment instead
            // if range == 2, launch third hard segment instead
        }
        else if (difficulty > 2) {
            checker = CP_Random_RangeInt(0, 2);
            selection = (checker == 0 && selection == 0) ? selection + 1 : selection;
            selection = (checker == 1 && selection == 0) ? selection - 1 : selection;
            selection = (checker == 2 && selection == 0) ? selection + 2 : selection;
            //random range is set to 0, 1 or 2
            if (1 == selection) {
                //launch first hard segment
                CP_Settings_Fill(red);
                spawnCheck(&first, 1.5, totalElapsedTime, 20.5);
                CP_Graphics_DrawCircle(value_x_left, first.value_y, radius * 4.0);
            }
            else if (selection == -1) {
                //launch second hard segment instead
                
                spawnCheck(&first, 1.5, totalElapsedTime, 20.5);
                CP_Graphics_DrawCircle(value_x_left, first.value_y, radius * 4.0);

                spawnCheck(&second, 2.5, totalElapsedTime, 20.5);
                CP_Graphics_DrawCircle(value_x_mid, second.value_y, radius * 4.0);

                spawnCheck(&third, 2.5, totalElapsedTime, 20.5);
                CP_Graphics_DrawCircle(value_x_right, third.value_y, radius * 4.0);

                spawnCheck(&fourth, 4.5, totalElapsedTime, 21.5);
                CP_Graphics_DrawCircle(value_x_left, fourth.value_y, radius * 4.0);

                spawnCheck(&fifth, 4.5, totalElapsedTime, 21.5);
                CP_Graphics_DrawCircle(value_x_right, fifth.value_y, radius * 4.0);

                spawnCheck(&sixth, 5.5, totalElapsedTime, 21.5);
                CP_Graphics_DrawCircle(value_x_mid, sixth.value_y, radius * 4.0);
                
            }
            else if(selection == 2) {
                //launch third hard segment instead
                CP_Settings_Fill(blue);
                spawnCheck(&first, 3.0, totalElapsedTime, 24.0);
                CP_Graphics_DrawCircle(value_x_mid, first.value_y, radius * 6.0);
            }
            // if range == 0, launch first hard segment
            // if range == 1, launch second hard segment instead
            // if range == 2, launch third hard segment instead
        }



        /*
        first.boulder = (totalElapsedTime > 3.0 && first.boulder != 2) ? first.boulder + 1 : first.boulder;
        first.boulder_spawn = (first.boulder == 1) ? TRUE : first.boulder_spawn;
        first.value_y += (first.boulder_spawn == TRUE) ? 5.0f : 0.0f;
        CP_Graphics_DrawCircle(value_x_mid, first.value_y, radius * 4.0);


        second.boulder = (totalElapsedTime > 7.0 && second.boulder != 2) ? second.boulder + 1: second.boulder;
        second.boulder_spawn = (second.boulder == 1) ? TRUE : second.boulder_spawn;
        second.value_y += (second.boulder_spawn == TRUE) ? 5.0f : 0.0f;
        CP_Graphics_DrawCircle(value_x_left, second.value_y, radius * 4.0);

        third.boulder = (totalElapsedTime > 10.0 && third.boulder != 2) ? third.boulder + 1: third.boulder;
        third.boulder_spawn = (third.boulder == 1) ? TRUE : third.boulder_spawn;
        third.value_y += (third.boulder_spawn == TRUE) ? 5.0f : 0.0f;
        CP_Graphics_DrawCircle(value_x_right, third.value_y, radius * 4.0);
        */


        first.coins = (totalElapsedTime > 1.0 && first.coins != 2) ? first.coins + 1 : first.coins;
        first.coin_spawn = (first.coins == 1) ? TRUE : first.coin_spawn;
        coin_y += (first.coin_spawn == TRUE) ? 9.0f : 0.0f;
        if (spawn == 1) { CP_Graphics_DrawCircle(value_x_mid, coin_y, radius * 1.0); }

        CP_Settings_Fill(blue);
        first.pts_boost = (totalElapsedTime > 0.5 && first.pts_boost != 2) ? first.pts_boost + 1 : first.pts_boost;
        first.pts_boost_spawn = (first.pts_boost == 1) ? TRUE : first.pts_boost_spawn;
        pts_boost_y += (first.pts_boost_spawn == TRUE) ? 9.0f : 0.0f;
        if (spawn_ptsboost == 1) { CP_Graphics_DrawCircle(value_x_mid, pts_boost_y, radius * 1.0); }

        CP_Settings_Fill(red);
        first.invul = (totalElapsedTime > 0.0 && first.invul != 2) ? first.invul + 1 : first.invul;
        first.invul_spawn = (first.invul == 1) ? TRUE : first.invul_spawn;
        invul_y += (first.invul_spawn == TRUE) ? 9.0f : 0.0f;
        if (spawn_invul == 1) { CP_Graphics_DrawCircle(value_x_mid, invul_y, radius * 1.0); }

        //Displaying Points
        CP_Settings_TextSize(20.0f);
        CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
        char buffer[16] = { 0 };
        sprintf_s(buffer, _countof(buffer), "%d", points);
        CP_Font_DrawText(buffer, width/10, 20);
        CP_Font_DrawText("Points:", width/25,20);

        CP_Settings_Fill(red);
        CP_Settings_RectMode(CP_POSITION_CENTER);
        if (health >= 1) { CP_Graphics_DrawRect(windows_length / 20 * 19, windows_height / 20, windows_length / 20, windows_height / 40); }
        if (health >= 2) { CP_Graphics_DrawRect(windows_length / 20 * 17, windows_height / 20, windows_length / 20, windows_height / 40); }
        if (health >= 3) { CP_Graphics_DrawRect(windows_length / 20 * 15, windows_height / 20, windows_length / 20, windows_height / 40); }



        if (health <= 0) {
            //dies
            totalElapsedTime = 0;
            score += points;
            health_toggle = 0;
            CP_Engine_SetNextGameStateForced(Death_Screen_Init, Death_Screen_Update, Death_Screen_Exit);
        }
        /*
        if (invulnerable == 0) { //detects obstacle collision only when invulnerable flag is off
            if (iscirclecollided(current_position.x, current_position.y, value_x_mid, first.value_y, radius,0) == 1) {
                totalElapsedTime = 0;
                score += points;
            };
            if (iscirclecollided(current_position.x, current_position.y, value_x_left, second.value_y, radius,0) == 1) {
                totalElapsedTime = 0;
                score += points;
            };
            if (iscirclecollided(current_position.x, current_position.y, value_x_right, third.value_y, radius,0) == 1) {
                totalElapsedTime = 0;
                score += points;
            };
        }
        */

        if (spawn == 1) {
            if (iscirclecollided(current_position.x, current_position.y, value_x_mid, coin_y, radius,1) == 2) {points += 1 * multiplier; spawn = 0;}
        }
        if(spawn_ptsboost == 1) {
            if (iscirclecollided(current_position.x, current_position.y, value_x_mid, pts_boost_y, radius,1) == 2) { current_pts_increase = 1; spawn_ptsboost = 0; }
        }
        if (spawn_invul == 1) {
            if (iscirclecollided(current_position.x, current_position.y, value_x_mid, invul_y, radius,1) == 2) { invulnerable = 1; spawn_invul = 0; }
        }
        
        if (current_pts_increase == 1) { //system to increase multiplier during point boost buff
            pts_increase_timer += currentElapsedTime;
            multiplier = 3;
            if (pts_increase_timer >= 10.0f) {
                current_pts_increase = 0; //turns off the point boost flag
                pts_increase_timer = 0.0f; //resets timer
                multiplier = 1; //resets multiplier back to 1
            }
        }

        if (invulnerable == 1) { //system to make player invulnerable during buff
            invulnerable_timer += currentElapsedTime;
            if (invulnerable_timer >= 5.0f) {
                invulnerable = 0; //turns off invulnerable flag
                invulnerable_timer = 0.0f; //resets timer
            }
        }

        //rmb to add just_got_hit to disable obstacle collision 
        if (just_got_hit == 1) { //system to make player invulnerable during buff
            ++health_toggle;
            just_got_hit_timer += currentElapsedTime;
            if (just_got_hit_timer >= 1.0f) {
                health_toggle = 0;
                just_got_hit = 0; //turns off invulnerable flag
                just_got_hit_timer = 0.0f; //resets timer
            }
        }

        if (health_toggle == 1) {
            --health;
            CP_Sound_PlayAdvanced(damagesound, 0.7f, 1.0f, FALSE, CP_SOUND_GROUP_2);
        }

    }

    if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
        gIsPaused = !gIsPaused;
    }

    if (gIsPaused == TRUE) {
        float retainedTime = retainTime(&totalElapsedTime);

        //CP_Graphics_ClearBackground(green);

        CP_Settings_Fill(pause);
        CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.5, CP_System_GetWindowHeight() * 0.5, width, height);

        // set the rectangle drawing mode to CENTER
        CP_Settings_RectMode(CP_POSITION_CENTER);
        CP_Settings_Fill(blue);
        // draw a rectangle at the center of the screen, half the size of the screen
        CP_Settings_TextSize(60.0f);
        CP_Graphics_DrawRect(resumeX, resumeY, resumeWidth, resumeHeight); // Resume Button
        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
        CP_Settings_Fill(white);
        CP_Font_DrawText("Resume", resumeX, resumeY);

        CP_Settings_Fill(blue);
        CP_Graphics_DrawRect(restartX, restartY, restartWidth, restartHeight); //Restart Button
        CP_Settings_Fill(white);
        CP_Font_DrawText("Restart", restartX, restartY);

        CP_Settings_Fill(blue);
        CP_Graphics_DrawRect(b2mmX, b2mmY, b2mmWidth, b2mmHeight);
        CP_Settings_Fill(white);
        CP_Font_DrawText("Main Menu", b2mmX, b2mmY);

        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
            if (areaClick(resumeX, resumeY, resumeWidth, resumeHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
                gIsPaused = FALSE;
            }
            if (areaClick(restartX, restartY, restartWidth, restartHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
                gIsPaused = FALSE;
                totalElapsedTime = 0;
                currentElapsedTime = 0;
                difficulty = 0;
                CP_Engine_SetNextGameStateForced(game_init, game_update, game_update);
            }
            if (areaClick(b2mmX, b2mmY, b2mmWidth, b2mmHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
                CP_Engine_SetNextGameStateForced(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
               
            }
        }

        if (CP_Input_KeyTriggered(KEY_R)) {
            gIsPaused = FALSE;
            totalElapsedTime = 0;
            currentElapsedTime = 0;
            difficulty = 0;
            CP_Engine_SetNextGameStateForced(game_init, game_update, game_update);
        }
    }
}

void game_exit(void)
{
    CP_Sound_Free(&shiftsound);
    CP_Sound_Free(&runsound);
    CP_Sound_Free(&Startsound);
    CP_Sound_Free(&damagesound);
}
