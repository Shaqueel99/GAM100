#include <stdio.h>
#include <stdlib.h>
#include "cprocessing.h"
#include "mainmenu.h"
#include "deathscreen.h"
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
CP_Color black, blue, purple, green, red, white, pause;
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
int playervisible = 255;
// Assets (Image)
static CP_Image image_background, image_boulder, image_dino, image_heart, image_invul, image_meat, image_double_meat, image_log;
static CP_Image image_pause_background, image_resume, image_restart, image_mainmenu;

void game_init(void)

{
    playervisible = 255;
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
    windows_length = (float)width;
    windows_height = (float)height;
    radius = 20.0f;
    CP_System_SetWindowSize((int)windows_length, (int)windows_height);

    // Assets (Image declaration)
    image_background = CP_Image_Load("Assets/game_ui/game_background.png");
    image_boulder = CP_Image_Load("Assets/game_ui/game_boulder.png");
    image_dino = CP_Image_Load("Assets/game_ui/game_dino.png");
    image_heart = CP_Image_Load("Assets/game_ui/game_heart.png");
    image_invul = CP_Image_Load("Assets/game_ui/game_invul.png");
    image_meat = CP_Image_Load("Assets/game_ui/game_meat.png");
    image_double_meat = CP_Image_Load("Assets/game_ui/game_double_meat.png");
    image_log = CP_Image_Load("Assets/game_ui/game_log.png");
    image_pause_background = CP_Image_Load("Assets/mainmenu_button/option_background.png");
    image_resume = CP_Image_Load("Assets/game_ui/pause_resume_button.png");
    image_restart = CP_Image_Load("Assets/game_ui/pause_restart_button.png");
    image_mainmenu = CP_Image_Load("Assets/game_ui/pause_mainmenu_button.png");

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

    resumeWidth = width * 0.35f;
    resumeHeight = height * 0.1f;
    resumeX = CP_System_GetWindowWidth() * 0.3f;
    resumeY = CP_System_GetWindowHeight() * 0.45f;

    restartWidth = width * 0.35f;
    restartHeight = height * 0.1f;
    restartX = CP_System_GetWindowWidth() * 0.7f;
    restartY = CP_System_GetWindowHeight() * 0.45f;

    b2mmWidth = width * 0.5f;
    b2mmHeight = height * 0.1f;
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


    
    left_position = CP_Vector_Set(width * 0.16f, height / 4.0f * 3.0f);
    mid_position = CP_Vector_Set(width / 2.0f, height / 4.0f * 3.0f);
    right_position = CP_Vector_Set(width * 0.83f, height / 4.0f * 3.0f);


    current_position = mid_position;

    // Boulder spawn postion
    value_y = -windows_height / 8.0f;
    value_x_left = windows_length / 6.0f;
    value_x_mid = windows_length / 2.0f;
    value_x_right = windows_length / 6.0f * 5.0f;



    first.value_y = second.value_y = third.value_y = coin_y= pts_boost_y = invul_y = -windows_height / 12.0f;
    fourth.value_y = fifth.value_y = sixth.value_y = seventh.value_y = -height / 12.0f;
    eighth.value_y = ninth.value_y = ten.value_y = -height / 12.0f;
    eleven.value_y = twelve.value_y = thirt.value_y = fourt.value_y = fifte.value_y = -height / 12.0f;
    sixte.value_y = sevente.value_y = eighte.value_y = ninete.value_y = -height / 12.0f;
    rect1.value_y = rect2.value_y = rect3.value_y = -height / 3.0f;

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
        CP_Sound_ResumeAll();
        //Displaying background
        CP_Color myColor = CP_Color_Create(211, 211, 211, 255);
        CP_Settings_Fill(myColor);
        CP_Graphics_ClearBackground(green);
        CP_Settings_Stroke(black);
        CP_Graphics_DrawLine(windows_length / 3.0f, 0.0f, windows_length / 3.0f, windows_height);
        CP_Graphics_DrawLine(windows_length / 3.0f * 2.0f, 0.0f, windows_length / 3.0f * 2.0f, windows_height);

        //Drawing (UI)
        CP_Image_Draw(image_background, (float)width / 2.0f, height / 2.0f, (float)width, (float)height, 255);

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





        //Displaying player
        
        //CP_Graphics_DrawCircle(current_position.x, current_position.y, radius * 2.0f);
        //CP_Graphics_DrawTriangleAdvanced(current_position.x, current_position.y - radius, current_position.x - radius / 2.0f, current_position.y, current_position.x + radius / 2.0f, current_position.y, 0.0f);
        CP_Image_DrawAdvanced(image_dino, current_position.x, current_position.y, width * 0.06f, height * 0.05f, playervisible, 270);

        //Spawing Boulders
        currentElapsedTime = CP_System_GetDt();
        totalElapsedTime += currentElapsedTime;




        if (totalElapsedTime > 20.0f) {
            totalElapsedTime = 0.0f;
            ++difficulty;

            //resets all parameters
            first.value_y = second.value_y = third.value_y = -windows_height / 12.0f;
            fourth.value_y = fifth.value_y = sixth.value_y = seventh.value_y = -height / 12.0f;
            eighth.value_y = ninth.value_y = ten.value_y = -height / 12.0f;
            eleven.value_y = twelve.value_y = thirt.value_y = fourt.value_y = fifte.value_y = -height / 12.0f;
            sixte.value_y = sevente.value_y = eighte.value_y = ninete.value_y = -height / 12.0f;

            rect1.value_y = rect2.value_y = rect3.value_y = -height / 4.0f;

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
                spawnCheck(&first, 2.0f, totalElapsedTime, 6.0f);
                //CP_Graphics_DrawCircle(value_x_mid, first.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, first.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&second, 3.0f, totalElapsedTime, 6.0f);
                //CP_Graphics_DrawCircle(value_x_left, second.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, second.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&third, 3.5f, totalElapsedTime, 5.0f);
                //CP_Graphics_DrawCircle(value_x_mid, third.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, third.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fourth, 4.0f, totalElapsedTime, 5.0f);
                //CP_Graphics_DrawCircle(value_x_right, fourth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, fourth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fifth, 7.0f, totalElapsedTime, 6.0f);
                //CP_Graphics_DrawCircle(value_x_right, fifth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, fifth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&sixth, 8.0f, totalElapsedTime, 10.0f);
                //CP_Graphics_DrawCircle(value_x_left, sixth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, sixth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&seventh, 10.0f, totalElapsedTime, 11.0f);
                //CP_Graphics_DrawCircle(value_x_mid, seventh.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, seventh.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&eighth, 13.0f, totalElapsedTime, 11.0f);
                //CP_Graphics_DrawCircle(value_x_left, eighth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, eighth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&ninth, 15.0f, totalElapsedTime, 14.0f);
                //CP_Graphics_DrawCircle(value_x_right, ninth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, ninth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&ten, 16.0f, totalElapsedTime, 12.0f);
                //CP_Graphics_DrawCircle(value_x_mid, ten.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, ten.value_y, width * 0.13f, height * 0.09f, 255);

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
                spawnCheck(&first, 2.0f, totalElapsedTime, 8.0f);
                // CP_Graphics_DrawCircle(value_x_left, first.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, first.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&second, 3.0f, totalElapsedTime, 10.0f);
                //CP_Graphics_DrawCircle(value_x_right, second.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, second.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&third, 4.0f, totalElapsedTime, 15.0f);
                //CP_Graphics_DrawCircle(value_x_mid, third.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, third.value_y, width * 0.13f, height * 0.09f, 255);

                CP_Settings_RectMode(CP_POSITION_CENTER);
                spawnRect(&rect1, 5.5f, totalElapsedTime, 15.0f);
                // CP_Graphics_DrawRect(value_x_right, rect1.value_y, width * 0.15f, height * 0.6f);
                CP_Image_Draw(image_log, value_x_right, rect1.value_y, width * 0.15f, height * 0.6f, 255);

                spawnCheck(&fourth, 5.5f, totalElapsedTime, 13.0f);
               // CP_Graphics_DrawCircle(value_x_left, fourth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, fourth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fifth, 8.5f, totalElapsedTime, 13.0f);
                //CP_Graphics_DrawCircle(value_x_right, fifth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, fifth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&sixth, 10.0f, totalElapsedTime, 10.0f);
                //CP_Graphics_DrawCircle(value_x_mid, sixth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, sixth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&seventh, 12.0f, totalElapsedTime, 7.0f);
                //CP_Graphics_DrawCircle(value_x_mid, seventh.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, seventh.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&eighth, 14.0f, totalElapsedTime, 11.0f);
                //CP_Graphics_DrawCircle(value_x_left, eighth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, eighth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&ninth, 15.0f, totalElapsedTime, 9.0f);
                //CP_Graphics_DrawCircle(value_x_right, ninth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, ninth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&ten, 16.0f, totalElapsedTime, 12.0f);
                //CP_Graphics_DrawCircle(value_x_mid, ten.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, ten.value_y, width * 0.13f, height * 0.09f, 255);

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
                    if (isRectCollided(current_position.x, current_position.y, radius, value_x_right, rect1.value_y, width * 0.15f, height * 0.6f) == 1) {
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
                spawnCheck(&first, 2.5f, totalElapsedTime, 8.5f);
                //CP_Graphics_DrawCircle(value_x_left, first.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, first.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&second, 3.3f, totalElapsedTime, 9.0f);
                //CP_Graphics_DrawCircle(value_x_right, second.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, second.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&third, 3.8f, totalElapsedTime, 9.0f);
                //CP_Graphics_DrawCircle(value_x_mid, third.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, third.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fourth, 6.0f, totalElapsedTime, 10.0f);
                //CP_Graphics_DrawCircle(value_x_right, fourth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, fourth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fifth, 8.0f, totalElapsedTime, 11.0f);
                //CP_Graphics_DrawCircle(value_x_left, fifth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, fifth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&sixth, 8.0f, totalElapsedTime, 11.0f);
                //CP_Graphics_DrawCircle(value_x_mid, sixth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, sixth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&seventh, 11.0f, totalElapsedTime, 8.0f);
                //CP_Graphics_DrawCircle(value_x_mid, seventh.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, seventh.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&eighth, 13.0f, totalElapsedTime, 10.0f);
                //CP_Graphics_DrawCircle(value_x_right, eighth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, eighth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&ninth, 15.0f, totalElapsedTime, 14.0f);
                //CP_Graphics_DrawCircle(value_x_left, ninth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, ninth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&ten, 16.0f, totalElapsedTime, 12.0f);
                //CP_Graphics_DrawCircle(value_x_mid, ten.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, ten.value_y, width * 0.13f, height * 0.09f, 255);

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
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, fourth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, fifth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, sixth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, seventh.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, eighth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, ninth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, ten.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                }
            }
            else if (selection == -1) {
                //launch second medium segment instead
                spawnCheck(&first, 2.8f, totalElapsedTime, 16.0f);
                //CP_Graphics_DrawCircle(value_x_left, first.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, first.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&second, 3.6f, totalElapsedTime, 14.0f);
                //CP_Graphics_DrawCircle(value_x_right, second.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, second.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&third, 3.6f, totalElapsedTime, 14.0f);
                //CP_Graphics_DrawCircle(value_x_mid, third.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, third.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fourth, 6.2f, totalElapsedTime, 13.0f);
                //CP_Graphics_DrawCircle(value_x_right, fourth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, fourth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fifth, 7.0f, totalElapsedTime, 17.0f);
                //CP_Graphics_DrawCircle(value_x_left, fifth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, fifth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&sixth, 7.8f, totalElapsedTime, 14.0f);
                //CP_Graphics_DrawCircle(value_x_mid, sixth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, sixth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&seventh, 11.0f, totalElapsedTime, 8.0f);
                //CP_Graphics_DrawCircle(value_x_right, seventh.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, seventh.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&eighth, 13.0f, totalElapsedTime, 10.0f);
                //CP_Graphics_DrawCircle(value_x_left, eighth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, eighth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&ninth, 15.0f, totalElapsedTime, 14.0f);
                //CP_Graphics_DrawCircle(value_x_mid, ninth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, ninth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&ten, 16.0f, totalElapsedTime, 12.0f);
                //CP_Graphics_DrawCircle(value_x_right, ten.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, ten.value_y, width * 0.13f, height * 0.09f, 255);

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
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, fourth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, fifth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, sixth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, seventh.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, eighth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, ninth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, ten.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
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
                
                spawnCheck(&first, 1.5f, totalElapsedTime, 20.0f);
                //CP_Graphics_DrawCircle(value_x_left, first.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, first.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&second, 1.5f, totalElapsedTime, 20.0f);
                //CP_Graphics_DrawCircle(value_x_right, second.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, second.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&third, 2.0f, totalElapsedTime, 20.0f);
                //CP_Graphics_DrawCircle(value_x_mid, third.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, third.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fourth, 3.7f, totalElapsedTime, 20.0f);
                //CP_Graphics_DrawCircle(value_x_left, fourth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, fourth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fifth, 3.7f, totalElapsedTime, 20.0f);
                //CP_Graphics_DrawCircle(value_x_right, fifth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, fifth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&sixth, 4.5f, totalElapsedTime, 20.0f);
                //CP_Graphics_DrawCircle(value_x_left, sixth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, sixth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&seventh, 4.5f, totalElapsedTime, 20.0f);
                //CP_Graphics_DrawCircle(value_x_mid, seventh.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, seventh.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&eighth, 6.0f, totalElapsedTime, 20.0f);
                //CP_Graphics_DrawCircle(value_x_right, eighth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, eighth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&ninth, 6.5f, totalElapsedTime, 20.0f);
                //CP_Graphics_DrawCircle(value_x_mid, ninth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, ninth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&ten, 7.0f, totalElapsedTime, 20.0f);
                //CP_Graphics_DrawCircle(value_x_left, ten.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, ten.value_y, width * 0.13f, height * 0.09f, 255);

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
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, sixth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, seventh.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, eighth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, ninth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, ten.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
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
                spawnCheck(&first, 1.5f, totalElapsedTime, 20.5f);
                CP_Graphics_DrawCircle(value_x_left, first.value_y, radius * 4.0f);
            }
            else if (selection == -1) {
                //launch second hard segment instead
                
                spawnCheck(&first, 1.5f, totalElapsedTime, 20.5f);
                //CP_Graphics_DrawCircle(value_x_left, first.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, first.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&second, 2.5f, totalElapsedTime, 20.5f);
                //CP_Graphics_DrawCircle(value_x_mid, second.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, second.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&third, 2.5f, totalElapsedTime, 20.5f);
                //CP_Graphics_DrawCircle(value_x_right, third.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, third.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fourth, 4.5f, totalElapsedTime, 21.5f);
                //CP_Graphics_DrawCircle(value_x_left, fourth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, fourth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fifth, 4.5f, totalElapsedTime, 21.5f);
                //CP_Graphics_DrawCircle(value_x_right, fifth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, fifth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&sixth, 5.5f, totalElapsedTime, 21.5f);
                //CP_Graphics_DrawCircle(value_x_mid, sixth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, sixth.value_y, width * 0.13f, height * 0.09f, 255);
            }
            else if(selection == 2) {
                //launch third hard segment instead
                CP_Settings_Fill(blue);
                spawnCheck(&first, 3.0f, totalElapsedTime, 24.0f);
                CP_Graphics_DrawCircle(value_x_mid, first.value_y, radius * 6.0f);
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
        if (spawn == 1) { 
            // CP_Graphics_DrawCircle(value_x_mid, coin_y, radius * 1.0f); 
            CP_Image_Draw(image_meat, value_x_mid, coin_y, width * 0.07f, height * 0.04f, 255);
        }

        CP_Settings_Fill(blue);
        first.pts_boost = (totalElapsedTime > 0.5 && first.pts_boost != 2) ? first.pts_boost + 1 : first.pts_boost;
        first.pts_boost_spawn = (first.pts_boost == 1) ? TRUE : first.pts_boost_spawn;
        pts_boost_y += (first.pts_boost_spawn == TRUE) ? 9.0f : 0.0f;
        if (spawn_ptsboost == 1) { 
            // CP_Graphics_DrawCircle(value_x_mid, pts_boost_y, radius * 1.0f); 
            CP_Image_Draw(image_double_meat, value_x_mid, pts_boost_y, width * 0.07f, height * 0.04f, 255);
        }

        CP_Settings_Fill(red);
        first.invul = (totalElapsedTime > 0.0 && first.invul != 2) ? first.invul + 1 : first.invul;
        first.invul_spawn = (first.invul == 1) ? TRUE : first.invul_spawn;
        invul_y += (first.invul_spawn == TRUE) ? 9.0f : 0.0f;
        if (spawn_invul == 1) { 
            // CP_Graphics_DrawCircle(value_x_mid, invul_y, radius * 1.0f);
            CP_Image_Draw(image_invul, value_x_mid, invul_y, width * 0.05f, height * 0.04f, 255);
        }

        //Displaying Points
        CP_Settings_TextSize(width * 0.05f);
        CP_Settings_Fill(black);
        char buffer[16] = { 0 };
        sprintf_s(buffer, _countof(buffer), "%d", points);
        CP_Font_DrawText("Points:", width* 0.07f, height * 0.05f);
        CP_Font_DrawText(buffer, width * 0.21f, height * 0.05f);

        CP_Settings_Fill(red);
        CP_Settings_RectMode(CP_POSITION_CENTER);
        if (health >= 3) {
            //CP_Graphics_DrawRect(windows_length / 20 * 15, windows_height / 20, windows_length / 20, windows_height / 40);
            CP_Image_Draw(image_heart, width * 0.75f, height * 0.05f, width * 0.07f, height * 0.04f, 255);
        }
        if (health >= 2) { 
            //CP_Graphics_DrawRect(windows_length / 20 * 17, windows_height / 20, windows_length / 20, windows_height / 40); 
            CP_Image_Draw(image_heart, width * 0.85f, height * 0.05f, width * 0.07f, height * 0.04f, 255);
        }
        if (health >= 1) { 
            //CP_Graphics_DrawRect(windows_length / 20 * 19, windows_height / 20, windows_length / 20, windows_height / 40); 
            CP_Image_Draw(image_heart, width * 0.95f, height * 0.05f, width * 0.07f, height * 0.04f, 255);
        }
        if (health <= 0) {
            //dies
            totalElapsedTime = 0;
            score += points;
            health_toggle = 0;
            CP_Engine_SetNextGameStateForced(Death_Screen_Init, Death_Screen_Update, Death_Screen_Exit);
        }

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
            playervisible = 100;
            if (invulnerable_timer >= 5.0f) {
                playervisible = 255;
                invulnerable = 0; //turns off invulnerable flag
                invulnerable_timer = 0.0f; //resets timer
            }
        }

        //rmb to add just_got_hit to disable obstacle collision 
        if (just_got_hit == 1) { //system to make player invulnerable right after getting hit
            ++health_toggle;
            just_got_hit_timer += currentElapsedTime;
            playervisible = 100;
            if (just_got_hit_timer >= 1.0f) {
                playervisible = 255;
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
        CP_Sound_PauseAll();
        CP_Settings_Fill(pause);
        CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.5f, (float)width, (float)height);
        CP_Image_Draw(image_pause_background, (float)width / 2.0f, (float)height / 2.0f, (float)width, (float)height, 255);

        // set the rectangle drawing mode to CENTER
        CP_Settings_RectMode(CP_POSITION_CENTER);
        CP_Settings_Fill(blue);
        // draw a rectangle at the center of the screen, half the size of the screen
        
        //CP_Settings_TextSize(60.0f);
        //CP_Graphics_DrawRect(resumeX, resumeY, resumeWidth, resumeHeight); // Resume Button
        //CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
        //CP_Settings_Fill(white);
        //CP_Font_DrawText("Resume", resumeX, resumeY);
        CP_Image_Draw(image_resume, resumeX, resumeY, resumeWidth, resumeHeight, 255);

        //CP_Settings_Fill(blue);
        //CP_Graphics_DrawRect(restartX, restartY, restartWidth, restartHeight); //Restart Button
        //CP_Settings_Fill(white);
        //CP_Font_DrawText("Restart", restartX, restartY);
        CP_Image_Draw(image_restart, restartX, restartY, restartWidth, restartHeight, 255);

        //CP_Settings_Fill(blue);
        //CP_Graphics_DrawRect(b2mmX, b2mmY, b2mmWidth, b2mmHeight);
        //CP_Settings_Fill(white);
        //CP_Font_DrawText("Main Menu", b2mmX, b2mmY);
        CP_Image_Draw(image_mainmenu, b2mmX, b2mmY, b2mmWidth, b2mmHeight, 255);

        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
            if (areaClick(resumeX, resumeY, resumeWidth, resumeHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
                gIsPaused = FALSE;
            }
            if (areaClick(restartX, restartY, restartWidth, restartHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
                gIsPaused = FALSE;
                points = 0;
                totalElapsedTime = 0;
                currentElapsedTime = 0;
                difficulty = 0;
                CP_Sound_StopAll();
                CP_Engine_SetNextGameStateForced(game_init, game_update, game_update);
            }
            if (areaClick(b2mmX, b2mmY, b2mmWidth, b2mmHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
                gIsPaused = FALSE;
                totalElapsedTime = 0;
                currentElapsedTime = 0;
                difficulty = 0;
                CP_Sound_StopAll();
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
    CP_Image_Free(&image_background);
    CP_Image_Free(&image_boulder);
    CP_Image_Free(&image_dino);
    CP_Image_Free(&image_heart);
    CP_Image_Free(&image_invul);
    CP_Image_Free(&image_meat);
    CP_Image_Free(&image_double_meat);
    CP_Image_Free(&image_log);
}
