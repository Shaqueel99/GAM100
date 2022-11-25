/*----------------------------------------------------------------------------------
All content © 2022 DigiPen Institute of Technology

FILE NAME:                  main.c

----------------------------------------------------------------------------------*/

#include "cprocessing.h"

void Main_Menu_Init(void);
void Main_Menu_Update(void);
void Main_Menu_Exit(void);

int main(void)
{
    CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
    CP_Engine_Run();
    return 0;
}
