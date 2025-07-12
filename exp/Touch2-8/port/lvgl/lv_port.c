#include "lv_port.h"
#include "bsp_st7789.h"
#include "bsp_cst328.h"

#define LCD_WIDTH 240
#define LCD_HEIGHT 320
#define LCD_ROTATION 0

lv_indev_t *indev_touchpad;
static lv_disp_drv_t disp_drv; /*Descriptor of a display driver*/

#define LVGL_TICK_PERIOD_MS 1

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    // if(disp_flush_enabled) {
    //     /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    //     int32_t x;
    //     int32_t y;
    //     for(y = area->y1; y <= area->y2; y++) {
    //         for(x = area->x1; x <= area->x2; x++) {
    //             /*Put a pixel to the display. For example:*/
    //             /*put_px(x, y, *color_p)*/
    //             color_p++;
    //         }
    //     }
    // }
    bsp_st7789_flush_dma(area->x1, area->y1, area->x2, area->y2,
                         (uint16_t *)color_p);
    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    // lv_disp_flush_ready(disp_drv);
}

static void touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    bsp_cst328_data_t cst328_data;

    /*Save the pressed coordinates and the state*/
    bsp_cst328_read();
    if (bsp_cst328_get_touch_data(&cst328_data))
    {
        data->state = LV_INDEV_STATE_PR;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }

    /*Set the last pressed coordinates*/
    data->point.x = cst328_data.coords[0].x;
    data->point.y = cst328_data.coords[0].y;
}

static bool repeating_lvgl_timer_cb(struct repeating_timer *t)
{
    lv_tick_inc(LVGL_TICK_PERIOD_MS);
    return true;
}

void lvgl_flush_done_callback(void)
{
    lv_disp_flush_ready(&disp_drv);
}

void lv_port_init(void)
{
    lv_init();
    static lv_disp_draw_buf_t draw_buf_dsc;

    static bsp_st7789_info_t st7789_info;
    st7789_info.width = LCD_WIDTH;
    st7789_info.height = LCD_HEIGHT;
    st7789_info.rotation = LCD_ROTATION;
    st7789_info.x_offset = 0;
    st7789_info.y_offset = 0;
    st7789_info.enabled_dma = true;
    st7789_info.dma_flush_done_callback = lvgl_flush_done_callback;
    bsp_st7789_init(&st7789_info);
    // printf("cst328_info ->width:%d ->height:%d  rotation:%d \r\n", st7789_info.width, st7789_info.height, st7789_info.rotation);
    uint32_t buffer_size = st7789_info.width * st7789_info.height / 4;
    lv_color_t *buf_1 = (lv_color_t *)malloc(buffer_size * sizeof(lv_color_t));
    lv_color_t *buf_2 = (lv_color_t *)malloc(buffer_size * sizeof(lv_color_t));
    // lv_color_t *buf_2 = NULL;
    lv_disp_draw_buf_init(&draw_buf_dsc, buf_1, buf_2,
                          buffer_size); /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    lv_disp_drv_init(&disp_drv); /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = st7789_info.width;
    disp_drv.ver_res = st7789_info.height;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;
    // disp_drv.user_data = st7789_info;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc;

    /*Required for Example 3)*/
    // disp_drv.direct_mode = 1;
    // disp_drv.direct_mode = 1;

    /* Fill a memory array with a color if you have GPU.
     * Note that, in lv_conf.h you can enable GPUs that has built-in support in LVGL.
     * But if you have a different GPU you can use with this callback.*/
    // disp_drv.gpu_fill_cb = gpu_fill;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);

    static bsp_cst328_info_t cst328_info;
    cst328_info.width = LCD_WIDTH;
    cst328_info.height = LCD_HEIGHT;
    cst328_info.rotation = LCD_ROTATION;
    bsp_cst328_init(&cst328_info);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    indev_touchpad = lv_indev_drv_register(&indev_drv);

    static struct repeating_timer lvgl_timer;
    add_repeating_timer_ms(LVGL_TICK_PERIOD_MS, repeating_lvgl_timer_cb, NULL, &lvgl_timer);
}