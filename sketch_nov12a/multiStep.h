#ifndef WALKTEST_POSES
#define WALKTEST_POSES

#include <avr/pgmspace.h>

PROGMEM prog_uint16_t a[] = {12, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512};
PROGMEM prog_uint16_t firstleg[] = {12, 398, 478, 373, 698, 644, 285, 470, 483, 378, 677, 686, 306};
PROGMEM prog_uint16_t thirdleg[] = {12, 377, 594, 370, 686, 644, 285, 429, 644, 358, 687, 682, 308};
PROGMEM prog_uint16_t fifthleg[] = {12, 437, 592, 366, 650, 789, 178, 317, 643, 450, 595, 578, 378};
PROGMEM prog_uint16_t fourthleg[] = {12, 377, 594, 370, 701, 644, 285, 318, 646, 374, 684, 687, 308};
PROGMEM prog_uint16_t seventhleg[] = {12, 571, 592, 354, 654, 772, 167, 316, 485, 446, 620, 576, 376};
PROGMEM prog_uint16_t step6[] = {12, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512};
PROGMEM prog_uint16_t step5[] = {12, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512};
PROGMEM prog_uint16_t step4[] = {12, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512};
PROGMEM prog_uint16_t step3[] = {12, 545, 386, 460, 621, 545, 410, 652, 509, 426, 656, 561, 156};
PROGMEM prog_uint16_t step2[] = {12, 507, 385, 329, 666, 771, 178, 673, 503, 434, 609, 647, 194};
PROGMEM prog_uint16_t step1[] = {12, 536, 667, 395, 623, 640, 399, 320, 510, 404, 630, 674, 322};
PROGMEM prog_uint16_t eigthleg[] = {12, 570, 546, 354, 677, 772, 354, 318, 484, 477, 619, 580, 376};
PROGMEM prog_uint16_t Stand1[] = {12, 512, 510, 503, 519, 507, 516, 510, 511, 506, 519, 509, 517};
PROGMEM prog_uint16_t sixthleg[] = {12, 571, 592, 361, 652, 779, 173, 317, 645, 451, 620, 578, 378};
PROGMEM prog_uint16_t move1[] = {12, 474, 478, 381, 684, 666, 276, 464, 475, 402, 647, 701, 300};
PROGMEM prog_uint16_t move2[] = {12, 566, 414, 568, 626, 373, 343, 637, 558, 402, 569, 752, 440};
PROGMEM prog_uint16_t move3[] = {12, 567, 576, 564, 487, 370, 561, 543, 507, 484, 548, 506, 449};
PROGMEM prog_uint16_t move4[] = {12, 538, 528, 397, 626, 642, 395, 523, 511, 408, 635, 672, 325};
PROGMEM prog_uint16_t move5[] = {12, 542, 541, 475, 548, 429, 495, 543, 500, 371, 540, 803, 263};
PROGMEM prog_uint16_t move6[] = {12, 564, 543, 459, 519, 732, 228, 545, 500, 427, 607, 538, 467};
PROGMEM prog_uint16_t secondleg[] = {12, 381, 478, 370, 699, 644, 285, 451, 644, 374, 699, 684, 306};
PROGMEM prog_uint16_t ninthleg[] = {12, 571, 546, 354, 679, 776, 356, 478, 480, 480, 624, 578, 378};


PROGMEM prog_uint16_t turn1[] = {12, 197, 510, 503, 519, 507, 516, 510, 511, 506, 519, 509, 517};
PROGMEM prog_uint16_t turn2[] = {12, 197, 510, 503, 519, 507, 516, 510, 197, 506, 519, 509, 517};
PROGMEM prog_uint16_t turn3[] = {12, 512, 197, 503, 519, 507, 516, 510, 511, 506, 519, 509, 517};
PROGMEM prog_uint16_t turn4[] = {12, 512, 197, 503, 519, 507, 516, 197, 511, 506, 519, 509, 517};

PROGMEM prog_uint16_t lean[] = {12, 512, 510, 503, 197, 507, 516, 510, 511, 506, 197, 509, 517};

PROGMEM transition_t leanSeq[] = {{0,2}, {Stand1, 500}, {lean, 500}};
PROGMEM transition_t turnSeq[] = {{0,5} ,{Stand1,500} ,{turn1,500} ,{turn2,500}, {turn3,500}, {turn4,500}};
PROGMEM transition_t seq3[] = {{0,10} ,{move1,500} ,{firstleg,500} ,{secondleg,500} ,{thirdleg,500} ,{fourthleg,500} ,{fifthleg,500} ,{sixthleg,500} ,{seventhleg,500} ,{eigthleg,500} ,{ninthleg,500} };
PROGMEM transition_t seq2[] = {{0,6} ,{move1,500} ,{move2,500} ,{move3,500} ,{move5,500} ,{move6,500} ,{move4,500} };
PROGMEM transition_t seq1[] = {{0,4} ,{move1,500} ,{move2,500} ,{move3,500} ,{move4,500} };
PROGMEM transition_t seq4[] = {{0,30} ,{move1,500} ,{step1,500} ,{step2,500},{move1,500} ,{step1,500} ,{step2,500},{move1,500} ,{step1,500} ,{step2,500},{move1,500} ,{step1,500} ,{step2,500},{move1,500} ,{step1,500} ,{step2,500},{move1,500} ,{step1,500} ,{step2,500},{move1,500} ,{step1,500} ,{step2,500},{move1,500} ,{step1,500} ,{step2,500},{move1,500} ,{step1,500} ,{step2,500},{move1,500} ,{step1,500} ,{step2,500} };
PROGMEM transition_t seq5[] = {{0,3} ,{move1,500} ,{step1,500} ,{step2,500}};
#endif

