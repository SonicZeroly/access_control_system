#ifndef MY_UI_H
#define MY_UI_H

#include <stdint.h>
#include "lvgl/lvgl.h"

typedef enum {
    UI_MAIN_SCREEN,
    UI_ENTER_PW_SCREEN,
    UI_CHANGE_PW_SCREEN,
    UI_ADD_FP_SCREEN,
    UI_DELETE_FP_SCREEN,
    UI_ADD_CARD_SCREEN,
    UI_DELETE_CARD_SCREEN
}ui_screen_t;

typedef struct {
    ui_screen_t   page_id;
    int8_t state;
}ui_screen_info_t;

void my_ui_init(void);

#endif
