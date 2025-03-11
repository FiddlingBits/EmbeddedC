/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "application.h"
#include "project.h"
#include <stdlib.h>
#include "system.h"

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Project Main ***/
void PROJECT_MAIN(void)
{
    /*** Project Main ***/
    /* Initialize */
    system_init(free, malloc);

    /* Run */
    PROJECT_INFINITE_LOOP
    {
    }
}
