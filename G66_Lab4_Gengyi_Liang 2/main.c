#include <stdio.h>
#include "./drivers/inc/VGA.h"
#include "./drivers/inc/ps2_keyboard.h"
#include "./drivers/inc/pushbutton.h"
#include "./drivers/inc/slider_switches.h"
void test_char();
void test_byte();
void test_pixel();
void test_vga();
void test_keyboard();

int main() {
	//test_vga();
	test_keyboard();
	return 0;
}

void test_char() {
	int x;
	int y;
	char c = 0;
	for (y = 0; y <= 59; y++) {
		for (x = 0; x <= 79; x++) {
			VGA_write_char_ASM(x, y, c++);
		}
	}
}

void test_byte() {
	int x;
	int y;
	char c = 0;

	for (y = 0; y <= 59; y++) {
		for (x = 0; x <= 79; x = x + 3){
			VGA_write_byte_ASM(x, y, c++);
		}
	}
}

void test_pixel() {
	int x;
	int y;
	unsigned short colour = 0;
	for (y = 0; y <= 239; y++){
		for (x = 0; x <= 319; x++) {
			VGA_draw_point_ASM(x, y, colour++);
		}
	}
}

void test_vga() {
	VGA_clear_charbuff_ASM();
	VGA_clear_pixelbuff_ASM();
	while(1) {
		int value = read_PB_data_ASM();
		if ((value & PB0) && (read_slider_switches_ASM() != 0)){
			test_byte();
		}	
		if ((value & PB0) && (read_slider_switches_ASM() == 0)){
			test_char();
		}
		if (value & PB1){
			test_pixel();
		}
		if (value & PB2){
			VGA_clear_charbuff_ASM();
		}
		if (value & PB3){
			VGA_clear_pixelbuff_ASM();
		}
	}
}

void test_keyboard() {
	int x = 0;
	int y = 0;
	char read;
	VGA_clear_charbuff_ASM();
	VGA_clear_pixelbuff_ASM();
	while(1) {
		if (read_PS2_data_ASM(&read)) {
			VGA_write_byte_ASM(x, y, read);
			if (x == 78) y = (y + 1) % 60;
			x = (x + 3) % 81;
		}
	}
}
