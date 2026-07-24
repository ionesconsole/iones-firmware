#pragma once
#include <TFT_eSPI.h>

#define HATED_COLOR TFT_MAGENTA

#define SD_CS 1
#define SD_SCK 12
#define SD_MOSI 11
#define SD_MISO 13

#define GM_CP       GPIO_NUM_5 // Clock
#define GM_PL       GPIO_NUM_6 // Parallel load in
#define GM_Q7       GPIO_NUM_21

#define GM_A (1u << 0)
#define GM_B (1u << 1)
#define GM_Y (1u << 2)
#define GM_X (1u << 3)
#define GM_U (1u << 4)
#define GM_R (1u << 5)
#define GM_D (1u << 6)
#define GM_L (1u << 7)

#define GM_SEL GPIO_NUM_15
#define GM_STR GPIO_NUM_16

#define PATH_MAX_IOWR 256

#define BUZZER GPIO_NUM_2