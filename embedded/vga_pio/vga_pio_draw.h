#ifndef VGA_PIO_DRAW_H
#define	VGA_PIO_DRAW_H



//extern void load_scanline_adress();

extern void fill_vga_data_array(int frame_counter);

extern void draw_function_0();
extern void draw_function_1();
extern void draw_function_2();
extern void draw_function_3();
extern void draw_function_4(int frame_counter);
extern void draw_function_5(int frame_counter);
extern void draw_function_6(int frame_counter);
extern void draw_function_7(int frame_counter);
#endif