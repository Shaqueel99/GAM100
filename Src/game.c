/*----------------------------------------------------------------------------------
All content ? 2022 DigiPen Institute of Technology

FILE NAME:                  game.c
AUTHOR:                     Loke Wei Zhi, Benjamin <loke.w@digipen.edu>
CO AUTHOR:                  Yeo Kat Long <katlong.yeo@digipen.edu>
                            Mohammad Shaqeel bin Mohammad Fadilah <mohammadshaqeel.b@digipen.edu>
                            Chua Zhi Yu <zhiyu.chua@digipen.edu>
                            Ong Chee Keong <o.cheekeong@digipen.edu>

*****************************    GAME LOGIC        *******************************

BOULDER SPAWNING:           Yeo Kat Long
HEALTH:                     Yeo Kat Long
POWER UP:                   Yeo Kat Long
PLAYER MOVEMENT:            Mohammad Shaqeel bin Mohammad Fadilah
SCREEN SHAKE:               Mohammad Shaqeel bin Mohammad Fadilah
DIFFICULTY SCALING:         Mohammad Shaqeel bin Mohammad Fadilah
POINTS:                     Mohammad Shaqeel bin Mohammad Fadilah
PAUSE MENU:                 Loke Wei Zhi, Benjamin
LEVEL DESIGN:               Loke Wei Zhi, Benjamin
                            Ong Chee Keong

*****************************    GAME COLLISION    *******************************

GAME COLLISION:             Mohammad Shaqeel bin Mohammad Fadilah
                            Loke Wei Zhi, Benjamin

*****************************    GAME AUDIO        *******************************

GAME AUDIO:                 Mohammad Shaqeel bin Mohammad Fadilah

*****************************    GAME ASSETS       *******************************

GAME ASSETS:                Chua Zhi Yu

----------------------------------------------------------------------------------*/

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
extern float speed_scale, volume;
int gIsPaused;
int spawn, spawn_ptsboost, spawn_invul;
int c;
int selection;
int checker;
CP_Color black, blue, purple, green, red, white, pause;
float windows_length, windows_height, radius = 20.0f;
float rotation;
int name = 'a';
char score = '0';
CP_Vector current_position, left_position, mid_position, right_position;
float currentElapsedTime = 0, totalElapsedTime = 0;
float value_y, value_x_left, value_x_mid, value_x_right;

// Variables needed for use for Pause menu Screen
float resumeWidth, resumeHeight, resumeX, resumeY;
float restartWidth, restartHeight, restartX, restartY;
float b2mmWidth, b2mmHeight, b2mmX, b2mmY;
static int upordown; //used for visual cues
static CP_Image dblptsimg, invulimg; //Images loaded for visual cues

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

// obstacles needed for the segments 
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
int endgamestart;
float endgame = 0, totalEndgameTime = 0;
int speed = 50;
void game_init(void)

