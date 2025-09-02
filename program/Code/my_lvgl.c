/**
  ******************************************************************************
  * @file    test.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  */
#include "my_lvgl.h"
#include "lvgl.h"

// 键盘按钮回调函数
static void keyboard_cb(lv_event_t * e)
{
    lv_obj_t * target = lv_event_get_target(e);
    lv_obj_t * textarea = (lv_obj_t *)lv_event_get_user_data(e);
    const char * text = lv_btnmatrix_get_btn_text(target, lv_btnmatrix_get_selected_btn(target));
    
    if(strcmp(text, LV_SYMBOL_BACKSPACE) == 0) {
        lv_textarea_del_char(textarea);
    }
    else if(strcmp(text, LV_SYMBOL_NEW_LINE) == 0) {
        // 这里可以添加按下Enter键后的处理逻辑
        LV_LOG_USER("Password entered");
    }
    else {
        lv_textarea_add_text(textarea, "*");
    }
}

void create_password_ui(lv_obj_t * parent)
{
    // 创建主容器
    lv_obj_t * cont = lv_obj_create(parent);
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));	//LV_PCT(100)就是100%
    lv_obj_set_style_bg_color(cont, lv_color_hex(0x2c3e50), 0);		//基础颜色
    lv_obj_set_style_bg_grad_color(cont, lv_color_hex(0x3498db), 0);	//渐变颜色
    lv_obj_set_style_bg_grad_dir(cont, LV_GRAD_DIR_VER, 0);		//渐变方向
    lv_obj_set_style_border_width(cont, 0, 0);	//设置对象的边框宽度为 0
    lv_obj_set_style_radius(cont, 0, 0);		//设置对象的圆角半径为 0
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);	//移除对象的可滚动标志：LVGL 对象通常可以滚动（如果内容超出显示区域）
    lv_obj_center(cont);	//以父级中心居中

    // 创建标题
    lv_obj_t * title = lv_label_create(cont);
    lv_label_set_text(title, "Please Enter PassWord");
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 40);

    // 创建密码文本区域
    lv_obj_t * pwd_ta = lv_textarea_create(cont);
    lv_obj_set_size(pwd_ta, 200, 60);
    lv_obj_align(pwd_ta, LV_ALIGN_TOP_MID, 0, 100);
    lv_textarea_set_text(pwd_ta, "");
    lv_textarea_set_password_mode(pwd_ta, true);
    lv_textarea_set_one_line(pwd_ta, true);	//设置文本区域为单行模式
    lv_textarea_set_max_length(pwd_ta, 16);
    lv_obj_set_style_bg_color(pwd_ta, lv_color_hex(0x34495e), 0);
    lv_obj_set_style_text_color(pwd_ta, lv_color_white(), 0);
    lv_obj_set_style_border_width(pwd_ta, 2, 0);	//边框厚度
    lv_obj_set_style_border_color(pwd_ta, lv_color_hex(0x1abc9c), 0);	//边框颜色

    // 创建键盘按钮映射
    static const char * btnm_map[] = {
        "1", "2", "3", "\n",
        "4", "5", "6", "\n",
        "7", "8", "9", "\n",
        LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_NEW_LINE, ""
    };

    // 创建键盘
    lv_obj_t * btnm = lv_btnmatrix_create(cont);
    lv_btnmatrix_set_map(btnm, btnm_map);
    lv_obj_set_size(btnm, 240, 130);
    lv_obj_align(btnm, LV_ALIGN_TOP_MID, 0, 180);
    lv_obj_add_event_cb(btnm, keyboard_cb, LV_EVENT_VALUE_CHANGED, pwd_ta);
    
    // 设置键盘样式
    lv_obj_set_style_bg_color(btnm, lv_color_hex(0x34495e), 0);
    lv_obj_set_style_border_width(btnm, 0, 0);
    
    // 设置按钮样式
    lv_obj_set_style_bg_color(btnm, lv_color_hex(0x2c3e50), LV_PART_ITEMS);
    lv_obj_set_style_bg_color(btnm, lv_color_hex(0x1abc9c), LV_PART_ITEMS | LV_STATE_PRESSED);
    lv_obj_set_style_text_color(btnm, lv_color_white(), LV_PART_ITEMS);
    lv_obj_set_style_radius(btnm, 10, LV_PART_ITEMS);
}
