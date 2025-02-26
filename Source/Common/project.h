/****************************************************************************************************
 * Pragma
 ****************************************************************************************************/

#pragma once

/****************************************************************************************************
 * Define
 ****************************************************************************************************/

#ifdef CEEDLING
  #define PROJECT_INFINITE_LOOP
  #define PROJECT_MAIN                MAIN
  #define PROJECT_STATIC(DECLARATION) extern DECLARATION; DECLARATION
#else
  #define PROJECT_INFINITE_LOOP       while(1)
  #define PROJECT_MAIN                main
  #define PROJECT_STATIC(DECLARATION) static DECLARATION
#endif
