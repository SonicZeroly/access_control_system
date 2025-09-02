/**
  ******************************************************************************
  * @file    test.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  */
#include "my_lvgl.h"
#include "lvgl.h"

// ���̰�ť�ص�����
static void keyboard_cb(lv_event_t * e)
{
    lv_obj_t * target = lv_event_get_target(e);
    lv_obj_t * textarea = (lv_obj_t *)lv_event_get_user_data(e);
    const char * text = lv_btnmatrix_get_btn_text(target, lv_btnmatrix_get_selected_btn(target));
    
    if(strcmp(text, LV_SYMBOL_BACKSPACE) == 0) {
        lv_textarea_del_char(textarea);
    }
    else if(strcmp(text, LV_SYMBOL_NEW_LINE) == 0) {
        // ���������Ӱ���Enter����Ĵ����߼�
        LV_LOG_USER("Password entered");
    }
    else {
        lv_textarea_add_text(textarea, "*");
    }
}

void create_password_ui(lv_obj_t * parent)
{
    // ����������
    lv_obj_t * cont = lv_obj_create(parent);
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));	//LV_PCT(100)����100%
    lv_obj_set_style_bg_color(cont, lv_color_hex(0x2c3e50), 0);		//������ɫ
    lv_obj_set_style_bg_grad_color(cont, lv_color_hex(0x3498db), 0);	//������ɫ
    lv_obj_set_style_bg_grad_dir(cont, LV_GRAD_DIR_VER, 0);		//���䷽��
    lv_obj_set_style_border_width(cont, 0, 0);	//���ö���ı߿���Ϊ 0
    lv_obj_set_style_radius(cont, 0, 0);		//���ö����Բ�ǰ뾶Ϊ 0
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);	//�Ƴ�����Ŀɹ�����־��LVGL ����ͨ�����Թ�����������ݳ�����ʾ����
    lv_obj_center(cont);	//�Ը������ľ���

    // ��������
    lv_obj_t * title = lv_label_create(cont);
    lv_label_set_text(title, "Please Enter PassWord");
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 40);

    // ���������ı�����
    lv_obj_t * pwd_ta = lv_textarea_create(cont);
    lv_obj_set_size(pwd_ta, 200, 60);
    lv_obj_align(pwd_ta, LV_ALIGN_TOP_MID, 0, 100);
    lv_textarea_set_text(pwd_ta, "");
    lv_textarea_set_password_mode(pwd_ta, true);
    lv_textarea_set_one_line(pwd_ta, true);	//�����ı�����Ϊ����ģʽ
    lv_textarea_set_max_length(pwd_ta, 16);
    lv_obj_set_style_bg_color(pwd_ta, lv_color_hex(0x34495e), 0);
    lv_obj_set_style_text_color(pwd_ta, lv_color_white(), 0);
    lv_obj_set_style_border_width(pwd_ta, 2, 0);	//�߿���
    lv_obj_set_style_border_color(pwd_ta, lv_color_hex(0x1abc9c), 0);	//�߿���ɫ

    // �������̰�ťӳ��
    static const char * btnm_map[] = {
        "1", "2", "3", "\n",
        "4", "5", "6", "\n",
        "7", "8", "9", "\n",
        LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_NEW_LINE, ""
    };

    // ��������
    lv_obj_t * btnm = lv_btnmatrix_create(cont);
    lv_btnmatrix_set_map(btnm, btnm_map);
    lv_obj_set_size(btnm, 240, 130);
    lv_obj_align(btnm, LV_ALIGN_TOP_MID, 0, 180);
    lv_obj_add_event_cb(btnm, keyboard_cb, LV_EVENT_VALUE_CHANGED, pwd_ta);
    
    // ���ü�����ʽ
    lv_obj_set_style_bg_color(btnm, lv_color_hex(0x34495e), 0);
    lv_obj_set_style_border_width(btnm, 0, 0);
    
    // ���ð�ť��ʽ
    lv_obj_set_style_bg_color(btnm, lv_color_hex(0x2c3e50), LV_PART_ITEMS);
    lv_obj_set_style_bg_color(btnm, lv_color_hex(0x1abc9c), LV_PART_ITEMS | LV_STATE_PRESSED);
    lv_obj_set_style_text_color(btnm, lv_color_white(), LV_PART_ITEMS);
    lv_obj_set_style_radius(btnm, 10, LV_PART_ITEMS);
}
