#include "./drivers/inc/vga.h"
#include "./drivers/inc/ISRs.h"
#include "./drivers/inc/LEDs.h"
#include "./drivers/inc/audio.h"
#include "./drivers/inc/HPS_TIM.h"
#include "./drivers/inc/int_setup.h"
#include "./drivers/inc/wavetable.h"
#include "./drivers/inc/pushbuttons.h"
#include "./drivers/inc/ps2_keyboard.h"
#include "./drivers/inc/HEX_displays.h"
#include "./drivers/inc/slider_switches.h"
//Global constants
// sample rate = 48k
// frequency = 100Hz
const int rate = 48000, frqcy = 100;
const float fs[] = {130.813, 146.832, 164.814, 174.614, 195.998, 220.000, 246.942, 261.626};
// indicator of which keys were pressed
char keys[8] = {};
// volumn control
int amp = 2;

/**
*This is Lab0. Genereating constant 100Hz noise.
*/
void audio(int f){
	int half = (rate/f)/2;
	int full = rate/f;
	int counter = 0, rising =1;	
	while(1){		
		if(!read_slider_switches_ASM()) {
		break;
		}
		else if(write_audio_ASM(counter<half?0x00FFFFFF:0x00000000)){
		counter = (counter+1)%full;				
		}	
	}
}

//Query table.s and get the proper value for sine wave.
double signal(int f, int t){
	int index = f*t%rate;
	return sine[index];
}

//Linear interpolation
double signal_float(double f, int t){
	int floor = ((int)(f))*t;
	double r = ((double)(f*t)-floor);
	floor = floor%rate;
	return ((1-r)*(sine[floor]) + (r*sine[floor+1]));	 
}

//Summing up wave according to keys pressed
double makewave(char* keyspressed, int t) {
	int i;
	double note = 0;
	for(i = 0; i < 8; i++){
		if(keyspressed[i]==1)	note += signal(fs[i], t);		
	}
	return note;
}

//main function
int main(){
	//no need now
	//lab0=============================================================================
	//audio(frqcy);

	//lab1=============================================================================
	// timer
	int_setup(1, (int []){199});
	HPS_TIM_config_t hps_tim;
	hps_tim.tim = TIM0; 
	hps_tim.timeout = 20; 
	hps_tim.LD_en = 1; 
	hps_tim.INT_en = 1; 
	hps_tim.enable = 1; 
	HPS_TIM_config_ASM(&hps_tim);
	
	
	int press = 0;//break code status
	int t = 0;	//counter
	char data ;	//keyboard data
	double signal = 0.0;	//signal made


	while(1) {
				//use the slider switch so no need to recompile
				if(read_slider_switches_ASM() > 0) {
					audio(frqcy);
				}
				//read keyboard data
				else if (read_ps2_data_ASM(&data)) {
					printf("here");
					switch (data){							

																						// A  C
						case 0x1C://A
							keys[0] = press?0:1;
							press = press?0:press;
							break;
																						// S  D
						case 0x1B://S
							keys[1] = press?0:1;
							press = press?0:press;
							break;
																						// D  E 
						case 0x23://D
							keys[2] = press?0:1;
							press = press?0:press;
							break;
																						// F  F 
						case 0x2B://F
							keys[3] = press?0:1;
							press = press?0:press;
							break;
																						// J  G 
						case 0x3B://J
							keys[4] =  press?0:1;
							press = press?0:press;
							break;
																						// K  A 
						case 0x42://K
							keys[5] = press?0:1;
							press = press?0:press;
							break;
																						// L  B
						case 0x4B://L
							keys[6] = press?0:1;
							press = press?0:press;
							break;
																						// ;  C
						case 0x4C://;
							keys[7] = press?0:1;
							press = press?0:press;
							break;		
						case 0xF0: //break code F0												//F0
							press = 1;
							//printf("u");
							break;	
							
																						//up G
						case 0x34://G: volumn up
							if(press){
							//printf("upp");
								amp++;
								press = 0;
							}
							break;

																						//down  H
						case 0x33://H:volumn down
							if(press){
								if(amp>0)amp--;
								press = 0;
							}
							break;
				
						default:
							press = 0;
					}
				}
			//summing up waves
			signal = amp * makewave(keys, t); 
			//write to audio when ISR flag gives 1
			if(hps_tim0_int_flag) {
				hps_tim0_int_flag = 0;
				audio_write_data_ASM(signal, signal);
				t++;
			}
			//reset counter and signal
			signal = 0;
			t = t%rate;		
	}
	return 0;
}