{
    speed = 50;
    totalEndgameTime = 0;
    endgame = 0,
    endgamestart = 0;
    rotation = 0;
    playervisible = 255;
    damagesound = CP_Sound_Load("Assets/Soundeffects/Dinodamage.wav");
    shiftsound = CP_Sound_Load("Assets/Soundeffects/DinoShifttrim.wav");
    runsound = CP_Sound_Load("Assets/Soundeffects/Dinorunningtrim.wav");
    CP_Sound_PlayAdvanced(runsound, volume, 0.6f, TRUE, CP_SOUND_GROUP_2);
    int startsnd = CP_Random_RangeInt(1, 2);
    if (startsnd == 1) {
        Startsound = CP_Sound_Load("Assets/Soundeffects/DinoStart2.wav");
        CP_Sound_PlayAdvanced(Startsound, volume, 1.0f, FALSE, CP_SOUND_GROUP_2);
    }
    else {
        Startsound = CP_Sound_Load("Assets/Soundeffects/DinoStart1.wav");
        CP_Sound_PlayAdvanced(Startsound, volume, 1.0f, FALSE, CP_SOUND_GROUP_2);
    }

    health = 3;
    health_toggle = 0;
    just_got_hit = 0;
    just_got_hit_timer = 0.0;

    multiplier = 1; //default pts multiplier is 1
    current_pts_increase = 0, invulnerable = 0;
    pts_increase_timer = invulnerable_timer = 0.0f;
    spawn = 1, spawn_ptsboost = 1, spawn_invul = 1;
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
    dblptsimg = CP_Image_Load("Assets/game_ui/dblPts.png");
    invulimg = CP_Image_Load("Assets/game_ui/invul.png");

    /* We start unpaused */
    gIsPaused = FALSE;

    /* Initialization of your other variables here */
    black = CP_Color_Create(0, 0, 0, 255);
    blue = CP_Color_Create(0, 255, 255, 255);
    purple = CP_Color_Create(76, 0, 153, 255);
    green = CP_Color_Create(0, 154, 23, 255);
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

    left_position = CP_Vector_Set(width * 0.16f, height / 4.0f * 3.0f);
    mid_position = CP_Vector_Set(width / 2.0f, height / 4.0f * 3.0f);
    right_position = CP_Vector_Set(width * 0.83f, height / 4.0f * 3.0f);


    current_position = mid_position;

    // Boulder spawn postion
    value_y = -windows_height / 8.0f;
    value_x_left = windows_length / 6.0f;
    value_x_mid = windows_length / 2.0f;
    value_x_right = windows_length / 6.0f * 5.0f;

    first.value_y = second.value_y = third.value_y = coin_y = first.pts_boost_y = first.invul_y = -windows_height / 12.0f;
    fourth.value_y = fifth.value_y = sixth.value_y = seventh.value_y = -height / 12.0f;
    eighth.value_y = ninth.value_y = ten.value_y = -height / 12.0f;
    eleven.value_y = twelve.value_y = thirt.value_y = fourt.value_y = fifte.value_y = -height / 12.0f;
    sixte.value_y = sevente.value_y = eighte.value_y = ninete.value_y = twoZero.value_y = -height / 12.0f;
    rect1.value_y = rect2.value_y = rect3.value_y = -height / 2.0f;

    first.pts_boost = 0;
    first.pts_boost_spawn = 0;
    first.invul = 0;
    first.invul_spawn = 0;

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
    eleven.boulder = 0;
    eleven.boulder_spawn = 0;
    twelve.boulder = 0;
    twelve.boulder_spawn = 0;
    thirt.boulder = 0;
    thirt.boulder_spawn = 0;
    fourt.boulder = 0;
    fourt.boulder_spawn = 0;
    fifte.boulder = 0;
    fifte.boulder_spawn = 0;
    sixte.boulder = 0;
    sixte.boulder_spawn = 0;
    sevente.boulder = 0;
    sevente.boulder_spawn = 0;
    eighte.boulder = 0;
    eighte.boulder_spawn = 0;
    ninete.boulder = 0;
    ninete.boulder_spawn = 0;
    twoZero.boulder = 0;
    twoZero.boulder_spawn = 0;

    rect1.rect = 0;
    rect1.rectSpawn = 0;
    rect2.rect = 0;
    rect2.rectSpawn = 0;
    rect3.rect = 0;
    rect3.rectSpawn = 0;

    selection = 0;
    checker = -1;

    upordown = 1;
}
float rot_counter = 0;
void game_update(void)
{
    if (gIsPaused == FALSE) {


        CP_Sound_ResumeAll();
        //Displaying background
        CP_Color myColor = CP_Color_Create(211, 211, 211, 255);
        CP_Settings_Fill(myColor);
        CP_Graphics_ClearBackground(green);
        CP_Settings_Stroke(black);

         //Drawing (UI)
        CP_Image_DrawAdvanced(image_background, (float)width / 2.0f, height / 2.0f, (float)width, (float)height, 255, rotation);
        //Movement

        if (movingleft == TRUE) {
            if (current_position.x != left_position.x && current_position.x != mid_position.x) { current_position.x -= (speed * speed_scale); }
            if (current_position.x <= left_position.x || current_position.x == mid_position.x) { movingleft = FALSE; }
        }
        if (movingright == TRUE) {
            if (current_position.x != right_position.x && current_position.x != mid_position.x) { current_position.x += (speed * speed_scale); }
            if (current_position.x >= right_position.x || current_position.x == mid_position.x) { movingright = FALSE; }
        }
        if (CP_Input_KeyTriggered(KEY_A)) {

            if (current_position.x == mid_position.x || current_position.x >= right_position.x) {
                current_position.x -= (25 * speed_scale);   movingleft = TRUE;  movingright = FALSE; CP_Sound_PlayAdvanced(shiftsound, volume, 1.0f, FALSE, CP_SOUND_GROUP_2);
            }
        }
        else if (CP_Input_KeyTriggered(KEY_D)) {
            if (current_position.x == mid_position.x || current_position.x <= left_position.x) {
                current_position.x += (25 * speed_scale);   movingright = TRUE;  movingleft = FALSE;  CP_Sound_PlayAdvanced(shiftsound, volume, 1.0f, FALSE, CP_SOUND_GROUP_2);
            }


        }

        float frame2frame = CP_System_GetDt();
        float timer = 1;
        float bluemulti = 255 * frame2frame / timer;
        float alphamulti = 255 * frame2frame / 2.5f;
        float invulmulti = 255 * frame2frame / 0.5f;


         //Displaying player
        CP_Image_DrawAdvanced(image_dino, current_position.x, current_position.y, width * 0.06f, height * 0.05f, playervisible, 0);

        //Spawing Boulders
        currentElapsedTime = CP_System_GetDt();
        totalElapsedTime += currentElapsedTime;
        totalEndgameTime += currentElapsedTime;
        if (endgamestart == 1) {
            if (totalEndgameTime > 5.0f) {
                if (endgame < 60.0f) {//cap at 60 speed, beyond that its ultra instinct 
                    endgame += 1.5f;
                    totalEndgameTime = 0.0f;
                   
                }
            }
        }
        if (totalElapsedTime > 20.0f) {
            totalElapsedTime = 0.0f;
            endgamestart = 1;
            ++difficulty;

            //resets all parameters
            first.value_y = second.value_y = third.value_y = -windows_height / 12.0f;
            fourth.value_y = fifth.value_y = sixth.value_y = seventh.value_y = -height / 12.0f;
            eighth.value_y = ninth.value_y = ten.value_y = -height / 12.0f;
            eleven.value_y = twelve.value_y = thirt.value_y = fourt.value_y = fifte.value_y = -height / 12.0f;
            sixte.value_y = sevente.value_y = eighte.value_y = ninete.value_y = twoZero.value_y = -height / 12.0f;

            first.invul_y = -height / 12.0f;
            first.pts_boost_y = -height / 12.0f;
            rect1.value_y = rect2.value_y = rect3.value_y = -height / 2.0f;

            first.pts_boost = 0;
            first.pts_boost_spawn = 0;
            first.invul = 0;
            first.invul_spawn = 0;

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
            eleven.boulder = 0;
            eleven.boulder_spawn = 0;
            twelve.boulder = 0;
            twelve.boulder_spawn = 0;
            thirt.boulder = 0;
            thirt.boulder_spawn = 0;
            fourt.boulder = 0;
            fourt.boulder_spawn = 0;
            fifte.boulder = 0;
            fifte.boulder_spawn = 0;
            sixte.boulder = 0;
            sixte.boulder_spawn = 0;
            sevente.boulder = 0;
            sevente.boulder_spawn = 0;
            eighte.boulder = 0;
            eighte.boulder_spawn = 0;
            ninete.boulder = 0;
            ninete.boulder_spawn = 0;
            twoZero.boulder = 0;
            twoZero.boulder_spawn = 0;

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
                CP_Image_Draw(image_boulder, value_x_mid, first.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&second, 3.0f, totalElapsedTime, 6.0f);
                CP_Image_Draw(image_boulder, value_x_left, second.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&third, 3.5f, totalElapsedTime, 5.0f);
                CP_Image_Draw(image_boulder, value_x_mid, third.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fourth, 4.0f, totalElapsedTime, 5.0f);
                CP_Image_Draw(image_boulder, value_x_right, fourth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fifth, 7.5f, totalElapsedTime, 6.0f);
                CP_Image_Draw(image_boulder, value_x_right, fifth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&sixth, 8.0f, totalElapsedTime, 11.3f);
                CP_Image_Draw(image_boulder, value_x_left, sixth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&seventh, 8.5f, totalElapsedTime, 9.4f);
                CP_Image_Draw(image_boulder, value_x_mid, seventh.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&eighth, 8.5f, totalElapsedTime, 9.4f);
                CP_Image_Draw(image_boulder, value_x_left, eighth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&ninth, 10.0f, totalElapsedTime, 6.0f);
                CP_Image_Draw(image_boulder, value_x_right, ninth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&ten, 11.0f, totalElapsedTime, 10.8f);
                CP_Image_Draw(image_boulder, value_x_mid, ten.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&eleven, 12.0f, totalElapsedTime, 13.0f);
                CP_Image_Draw(image_boulder, value_x_left, eleven.value_y, width * 0.13f, height * 0.09f, 255);

                dblPtsSpawn(&first, 12.5f, totalElapsedTime, 13.0f);//right side double points powerup here
                if (first.pts_boost_spawn == 1) {
                    CP_Image_Draw(image_double_meat, value_x_left, first.pts_boost_y, width * 0.07f, height * 0.04f, 255);
                }

                spawnCheck(&twelve, 13.0f, totalElapsedTime, 14.0f);
                CP_Image_Draw(image_boulder, value_x_left, twelve.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&thirt, 13.5f, totalElapsedTime, 15.0f);
                CP_Image_Draw(image_boulder, value_x_mid, thirt.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fourt, 15.0f, totalElapsedTime, 13.5f);
                CP_Image_Draw(image_boulder, value_x_mid, fourt.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fifte, 15.0f, totalElapsedTime, 13.5f);
                CP_Image_Draw(image_boulder, value_x_right, fifte.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&sixte, 15.7f, totalElapsedTime, 13.5f);
                CP_Image_Draw(image_boulder, value_x_left, sixte.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&sevente, 15.7f, totalElapsedTime, 13.5f);
                CP_Image_Draw(image_boulder, value_x_mid, sevente.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&eighte, 16.4f, totalElapsedTime, 13.5f);
                CP_Image_Draw(image_boulder, value_x_right, eighte.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&ninete, 16.4f, totalElapsedTime, 13.5f);
                CP_Image_Draw(image_boulder, value_x_mid, ninete.value_y, width * 0.13f, height * 0.09f, 255);

                if (first.pts_boost_spawn == 1) {
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, first.pts_boost_y, radius, 1) == 2) { current_pts_increase = 1; first.pts_boost_spawn = 0; }
                };

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
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, eleven.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, twelve.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, thirt.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, fourt.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, fifte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, sixte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, sevente.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, eighte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, ninete.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                }

            }
            else if (selection == -1) {
                //launch second easy segment instead

                spawnCheck(&first, 2.0f, totalElapsedTime, 8.0f);
                CP_Image_Draw(image_boulder, value_x_left, first.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&second, 3.0f, totalElapsedTime, 10.0f);
                CP_Image_Draw(image_boulder, value_x_right, second.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&third, 4.0f, totalElapsedTime, 15.0f);
                
                CP_Image_Draw(image_boulder, value_x_mid, third.value_y, width * 0.13f, height * 0.09f, 255);

                CP_Settings_RectMode(CP_POSITION_CENTER);
                spawnRect(&rect1, 5.5f, totalElapsedTime, 15.0f);
                 
                CP_Image_Draw(image_log, value_x_right, rect1.value_y, width * 0.15f, height * 0.6f, 255);

                spawnCheck(&fourth, 4.5f, totalElapsedTime, 13.0f);
 
                CP_Image_Draw(image_boulder, value_x_left, fourth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fifth, 8.0f, totalElapsedTime, 13.0f);
                CP_Image_Draw(image_boulder, value_x_right, fifth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&sixth, 6.5f, totalElapsedTime, 11.0f);
                CP_Image_Draw(image_boulder, value_x_mid, sixth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&seventh, 6.5f, totalElapsedTime, 11.0f);
                CP_Image_Draw(image_boulder, value_x_mid, seventh.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&eighth, 8.0f, totalElapsedTime, 11.0f);
                CP_Image_Draw(image_boulder, value_x_left, eighth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&ninth, 9.0f, totalElapsedTime, 13.0f);
                CP_Image_Draw(image_boulder, value_x_right, ninth.value_y, width * 0.13f, height * 0.09f, 255);

                invulSpawn(&first, 10.5f, totalElapsedTime, 13.0f); //invulnerability
                if (first.invul_spawn == 1) {
                    CP_Image_Draw(image_invul, value_x_mid, first.invul_y, width * 0.05f, height * 0.04f, 255);
                }

                spawnCheck(&ten, 10.0f, totalElapsedTime, 13.0f);
                CP_Image_Draw(image_boulder, value_x_mid, ten.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&eleven, 11.0f, totalElapsedTime, 13.0f);
                CP_Image_Draw(image_boulder, value_x_mid, eleven.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&twelve, 12.0f, totalElapsedTime, 16.0f);
                CP_Image_Draw(image_boulder, value_x_right, twelve.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&thirt, 11.5f, totalElapsedTime, 15.0f);
                CP_Image_Draw(image_boulder, value_x_left, thirt.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fourt, 11.9f, totalElapsedTime, 15.0f);
                CP_Image_Draw(image_boulder, value_x_mid, fourt.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fifte, 12.7f, totalElapsedTime, 15.0f);
                CP_Image_Draw(image_boulder, value_x_right, fifte.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&sixte, 13.0, totalElapsedTime, 15.0f);
                CP_Image_Draw(image_boulder, value_x_mid, sixte.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&sevente, 15.0f, totalElapsedTime, 19.0f);
                CP_Image_Draw(image_boulder, value_x_right, sevente.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&eighte, 15.5f, totalElapsedTime, 15.0f);
                CP_Image_Draw(image_boulder, value_x_mid, eighte.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&ninete, 15.8f, totalElapsedTime, 15.0f);
                CP_Image_Draw(image_boulder, value_x_left, ninete.value_y, width * 0.13f, height * 0.09f, 255);

                if (first.invul_spawn == 1) {
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, first.invul_y, radius, 1) == 2) { invulnerable = 1; first.invul_spawn = 0; }
                };

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
                        health = 0;
                        //RECTANGLE INSTANT DEATH
                    }
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, eleven.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, twelve.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, thirt.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, fourt.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, fifte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, sixte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, sevente.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, eighte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, ninete.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
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
                spawnCheck(&first, 1.3f, totalElapsedTime, 8.3f);
                CP_Image_Draw(image_boulder, value_x_left, first.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&second, 1.3f, totalElapsedTime, 8.3f);
                CP_Image_Draw(image_boulder, value_x_right, second.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&third, 2.5f, totalElapsedTime, 19.0f);
                
                CP_Image_Draw(image_boulder, value_x_mid, third.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fourth, 3.5f, totalElapsedTime, 14.0f);
                CP_Image_Draw(image_boulder, value_x_left, fourth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fifth, 3.8f, totalElapsedTime, 14.0f);
                CP_Image_Draw(image_boulder, value_x_mid, fifth.value_y, width * 0.13f, height * 0.09f, 255);

                invulSpawn(&first, 4.8f, totalElapsedTime, 14.0f); //invulnerability
                if (first.invul_spawn == 1) {
                    CP_Image_Draw(image_invul, value_x_right, first.invul_y, width * 0.05f, height * 0.04f, 255);
                }

                spawnCheck(&sixth, 4.5f, totalElapsedTime, 14.0f);
                CP_Image_Draw(image_boulder, value_x_right, sixth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&seventh, 6.0f, totalElapsedTime, 14.0f);
                CP_Image_Draw(image_boulder, value_x_mid, seventh.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&eighth, 6.8f, totalElapsedTime, 13.5f);
                CP_Image_Draw(image_boulder, value_x_mid, eighth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&ninth, 7.6f, totalElapsedTime, 13.5f);
                CP_Image_Draw(image_boulder, value_x_mid, ninth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&ten, 6.9f, totalElapsedTime, 16.0f);
                CP_Image_Draw(image_boulder, value_x_left, ten.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&eleven, 7.7f, totalElapsedTime, 16.0f);
                CP_Image_Draw(image_boulder, value_x_right, eleven.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&twelve, 8.5f, totalElapsedTime, 16.0f);
                CP_Image_Draw(image_boulder, value_x_left, twelve.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&thirt, 11.0f, totalElapsedTime, 15.0f);
                CP_Image_Draw(image_boulder, value_x_right, thirt.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fourt, 12.5f, totalElapsedTime, 19.0f);
                CP_Image_Draw(image_boulder, value_x_mid, fourt.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fifte, 13.0f, totalElapsedTime, 19.0f);
                CP_Image_Draw(image_boulder, value_x_left, fifte.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&sixte, 14.0f, totalElapsedTime, 15.0f);
                CP_Image_Draw(image_boulder, value_x_mid, sixte.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&sevente, 15.0f, totalElapsedTime, 19.0f);
                CP_Image_Draw(image_boulder, value_x_left, sevente.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&eighte, 15.5f, totalElapsedTime, 19.0f);
                CP_Image_Draw(image_boulder, value_x_right, eighte.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&ninete, 16.0f, totalElapsedTime, 19.0f);
                CP_Image_Draw(image_boulder, value_x_mid, ninete.value_y, width * 0.13f, height * 0.09f, 255);

                if (first.invul_spawn == 1) {
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, first.invul_y, radius, 1) == 2) { invulnerable = 1; first.invul_spawn = 0; }
                };

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
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, fifth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, sixth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, seventh.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, eighth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, ninth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, ten.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, eleven.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, twelve.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, thirt.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, fourt.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, fifte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, sixte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, sevente.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, eighte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, ninete.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                }
            }
            else if (selection == -1) {
                //launch second medium segment instead
                spawnCheck(&first, 1.8f, totalElapsedTime, 16.0f);
                CP_Image_Draw(image_boulder, value_x_left, first.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&second, 2.6f, totalElapsedTime, 17.0f);
                CP_Image_Draw(image_boulder, value_x_right, second.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&third, 2.6f, totalElapsedTime, 17.0f);
                
                CP_Image_Draw(image_boulder, value_x_mid, third.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fourth, 3.2f, totalElapsedTime, 17.0f);
                CP_Image_Draw(image_boulder, value_x_right, fourth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fifth, 3.8f, totalElapsedTime, 17.0f);
                CP_Image_Draw(image_boulder, value_x_left, fifth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&sixth, 4.8f, totalElapsedTime, 15.0f);
                CP_Image_Draw(image_boulder, value_x_mid, sixth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&seventh, 5.0f, totalElapsedTime, 17.0f);
                CP_Image_Draw(image_boulder, value_x_right, seventh.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&eighth, 6.0f, totalElapsedTime, 15.0f);
                CP_Image_Draw(image_boulder, value_x_left, eighth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&ninth, 7.0f, totalElapsedTime, 17.0f);
                CP_Image_Draw(image_boulder, value_x_mid, ninth.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&ten, 8.0f, totalElapsedTime, 15.0f);
                //CP_Graphics_DrawCircle(value_x_right, ten.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, ten.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&eleven, 9.5f, totalElapsedTime, 15.0f);
                //CP_Graphics_DrawCircle(value_x_right, ten.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, eleven.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&twelve, 11.2f, totalElapsedTime, 15.0f);
                //CP_Graphics_DrawCircle(value_x_right, ten.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, twelve.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&thirt, 11.5f, totalElapsedTime, 15.0f);
                //CP_Graphics_DrawCircle(value_x_right, ten.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, thirt.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fourt, 13.0f, totalElapsedTime, 19.0f);
                //CP_Graphics_DrawCircle(value_x_right, ten.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, fourt.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&fifte, 14.0f, totalElapsedTime, 19.0f);
                //CP_Graphics_DrawCircle(value_x_right, ten.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, fifte.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&sixte, 14.4f, totalElapsedTime, 19.0f);
                //CP_Graphics_DrawCircle(value_x_right, ten.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, sixte.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&sevente, 14.8f, totalElapsedTime, 19.0f);
                //CP_Graphics_DrawCircle(value_x_right, ten.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, sevente.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&eighte, 15.2f, totalElapsedTime, 19.0f);
                //CP_Graphics_DrawCircle(value_x_right, ten.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, eighte.value_y, width * 0.13f, height * 0.09f, 255);

                spawnCheck(&ninete, 15.2f, totalElapsedTime, 19.0f);
                //CP_Graphics_DrawCircle(value_x_right, ten.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, ninete.value_y, width * 0.13f, height * 0.09f, 255);

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
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, eleven.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, twelve.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, thirt.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, fourt.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, fifte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, sixte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, sevente.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, eighte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, ninete.value_y, radius, 0) == 1) {
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

                
                spawnCheck(&first, 1.5f, totalElapsedTime, 20.0f + endgame);
                //CP_Graphics_DrawCircle(value_x_left, first.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, first.value_y, width * 0.13f, height * 0.09f, 255);

        
                spawnCheck(&second, 1.5f, totalElapsedTime, 20.0f + endgame);
                //CP_Graphics_DrawCircle(value_x_right, second.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, second.value_y, width * 0.13f, height * 0.09f, 255);

               
                spawnCheck(&third, 2.0f, totalElapsedTime, 20.0f + endgame);
                
                CP_Image_Draw(image_boulder, value_x_mid, third.value_y, width * 0.13f, height * 0.09f, 255);

              
                spawnCheck(&fourth, 3.7f, totalElapsedTime, 20.0f + endgame);
                //CP_Graphics_DrawCircle(value_x_left, fourth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, fourth.value_y, width * 0.13f, height * 0.09f, 255);

             
                spawnCheck(&fifth, 3.7f, totalElapsedTime, 20.0f + endgame);
                //CP_Graphics_DrawCircle(value_x_right, fifth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, fifth.value_y, width * 0.13f, height * 0.09f, 255);

                //invulnerability
                invulSpawn(&first, 4.1f, totalElapsedTime, 20.5f + endgame); //invulnerability
                if (first.invul_spawn == 1) {
                    CP_Image_Draw(image_invul, value_x_left, first.invul_y, width * 0.05f, height * 0.04f, 255);
                }

              
                spawnCheck(&sixth, 4.5f, totalElapsedTime, 20.0f + endgame);
                //CP_Graphics_DrawCircle(value_x_left, sixth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, sixth.value_y, width * 0.13f, height * 0.09f, 255);

              
                spawnCheck(&seventh, 4.5f, totalElapsedTime, 20.0f + endgame);
                //CP_Graphics_DrawCircle(value_x_mid, seventh.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, seventh.value_y, width * 0.13f, height * 0.09f, 255);

              
                spawnCheck(&eighth, 6.0f, totalElapsedTime, 20.0f + endgame);
                //CP_Graphics_DrawCircle(value_x_right, eighth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_right, eighth.value_y, width * 0.13f, height * 0.09f, 255);

             
                spawnCheck(&ninth, 6.5f, totalElapsedTime, 20.0f + endgame);
                //CP_Graphics_DrawCircle(value_x_mid, ninth.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_mid, ninth.value_y, width * 0.13f, height * 0.09f, 255);

               
                spawnCheck(&ten, 7.0f, totalElapsedTime, 20.0f + endgame);
                //CP_Graphics_DrawCircle(value_x_left, ten.value_y, radius * 4.0f);
                CP_Image_Draw(image_boulder, value_x_left, ten.value_y, width * 0.13f, height * 0.09f, 255);

                CP_Settings_RectMode(CP_POSITION_CENTER);
         
                spawnRect(&rect1, 7.6f, totalElapsedTime, 21.0f + endgame);
                //CP_Graphics_DrawRect(value_x_right, rect1.value_y, width * 0.15, height * 0.7);
                CP_Image_Draw(image_log, value_x_right, rect1.value_y, width * 0.15f, height * 0.7f, 255);

             
                spawnCheck(&eleven, 8.0f, totalElapsedTime, 21.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, eleven.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_mid, eleven.value_y, radius * 4.0f);

              
                spawnCheck(&twelve, 9.0f, totalElapsedTime, 22.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_left, twelve.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_left, twelve.value_y, radius * 4.0f);

           
                spawnCheck(&thirt, 10.0f, totalElapsedTime, 22.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, thirt.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_mid, thirt.value_y, radius * 4.0f);

               
                spawnCheck(&fourt, 11.5f, totalElapsedTime, 21.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, fourt.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_mid, fourt.value_y, radius * 4.0f);

            
                spawnCheck(&fifte, 12.5f, totalElapsedTime, 22.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_left, fifte.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_left, fifte.value_y, radius * 4.0f);

            
                spawnCheck(&sixte, 12.5f, totalElapsedTime, 22.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_right, sixte.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_right, sixte.value_y, radius * 4.0f);

       
                spawnCheck(&sevente, 14.5f, totalElapsedTime, 20.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, sevente.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_mid, sevente.value_y, radius * 4.0f);

             
                spawnCheck(&eighte, 14.5f, totalElapsedTime, 19.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_right, eighte.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_right, eighte.value_y, radius * 4.0f);

      
                spawnCheck(&ninete, 16.5f, totalElapsedTime, 19.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, ninete.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_mid, ninete.value_y, radius * 4.0f);

             
                spawnCheck(&twoZero, 16.5f, totalElapsedTime, 19.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_left, twoZero.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_left, twoZero.value_y, radius * 4.0f);

                if (first.invul_spawn == 1) {
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, first.invul_y, radius, 1) == 2) { invulnerable = 1; first.invul_spawn = 0; }
                };

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
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, eighth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, ninth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, ten.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (isRectCollided(current_position.x, current_position.y, radius, value_x_right, rect1.value_y, width * 0.15f, height * 0.7f) == 1) {
                        health = 0;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, eleven.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, twelve.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, thirt.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, fourt.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, fifte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, sixte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, sevente.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, eighte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, ninete.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, twoZero.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                }
            }

            else if (selection == -1) {
                //launch second hard segment instead
           
                spawnCheck(&first, 1.5f, totalElapsedTime, 20.5f + endgame);
                CP_Image_Draw(image_boulder, value_x_left, first.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_left, first.value_y, radius * 4.0f);

                spawnCheck(&second, 2.5f, totalElapsedTime, 20.5f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, second.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_mid, second.value_y, radius * 4.0f);

             
                spawnCheck(&third, 2.5f, totalElapsedTime, 20.5f + endgame);
                CP_Image_Draw(image_boulder, value_x_right, third.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_right, third.value_y, radius * 4.0f);

                spawnCheck(&fourth, 4.5f, totalElapsedTime, 21.5f + endgame);
                CP_Image_Draw(image_boulder, value_x_left, fourth.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_left, fourth.value_y, radius * 4.0f);

      
                spawnCheck(&fifth, 4.5f, totalElapsedTime, 21.5f + endgame);
                CP_Image_Draw(image_boulder, value_x_right, fifth.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_right, fifth.value_y, radius * 4.0f);

         
                spawnCheck(&sixth, 5.5f, totalElapsedTime, 21.5f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, sixth.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_mid, sixth.value_y, radius * 4.0f);

        
                dblPtsSpawn(&first, 4.7f, totalElapsedTime, 22.0f + endgame);//right side double points powerup here
                if (first.pts_boost_spawn == 1) {
                    CP_Image_Draw(image_double_meat, value_x_mid, first.pts_boost_y, width * 0.07f, height * 0.04f, 255);
                }

                CP_Settings_RectMode(CP_POSITION_CENTER);
             
                spawnRect(&rect1, 5.0f, totalElapsedTime, 21.0f + endgame);
                CP_Image_Draw(image_log, value_x_right, rect1.value_y, width * 0.15f, height * 0.6f, 255);
                //CP_Graphics_DrawRect(value_x_right, rect1.value_y, width * 0.15f, height * 0.6f);

          
                spawnCheck(&seventh, 6.0f, totalElapsedTime, 23.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, seventh.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_mid, seventh.value_y, radius * 4.0f);

          
                spawnCheck(&eighth, 6.5f, totalElapsedTime, 20.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_left, eighth.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_left, eighth.value_y, radius * 4.0f);

         
                spawnCheck(&ninth, 7.0f, totalElapsedTime, 20.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, ninth.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_mid, ninth.value_y, radius * 4.0f);

       
                spawnCheck(&ten, 8.0f, totalElapsedTime, 20.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_left, ten.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_left, ten.value_y, radius * 4.0f);

  
                spawnCheck(&eleven, 8.0f, totalElapsedTime, 20.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, eleven.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_mid, eleven.value_y, radius * 4.0f);

           
                spawnCheck(&twelve, 9.5f, totalElapsedTime, 20.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, twelve.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_mid, twelve.value_y, radius * 4.0f);

            
                spawnCheck(&thirt, 10.0f, totalElapsedTime, 20.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_left, thirt.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_left, thirt.value_y, radius * 4.0f);

            
                spawnCheck(&fourt, 10.7f, totalElapsedTime, 20.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_right, fourt.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_right, fourt.value_y, radius * 4.0f);

                CP_Settings_RectMode(CP_POSITION_CENTER);
              
                spawnRect(&rect2, 10.9f, totalElapsedTime, 21.0f + endgame);
                CP_Image_Draw(image_log, value_x_left, rect2.value_y, width * 0.15f, height * 0.6f, 255);
                //CP_Graphics_DrawRect(value_x_left, rect2.value_y, width * 0.15f, height * 0.6f);

                spawnCheck(&fifte, 11.0f, totalElapsedTime, 20.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_right, fifte.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_right, fifte.value_y, radius * 4.0f);

            
                spawnCheck(&sixte, 11.5f, totalElapsedTime, 20.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, sixte.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_mid, sixte.value_y, radius * 4.0f);

           
                spawnCheck(&sevente, 12.0f, totalElapsedTime, 20.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, sevente.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_mid, sevente.value_y, radius * 4.0f);

            
                spawnCheck(&eighte, 14.0f, totalElapsedTime, 20.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_right, eighte.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_right, eighte.value_y, radius * 4.0f);

             
                spawnCheck(&ninete, 15.5f, totalElapsedTime, 20.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, ninete.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_mid, ninete.value_y, radius * 4.0f);

        
                spawnCheck(&twoZero, 17.0f, totalElapsedTime, 20.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_left, twoZero.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_left, twoZero.value_y, radius * 4.0f);

                if (first.pts_boost_spawn == 1) {
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, first.pts_boost_y, radius, 1) == 2) { current_pts_increase = 1; first.pts_boost_spawn = 0; }
                };

                if (invulnerable == 0 && just_got_hit == 0) {
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, first.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, second.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, third.value_y, radius, 0) == 1) {
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
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, ninth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, ten.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, eleven.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, twelve.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, thirt.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, fourt.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, fifte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, sixte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, sevente.value_y, radius, 0) == 1) {
                        just_got_hit = 1; 
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, eighte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, ninete.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, twoZero.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (isRectCollided(current_position.x, current_position.y, radius, value_x_right, rect1.value_y, width * 0.15f, height * 0.6f) == 1) {
                        health = 0;
                    };
                    if (isRectCollided(current_position.x, current_position.y, radius, value_x_left, rect2.value_y, width * 0.15f, height * 0.6f) == 1) {
                        health = 0;
                    };
                }
            }
            else {
                //launch third hard segment instead
                CP_Settings_RectMode(CP_POSITION_CENTER);
           
                spawnRect(&rect1, 1.0f, totalElapsedTime, 20.0f + endgame);
                CP_Image_Draw(image_log, value_x_left, rect1.value_y, width * 0.15f, height * 0.6f, 255);
                //CP_Graphics_DrawRect(value_x_left, rect1.value_y, width * 0.15f, height * 0.6f);

                
                spawnCheck(&first, 1.0f, totalElapsedTime, 20.5f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, first.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_mid, first.value_y, radius * 4.0f);

               
                spawnCheck(&second, 2.0f, totalElapsedTime, 20.5f + endgame);
                CP_Image_Draw(image_boulder, value_x_right, second.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_right, second.value_y, radius * 4.0f);

           
                spawnCheck(&twoZero, 2.5f, totalElapsedTime, 20.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, twoZero.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_mid, twoZero.value_y, radius * 4.0f);

             
                spawnCheck(&third, 3.4f, totalElapsedTime, 21.5f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, third.value_y, width * 0.13f, height * 0.09f, 255);
                

              
                spawnCheck(&fourth, 3.4f, totalElapsedTime, 21.5f + endgame);
                CP_Image_Draw(image_boulder, value_x_right, fourth.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_right, fourth.value_y, radius * 4.0f);

              
                invulSpawn(&first, 3.0f, totalElapsedTime, 20.5f + endgame); //invulnerability
                if (first.invul_spawn == 1) {
                    CP_Image_Draw(image_invul, value_x_left, first.invul_y, width * 0.05f, height * 0.04f, 255);
                }

                CP_Settings_RectMode(CP_POSITION_CENTER);
              
                spawnRect(&rect2, 4.0f, totalElapsedTime, 21.0f + endgame);
                CP_Image_Draw(image_log, value_x_right, rect2.value_y, width * 0.15f, height * 0.8f, 255);
                //CP_Graphics_DrawRect(value_x_right, rect2.value_y, width * 0.15f, height * 0.8f);

              
                spawnCheck(&fifth, 4.2f, totalElapsedTime, 21.5f + endgame);
                CP_Image_Draw(image_boulder, value_x_left, fifth.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_left, fifth.value_y, radius * 4.0f);

              
                spawnCheck(&sixth, 4.6f, totalElapsedTime, 21.5f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, sixth.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_mid, sixth.value_y, radius * 4.0f);

             
                spawnCheck(&seventh, 5.4f, totalElapsedTime, 23.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_left, seventh.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_left, seventh.value_y, radius * 4.0f);

              
                spawnCheck(&eighth, 6.4f, totalElapsedTime, 22.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, eighth.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_mid, eighth.value_y, radius * 4.0f);

            
                dblPtsSpawn(&first, 7.0f, totalElapsedTime, 22.0f + endgame);//right side double points powerup here
                if (first.pts_boost_spawn == 1) {
                    CP_Image_Draw(image_double_meat, value_x_right, first.pts_boost_y, width * 0.07f, height * 0.04f, 255);
                }

                spawnCheck(&ninth, 7.5f, totalElapsedTime, 20.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, ninth.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_mid, ninth.value_y, radius * 4.0f);

               
                spawnCheck(&ten, 8.5f, totalElapsedTime, 23.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_right, ten.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_right, ten.value_y, radius * 4.0f);

             
                spawnCheck(&eleven, 8.5f, totalElapsedTime, 23.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_left, eleven.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_left, eleven.value_y, radius * 4.0f);

              
                spawnCheck(&twelve, 9.0f, totalElapsedTime, 23.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, twelve.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_mid, twelve.value_y, radius * 4.0f);

            
                spawnCheck(&thirt, 9.5f, totalElapsedTime, 23.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_left, thirt.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_left, thirt.value_y, radius * 4.0f);

            
                spawnCheck(&fourt, 9.5f, totalElapsedTime, 23.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_right, fourt.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_right, fourt.value_y, radius * 4.0f);

             
                spawnCheck(&fifte, 10.5f, totalElapsedTime, 25.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_mid, fifte.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_mid, fifte.value_y, radius * 4.0f);

           
                spawnCheck(&sixte, 11.0f, totalElapsedTime, 25.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_left, sixte.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_left, sixte.value_y, radius * 4.0f);

              
                spawnCheck(&sevente, 11.0f, totalElapsedTime, 25.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_right, sevente.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_right, sevente.value_y, radius * 4.0f);

                CP_Settings_RectMode(CP_POSITION_CENTER);
             
                spawnRect(&rect3, 13.0f, totalElapsedTime, 23.0f + endgame);
                CP_Image_Draw(image_log, value_x_mid, rect3.value_y, width * 0.15f, height * 0.8f, 255);
                //CP_Graphics_DrawRect(value_x_mid, rect3.value_y, width * 0.15f, height * 0.8f);

          
                spawnCheck(&eighte, 14.7f, totalElapsedTime, 20.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_right, eighte.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_right, eighte.value_y, radius * 4.0f);

                
                spawnCheck(&ninete, 16.0f, totalElapsedTime, 20.0f + endgame);
                CP_Image_Draw(image_boulder, value_x_left, ninete.value_y, width * 0.13f, height * 0.09f, 255);
                //CP_Graphics_DrawCircle(value_x_left, ninete.value_y, radius * 4.0f);

                if (first.pts_boost_spawn == 1) {
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, first.pts_boost_y, radius, 1) == 2) { current_pts_increase = 1; first.pts_boost_spawn = 0; }
                };
                if (first.invul_spawn == 1) {
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, first.invul_y, radius, 1) == 2) { invulnerable = 1; first.invul_spawn = 0; }
                };

                if (invulnerable == 0 && just_got_hit == 0) {

                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, first.value_y, radius, 0) == 1) {
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
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, seventh.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, eighth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, ninth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, ten.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, eleven.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, twelve.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, thirt.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, fourt.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, fifte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, sixte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, sevente.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, eighte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, ninete.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, twoZero.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (isRectCollided(current_position.x, current_position.y, radius, value_x_left, rect1.value_y, width * 0.15f, height * 0.6f) == 1) {
                        health = 0;
                    };
                    if (isRectCollided(current_position.x, current_position.y, radius, value_x_right, rect2.value_y, width * 0.15f, height * 0.8f) == 1) {
                        health = 0;
                    };
                    if (isRectCollided(current_position.x, current_position.y, radius, value_x_mid, rect3.value_y, width * 0.15f, height * 0.8f) == 1) {
                        health = 0;
                    };
                }
            }
        }

        // if range == 0, launch first hard segment
        // if range == 1, launch second hard segment instead
        // if range == 2, launch third hard segment instead

        else if (difficulty > 2) {
        checker = CP_Random_RangeInt(0, 2);
        selection = (checker == 0 && selection == 0) ? selection + 1 : selection;
        selection = (checker == 1 && selection == 0) ? selection - 1 : selection;
        selection = (checker == 2 && selection == 0) ? selection + 2 : selection;
        //random range is set to 0, 1 or 2
        if (1 == selection) {
            //launch first hard segment
         
            spawnCheck(&first, 1.5f, totalElapsedTime, 20.5f + endgame);
            CP_Image_Draw(image_boulder, value_x_left, first.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_left, first.value_y, radius * 4.0f);

      
            spawnCheck(&second, 2.5f, totalElapsedTime, 20.5f + endgame);
            CP_Image_Draw(image_boulder, value_x_mid, second.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_mid, second.value_y, radius * 4.0f);

           
            spawnCheck(&third, 2.5f, totalElapsedTime, 20.5f + endgame);
            CP_Image_Draw(image_boulder, value_x_right, third.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_right, third.value_y, radius * 4.0f);

         
            spawnCheck(&fourth, 4.5f, totalElapsedTime, 21.5f + endgame);
            CP_Image_Draw(image_boulder, value_x_left, fourth.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_left, fourth.value_y, radius * 4.0f);

        
            spawnCheck(&fifth, 4.5f, totalElapsedTime, 21.5f + endgame);
            CP_Image_Draw(image_boulder, value_x_right, fifth.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_right, fifth.value_y, radius * 4.0f);

        
            spawnCheck(&sixth, 5.5f, totalElapsedTime, 21.5f + endgame);
            CP_Image_Draw(image_boulder, value_x_mid, sixth.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_mid, sixth.value_y, radius * 4.0f);

            dblPtsSpawn(&first, 4.7f, totalElapsedTime, 22.0f + endgame);//right side double points powerup here
            if (first.pts_boost_spawn == 1) {
                CP_Image_Draw(image_double_meat, value_x_mid, first.pts_boost_y, width * 0.07f, height * 0.04f, 255);
            }

            CP_Settings_RectMode(CP_POSITION_CENTER);
        
            spawnRect(&rect1, 5.0f, totalElapsedTime, 21.0f + endgame);
            CP_Image_Draw(image_log, value_x_right, rect1.value_y, width * 0.15f, height * 0.6f, 255);
            //CP_Graphics_DrawRect(value_x_right, rect1.value_y, width * 0.15f, height * 0.6f);

        
            spawnCheck(&seventh, 6.0f, totalElapsedTime, 23.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_mid, seventh.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_mid, seventh.value_y, radius * 4.0f);

       
            spawnCheck(&eighth, 6.5f, totalElapsedTime, 20.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_left, eighth.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_left, eighth.value_y, radius * 4.0f);

           
            spawnCheck(&ninth, 7.0f, totalElapsedTime, 20.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_mid, ninth.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_mid, ninth.value_y, radius * 4.0f);

            spawnCheck(&ten, 8.0f, totalElapsedTime, 20.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_left, ten.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_left, ten.value_y, radius * 4.0f);

        
            spawnCheck(&eleven, 8.0f, totalElapsedTime, 20.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_mid, eleven.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_mid, eleven.value_y, radius * 4.0f);

        
            spawnCheck(&twelve, 9.5f, totalElapsedTime, 20.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_mid, twelve.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_mid, twelve.value_y, radius * 4.0f);

         
            spawnCheck(&thirt, 10.0f, totalElapsedTime, 20.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_left, thirt.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_left, thirt.value_y, radius * 4.0f);

        
            spawnCheck(&fourt, 10.7f, totalElapsedTime, 20.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_right, fourt.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_right, fourt.value_y, radius * 4.0f);

            CP_Settings_RectMode(CP_POSITION_CENTER);
      
            spawnRect(&rect2, 10.9f, totalElapsedTime, 21.0f + endgame);
            CP_Image_Draw(image_log, value_x_left, rect2.value_y, width * 0.15f, height * 0.6f, 255);
            //CP_Graphics_DrawRect(value_x_left, rect2.value_y, width * 0.15f, height * 0.6f);

       
            spawnCheck(&fifte, 11.0f, totalElapsedTime, 20.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_right, fifte.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_right, fifte.value_y, radius * 4.0f);

  
            spawnCheck(&sixte, 11.5f, totalElapsedTime, 20.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_mid, sixte.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_mid, sixte.value_y, radius * 4.0f);

   
            spawnCheck(&sevente, 12.0f, totalElapsedTime, 20.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_right, sevente.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_right, sevente.value_y, radius * 4.0f);

            
            spawnCheck(&eighte, 14.0f, totalElapsedTime, 20.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_right, eighte.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_right, eighte.value_y, radius * 4.0f);

         
            spawnCheck(&ninete, 15.5f, totalElapsedTime, 20.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_mid, ninete.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_mid, ninete.value_y, radius * 4.0f);

  
            spawnCheck(&twoZero, 17.0f, totalElapsedTime, 20.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_left, twoZero.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_left, twoZero.value_y, radius * 4.0f);

            if (first.pts_boost_spawn == 1) {
                if (iscirclecollided(current_position.x, current_position.y, value_x_mid, first.pts_boost_y, radius, 1) == 2) { current_pts_increase = 1; first.pts_boost_spawn = 0; }
            };

            if (invulnerable == 0 && just_got_hit == 0) {
                if (iscirclecollided(current_position.x, current_position.y, value_x_left, first.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_mid, second.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_right, third.value_y, radius, 0) == 1) {
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
                if (iscirclecollided(current_position.x, current_position.y, value_x_mid, ninth.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_left, ten.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_mid, eleven.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_mid, twelve.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_left, thirt.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_right, fourt.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_right, fifte.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_mid, sixte.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_right, sevente.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_right, eighte.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_mid, ninete.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_left, twoZero.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (isRectCollided(current_position.x, current_position.y, radius, value_x_right, rect1.value_y, width * 0.15f, height * 0.6f) == 1) {
                    health = 0;
                };
                if (isRectCollided(current_position.x, current_position.y, radius, value_x_left, rect2.value_y, width * 0.15f, height * 0.6f) == 1) {
                    health = 0;
                };

            }
        }
        else if (selection == -1) {
            //launch second hard segment instead
            CP_Settings_RectMode(CP_POSITION_CENTER);
         
            spawnRect(&rect1, 1.0f, totalElapsedTime, 20.0f + endgame);
            CP_Image_Draw(image_log, value_x_left, rect1.value_y, width * 0.15f, height * 0.6f, 255);
            //CP_Graphics_DrawRect(value_x_left, rect1.value_y, width * 0.15f, height * 0.6f);

    
            spawnCheck(&first, 1.0f, totalElapsedTime, 20.5f + endgame);
            CP_Image_Draw(image_boulder, value_x_mid, first.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_mid, first.value_y, radius * 4.0f);

     
            spawnCheck(&second, 2.0f, totalElapsedTime, 20.5f + endgame);
            CP_Image_Draw(image_boulder, value_x_right, second.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_right, second.value_y, radius * 4.0f);

       
            spawnCheck(&twoZero, 2.5f, totalElapsedTime, 20.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_mid, twoZero.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_mid, twoZero.value_y, radius * 4.0f);

          
            spawnCheck(&third, 3.4f, totalElapsedTime, 21.5f + endgame);
            CP_Image_Draw(image_boulder, value_x_mid, third.value_y, width * 0.13f, height * 0.09f, 255);
            

   
            spawnCheck(&fourth, 3.4f, totalElapsedTime, 21.5f + endgame);
            CP_Image_Draw(image_boulder, value_x_right, fourth.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_right, fourth.value_y, radius * 4.0f);

            invulSpawn(&first, 3.0f, totalElapsedTime, 20.5f + endgame); //invulnerability
            if (first.invul_spawn == 1) {
                CP_Image_Draw(image_invul, value_x_left, first.invul_y, width * 0.05f, height * 0.04f, 255);
            }

            CP_Settings_RectMode(CP_POSITION_CENTER);
           
            spawnRect(&rect2, 4.0f, totalElapsedTime, 21.0f + endgame);
            CP_Image_Draw(image_log, value_x_right, rect2.value_y, width * 0.15f, height * 0.8f, 255);
            //CP_Graphics_DrawRect(value_x_right, rect2.value_y, width * 0.15f, height * 0.8f);

       
            spawnCheck(&fifth, 4.2f, totalElapsedTime, 21.5f + endgame);
            CP_Image_Draw(image_boulder, value_x_left, fifth.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_left, fifth.value_y, radius * 4.0f);

         
            spawnCheck(&sixth, 4.6f, totalElapsedTime, 21.5f + endgame);
            CP_Image_Draw(image_boulder, value_x_mid, sixth.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_mid, sixth.value_y, radius * 4.0f);

      
            spawnCheck(&seventh, 5.4f, totalElapsedTime, 23.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_left, seventh.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_left, seventh.value_y, radius * 4.0f);

         
            spawnCheck(&eighth, 6.4f, totalElapsedTime, 22.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_mid, eighth.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_mid, eighth.value_y, radius * 4.0f);

        
            dblPtsSpawn(&first, 7.0f, totalElapsedTime, 22.0f + endgame);//right side double points powerup here
            if (first.pts_boost_spawn == 1) {
                CP_Image_Draw(image_double_meat, value_x_right, first.pts_boost_y, width * 0.07f, height * 0.04f, 255);
            }

       
            spawnCheck(&ninth, 7.5f, totalElapsedTime, 20.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_mid, ninth.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_mid, ninth.value_y, radius * 4.0f);

     
            spawnCheck(&ten, 8.5f, totalElapsedTime, 23.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_right, ten.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_right, ten.value_y, radius * 4.0f);

      
            spawnCheck(&eleven, 8.5f, totalElapsedTime, 23.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_left, eleven.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_left, eleven.value_y, radius * 4.0f);

         
            spawnCheck(&twelve, 9.0f, totalElapsedTime, 23.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_mid, twelve.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_mid, twelve.value_y, radius * 4.0f);

      
            spawnCheck(&thirt, 9.5f, totalElapsedTime, 23.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_left, thirt.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_left, thirt.value_y, radius * 4.0f);

      
            spawnCheck(&fourt, 9.5f, totalElapsedTime, 23.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_right, fourt.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_right, fourt.value_y, radius * 4.0f);

       
            spawnCheck(&fifte, 10.5f, totalElapsedTime, 25.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_mid, fifte.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_mid, fifte.value_y, radius * 4.0f);

        
            spawnCheck(&sixte, 11.0f, totalElapsedTime, 25.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_left, sixte.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_left, sixte.value_y, radius * 4.0f);

         
            spawnCheck(&sevente, 11.0f, totalElapsedTime, 25.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_right, sevente.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_right, sevente.value_y, radius * 4.0f);

            CP_Settings_RectMode(CP_POSITION_CENTER);
         
            spawnRect(&rect3, 13.0f, totalElapsedTime, 23.0f + endgame);
            CP_Image_Draw(image_log, value_x_mid, rect3.value_y, width * 0.15f, height * 0.8f, 255);
            //CP_Graphics_DrawRect(value_x_mid, rect3.value_y, width * 0.15f, height * 0.8f);

         
            spawnCheck(&eighte, 14.7f, totalElapsedTime, 20.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_right, eighte.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_right, eighte.value_y, radius * 4.0f);

           
            spawnCheck(&ninete, 16.0f, totalElapsedTime, 20.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_left, ninete.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_left, ninete.value_y, radius * 4.0f);

            if (first.pts_boost_spawn == 1) {
                if (iscirclecollided(current_position.x, current_position.y, value_x_right, first.pts_boost_y, radius, 1) == 2) { current_pts_increase = 1; first.pts_boost_spawn = 0; }
            };

            if (invulnerable == 0 && just_got_hit == 0) {

                if (iscirclecollided(current_position.x, current_position.y, value_x_mid, first.value_y, radius, 0) == 1) {
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
                if (iscirclecollided(current_position.x, current_position.y, value_x_left, seventh.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_mid, eighth.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_mid, ninth.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_right, ten.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_left, eleven.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_mid, twelve.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_left, thirt.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_right, fourt.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_mid, fifte.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_left, sixte.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_right, sevente.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_right, eighte.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_left, ninete.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (iscirclecollided(current_position.x, current_position.y, value_x_mid, twoZero.value_y, radius, 0) == 1) {
                    just_got_hit = 1;
                };
                if (isRectCollided(current_position.x, current_position.y, radius, value_x_left, rect1.value_y, width * 0.15f, height * 0.6f) == 1) {
                    health = 0;
                };
                if (isRectCollided(current_position.x, current_position.y, radius, value_x_right, rect2.value_y, width * 0.15f, height * 0.8f) == 1) {
                    health = 0;
                };
                if (isRectCollided(current_position.x, current_position.y, radius, value_x_mid, rect3.value_y, width * 0.15f, height * 0.8f) == 1) {
                    health = 0;
                };
                if (first.invul_spawn == 1) {
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, first.invul_y, radius, 1) == 2) { invulnerable = 1; first.invul_spawn = 0; }
                };
            }
        }
        else if (selection == 2) {
            //launch third hard segment instead
           
            spawnCheck(&first, 1.5f, totalElapsedTime, 20.0f + endgame);
            //CP_Graphics_DrawCircle(value_x_left, first.value_y, radius * 4.0f);
            CP_Image_Draw(image_boulder, value_x_left, first.value_y, width * 0.13f, height * 0.09f, 255);

            spawnCheck(&second, 1.5f, totalElapsedTime, 20.0f + endgame);
            //CP_Graphics_DrawCircle(value_x_right, second.value_y, radius * 4.0f);
            CP_Image_Draw(image_boulder, value_x_right, second.value_y, width * 0.13f, height * 0.09f, 255);

      
            spawnCheck(&third, 2.0f, totalElapsedTime, 20.0f + endgame);
            
            CP_Image_Draw(image_boulder, value_x_mid, third.value_y, width * 0.13f, height * 0.09f, 255);

          
            spawnCheck(&fourth, 3.7f, totalElapsedTime, 20.0f + endgame);
            //CP_Graphics_DrawCircle(value_x_left, fourth.value_y, radius * 4.0f);
            CP_Image_Draw(image_boulder, value_x_left, fourth.value_y, width * 0.13f, height * 0.09f, 255);

          
            spawnCheck(&fifth, 3.7f, totalElapsedTime, 20.0f + endgame);
            //CP_Graphics_DrawCircle(value_x_right, fifth.value_y, radius * 4.0f);
            CP_Image_Draw(image_boulder, value_x_right, fifth.value_y, width * 0.13f, height * 0.09f, 255);

           
            invulSpawn(&first, 4.1f, totalElapsedTime, 20.5f + endgame); //invulnerability
            if (first.invul_spawn == 1) {
                CP_Image_Draw(image_invul, value_x_left, first.invul_y, width * 0.05f, height * 0.04f, 255);
            }

      
            spawnCheck(&sixth, 4.5f, totalElapsedTime, 20.0f + endgame);
            //CP_Graphics_DrawCircle(value_x_left, sixth.value_y, radius * 4.0f);
            CP_Image_Draw(image_boulder, value_x_left, sixth.value_y, width * 0.13f, height * 0.09f, 255);

           
            spawnCheck(&seventh, 4.5f, totalElapsedTime, 20.0f + endgame);
            //CP_Graphics_DrawCircle(value_x_mid, seventh.value_y, radius * 4.0f);
            CP_Image_Draw(image_boulder, value_x_mid, seventh.value_y, width * 0.13f, height * 0.09f, 255);

         
            spawnCheck(&eighth, 6.0f, totalElapsedTime, 20.0f + endgame);
            //CP_Graphics_DrawCircle(value_x_right, eighth.value_y, radius * 4.0f);
            CP_Image_Draw(image_boulder, value_x_right, eighth.value_y, width * 0.13f, height * 0.09f, 255);

         
            spawnCheck(&ninth, 6.5f, totalElapsedTime, 20.0f + endgame);
            //CP_Graphics_DrawCircle(value_x_mid, ninth.value_y, radius * 4.0f);
            CP_Image_Draw(image_boulder, value_x_mid, ninth.value_y, width * 0.13f, height * 0.09f, 255);

          
            spawnCheck(&ten, 7.0f, totalElapsedTime, 20.0f + endgame);
            //CP_Graphics_DrawCircle(value_x_left, ten.value_y, radius * 4.0f);
            CP_Image_Draw(image_boulder, value_x_left, ten.value_y, width * 0.13f, height * 0.09f, 255);

            CP_Settings_RectMode(CP_POSITION_CENTER);
            
            spawnRect(&rect1, 7.6f, totalElapsedTime, 21.0f + endgame);
            //CP_Graphics_DrawRect(value_x_right, rect1.value_y, width * 0.15, height * 0.7);
            CP_Image_Draw(image_log, value_x_right, rect1.value_y, width * 0.15f, height * 0.7f, 255);

          
            spawnCheck(&eleven, 8.0f, totalElapsedTime, 21.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_mid, eleven.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_mid, eleven.value_y, radius * 4.0f);

         
            spawnCheck(&twelve, 9.0f, totalElapsedTime, 22.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_left, twelve.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_left, twelve.value_y, radius * 4.0f);

         
            spawnCheck(&thirt, 10.0f, totalElapsedTime, 22.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_mid, thirt.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_mid, thirt.value_y, radius * 4.0f);

           
            spawnCheck(&fourt, 11.5f, totalElapsedTime, 21.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_mid, fourt.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_mid, fourt.value_y, radius * 4.0f);

      
            spawnCheck(&fifte, 12.5f, totalElapsedTime, 22.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_left, fifte.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_left, fifte.value_y, radius * 4.0f);

            spawnCheck(&sixte, 12.5f, totalElapsedTime, 22.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_right, sixte.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_right, sixte.value_y, radius * 4.0f);

           
            spawnCheck(&sevente, 14.5f, totalElapsedTime, 20.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_mid, sevente.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_mid, sevente.value_y, radius * 4.0f);

         
            spawnCheck(&eighte, 14.5f, totalElapsedTime, 19.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_right, eighte.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_right, eighte.value_y, radius * 4.0f);

          
            spawnCheck(&ninete, 16.5f, totalElapsedTime, 19.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_mid, ninete.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_mid, ninete.value_y, radius * 4.0f);

           
            spawnCheck(&twoZero, 16.5f, totalElapsedTime, 19.0f + endgame);
            CP_Image_Draw(image_boulder, value_x_left, twoZero.value_y, width * 0.13f, height * 0.09f, 255);
            //CP_Graphics_DrawCircle(value_x_left, twoZero.value_y, radius * 4.0f);

                if (first.invul_spawn == 1) {
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, first.invul_y, radius, 1) == 2) { invulnerable = 1; first.invul_spawn = 0; }
                };

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
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, eighth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, ninth.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, ten.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (isRectCollided(current_position.x, current_position.y, radius, value_x_right, rect1.value_y, width * 0.15f, height * 0.7f) == 1) {
                        health = 0;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, eleven.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, twelve.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, thirt.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, fourt.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, fifte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, sixte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, sevente.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_right, eighte.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_mid, ninete.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                    if (iscirclecollided(current_position.x, current_position.y, value_x_left, twoZero.value_y, radius, 0) == 1) {
                        just_got_hit = 1;
                    };
                }
            }
            // if range == 0, launch first hard segment
            // if range == 1, launch second hard segment instead
            // if range == 2, launch third hard segment instead
        }

        //Displaying Points
        static int blues = 0;
        CP_Settings_TextSize(width * 0.05f);
        CP_Settings_Fill(black);
        char buffer[16] = { 0 };
        sprintf_s(buffer, _countof(buffer), "%d", points);
        CP_Font_DrawText("Points:", width * 0.05f, height - height * 0.05f);
        CP_Font_DrawText(buffer, width * 0.19f, height - height * 0.05f);

        // Colour Pulse when Double Points is active
        if (current_pts_increase == 1) {
            CP_Settings_Fill(CP_Color_Create(0, 0, blues, 255));
            CP_Font_DrawText("Points:", width * 0.05f, height - height * 0.05f);
            CP_Font_DrawText(buffer, width * 0.19f, height - height * 0.05f);

            if (upordown == 1) {
                blues += (int)bluemulti;
                if (blues >= 255) {
                    upordown = 0;
                }
            }
            if (upordown == 0) {
                blues -= (int)bluemulti;
                if (blues <= 0) {
                    upordown = 1;
                }
            }
        }
        
        // visual cue for when player has increased points active
        if (current_pts_increase == 1) {
            static int alpha = 255;
            CP_Image_Draw(dblptsimg, current_position.x, current_position.y * 0.9f, width * 0.25f, height * 0.02f, alpha);

            if (alpha > 0) alpha -= (int)alphamulti;
            if (alpha <= 0) {
                alpha = 255;
            }
        }

        // visual cue for when player is INVINCIBLE
        if (invulnerable == 1) {
            static int inAlpha = 255;
            CP_Image_Draw(invulimg, current_position.x, current_position.y * 1.1f, width * 0.25f, height * 0.02f, inAlpha);

            if (inAlpha > 0) inAlpha -= (int)invulmulti;
            if (inAlpha <= 0) {
                inAlpha = 255;
            }
        }

        
        CP_Settings_Fill(red);
        CP_Settings_RectMode(CP_POSITION_CENTER);
        if (health >= 3) {
            //CP_Graphics_DrawRect(windows_length / 20 * 15, windows_height / 20, windows_length / 20, windows_height / 40);
            CP_Image_Draw(image_heart, width * 0.75f, height - height * 0.05f, width * 0.07f, height * 0.04f, 255);
        }
        if (health >= 2) {
            //CP_Graphics_DrawRect(windows_length / 20 * 17, windows_height / 20, windows_length / 20, windows_height / 40); 
            CP_Image_Draw(image_heart, width * 0.85f, height - height * 0.05f, width * 0.07f, height * 0.04f, 255);
        }
        if (health >= 1) {

            //CP_Graphics_DrawRect(windows_length / 20 * 19, windows_height / 20, windows_length / 20, windows_height / 40); 
            CP_Image_Draw(image_heart, width * 0.95f, height - height * 0.05f, width * 0.07f, height * 0.04f, 255);
        }
        if (health <= 0) {
            //dies
            endgamestart = 0;
            totalEndgameTime = 0;
            endgame = 0;
            rotation = 0;
            totalElapsedTime = 0;
            score += points;
            health_toggle = 0;
            CP_Engine_SetNextGameStateForced(Death_Screen_Init, Death_Screen_Update, Death_Screen_Exit);
        }

        if (health != 0 && multiplier == 1) { //the system for points based on times/ per frame
            points++;
        }
        else if (health != 0 && multiplier == 3) {
            points += 30;
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

        if (just_got_hit == 1) { //system to make player invulnerable right after getting hit
            ++health_toggle;
            just_got_hit_timer += currentElapsedTime;
            if (rot_counter >= 0.1f) {
                rot_counter -= 0.1f;
            }

            rot_counter += CP_System_GetDt();
            rotation = CP_Math_LerpFloat(0.0f, 15.0f, rot_counter);

            playervisible = 100;
            if (just_got_hit_timer >= 1.0f) {
                rotation = 0;
                playervisible = 255;
                health_toggle = 0;
                just_got_hit = 0; //turns off invulnerable flag
                just_got_hit_timer = 0.0f; //resets timer
            }
        }

        if (health_toggle == 1) {
            --health;
            CP_Sound_PlayAdvanced(damagesound, volume * 2.0f, 1.0f, FALSE, CP_SOUND_GROUP_2);

        }
    }

    if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
        gIsPaused = !gIsPaused;
    }

    if (gIsPaused == TRUE) {
        float retainedTime = retainTime(&totalElapsedTime);

        CP_Sound_PauseAll();
        CP_Settings_Fill(pause);
        CP_Graphics_DrawRect(CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.5f, (float)width, (float)height);
        CP_Image_Draw(image_pause_background, (float)width / 2.0f, (float)height / 2.0f, (float)width, (float)height, 255);

        // set the rectangle drawing mode to CENTER
        CP_Settings_RectMode(CP_POSITION_CENTER);
        CP_Settings_Fill(blue);
        // draw a rectangle at the center of the screen, half the size of the screen

        CP_Image_Draw(image_resume, resumeX, resumeY, resumeWidth, resumeHeight, 255);
        CP_Image_Draw(image_restart, restartX, restartY, restartWidth, restartHeight, 255);
        CP_Image_Draw(image_mainmenu, b2mmX, b2mmY, b2mmWidth, b2mmHeight, 255);

        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
            if (areaClick(resumeX, resumeY, resumeWidth, resumeHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
                gIsPaused = FALSE;
            }
            if (areaClick(restartX, restartY, restartWidth, restartHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
                gIsPaused = FALSE;
                points = 0;
                currentElapsedTime = 0;
                endgamestart = 0;
                totalEndgameTime = 0;
                endgame = 0;
                totalElapsedTime = 0;
                currentElapsedTime = 0;
                difficulty = 0;
                CP_Sound_StopAll();
                CP_Engine_SetNextGameStateForced(game_init, game_update, game_update);
            }
            if (areaClick(b2mmX, b2mmY, b2mmWidth, b2mmHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
                gIsPaused = FALSE;
                currentElapsedTime = 0;
                endgamestart = 0;
                totalEndgameTime = 0;
                endgame = 0;
                points = 0;
                totalElapsedTime = 0;
                currentElapsedTime = 0;
                difficulty = 0;
                CP_Sound_StopAll();
                CP_Engine_SetNextGameStateForced(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);

            }
        }

        if (CP_Input_KeyTriggered(KEY_R)) {
            gIsPaused = FALSE;
            currentElapsedTime = 0;
            endgamestart = 0;
            totalEndgameTime = 0;
            endgame = 0;
            points = 0;
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
    CP_Image_Free(&dblptsimg);
    CP_Image_Free(&invulimg);
}
