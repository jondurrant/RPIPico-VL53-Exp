/*
 * Widgets.c
 *
 *  Created on: 20 Aug 2024
 *      Author: jondurrant
 */

#include "lvgl.h"
#include "VL53L0X.h"
#include <cstdio>

extern VL53L0X *pSensor;

#define DISP_VER_RES 320
#define DISP_HOR_RES 240


#define SENSOR_CALIB -41


static void timerCB(lv_timer_t * timer){

	char s[50];
	if (pSensor != NULL){
		sprintf(s, "%dmm",  pSensor->readRangeContinuousMillimeters()+ (SENSOR_CALIB));
	} else {
		sprintf(s,"??mm");
	}

	lv_obj_t *label = (	lv_obj_t *) timer->user_data;;

	lv_label_set_text(label, s);
}


lv_style_t labelSt;
/********************************************************************************
function:	Initializes the layout of LVGL widgets
parameter:
********************************************************************************/
void Widgets_Init(void)
{

    // /*Create tileview*/
    lv_obj_t *tv = lv_tileview_create(lv_scr_act());
    lv_obj_set_scrollbar_mode(tv,  LV_SCROLLBAR_MODE_OFF);

    //Jon test Tile

    lv_obj_t *tileJD = lv_tileview_add_tile(tv, 0, 0, LV_DIR_TOP|LV_DIR_BOTTOM);



	//Label
	lv_style_init(&labelSt);
	lv_style_set_text_font(&labelSt, &lv_font_montserrat_46);
	lv_style_set_text_color(
			&labelSt,
			lv_color_make(0, 0x50, 0x10));


	lv_obj_t * label1 = lv_label_create(tileJD);
	lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
	lv_label_set_text(label1, "?mm");
	lv_obj_set_width(label1, DISP_HOR_RES);  /*Set smaller width to make the lines wrap*/
	lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_align(label1, LV_ALIGN_CENTER, 0, -20);
	lv_obj_add_style(label1 , &labelSt,  LV_PART_MAIN);

	lv_timer_t * timer = lv_timer_create(timerCB, 500,  label1);

}



