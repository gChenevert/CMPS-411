#ifndef SPIDEY_POSES
#define SPIDEY_POSES

#include <avr/pgmspace.h>

PROGMEM prog_uint16_t Stand[] = {12, 566, 449, 384, 643, 733, 289, 453, 572, 454, 635, 633, 376};
PROGMEM prog_uint16_t Move2[] = {12, 480, 448, 391, 634, 742, 285, 403, 455, 541, 633, 568, 276};
PROGMEM prog_uint16_t Move3[] = {12, 546, 449, 387, 626, 739, 281, 402, 348, 328, 635, 583, 200};
PROGMEM prog_uint16_t Move4[] = {12, 544, 448, 392, 632, 742, 284, 401, 349, 569, 561, 531, 332};

PROGMEM transition_t Count[] = {{0,15} ,{Stand,500} ,{Move4,500} ,{Move2,500} ,{Move3,500} ,{Move4,500} ,{Move2,500} ,{Move3,500} ,{Move4,500} ,{Move2,500} ,{Move3,500} ,{Move4,500} ,{Move2,500} ,{Move4,500} ,{Move3,500} ,{Move4,500} };

#endif
