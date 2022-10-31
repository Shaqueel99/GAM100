#include "cprocessing.h"


/* Forward declarations */
//void game_init(void);
//void game_update(void);
//void game_exit(void);

void Main_Menu_Init(void);
void Main_Menu_Update(void);
void Main_Menu_Exit(void);

/* Entry point */
int main(void)
{
    // CP_Engine_SetNextGameState(game_init, game_update, game_exit);
    CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
    CP_Engine_Run();
    return 0;
}
