#ifndef WALKTEST_POSES
#define WALKTEST_POSES

#include <avr/pgmspace.h>

PROGMEM prog_uint16_t move1[] = {12, 474, 478, 381, 684, 666, 276, 464, 475, 402, 647, 701, 300};
PROGMEM prog_uint16_t move2[] = {12, 566, 414, 568, 626, 373, 343, 637, 558, 402, 569, 752, 440};
PROGMEM prog_uint16_t move3[] = {12, 567, 576, 564, 487, 370, 561, 543, 507, 484, 548, 506, 449};
PROGMEM prog_uint16_t move4[] = {12, 538, 528, 397, 626, 642, 395, 523, 511, 408, 635, 672, 325};
PROGMEM prog_uint16_t Stand1[] = {12, 512, 510, 503, 519, 507, 516, 510, 511, 506, 519, 509, 517};

PROGMEM transition_t seq1[] = {{0,4} ,{move1,500} ,{move2,500} ,{move3,500} ,{move4,500} };

#endif
