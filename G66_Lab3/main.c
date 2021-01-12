#include <stdio.h>
#include "./drivers/inc/LEDs.h"
#include "./drivers/inc/slider_switches.h"
#include "./drivers/inc/HEX_displays.h"
#include "./drivers/inc/HPS_TIM.h"
#include "./drivers/inc/int_setup.h"
#include "./drivers/inc/ISRs.h"
#include "./drivers/inc/pushbutton.h"
int main(){ 
	//Part 1 & 2----------------------------------------------------------------------------------------------------------------------------
/*
int switchStatus;
while (1) {		
switchStatus = read_slider_switches_ASM();
		write_LEDs_ASM(switchStatus);//This is a sequence of string indicating the switch status			
		if ((0xF & read_PB_data_ASM()) && (switchStatus) <= 512) {			
			//0xF &: clearthe upper 28 bits, we only want the 4 bits that control the pushbuttons  
			HEX_flood_ASM(HEX4 | HEX5); 
			if(switchStatus <= 15){//only want to display the right number				
				switch (0xF & read_PB_data_ASM()) {
					case 1 ://0b0001
						HEX_write_ASM(HEX0, switchStatus);
						break;
					case 2 ://0b0010
						HEX_write_ASM(HEX1, switchStatus);
						break;					
					case 4://0b0100
						HEX_write_ASM(HEX2, switchStatus);
						break;			
					case 8://0b1000
						HEX_write_ASM(HEX3, switchStatus);
						break;					
					default://When default is triggered, the program has a bug
						break;
				}
			}
		} else if (read_slider_switches_ASM() >= 512) {
			//clear
			HEX_clear_ASM(HEX0 | HEX1 | HEX2 | HEX3 | HEX4 | HEX5);
		} 
	}
//*/

//Part 3: Polling ----------------------------------------------------------------------------------------------------------------

	int ms = 0;
	int ms_display = 0;
	int sec = 0;
	int sec_display = 0;
	int min= 0;
	int min_display = 0;
	HPS_TIM_config_t hps_tim;
	hps_tim.tim = TIM0|TIM1|TIM2|TIM3;
	hps_tim.timeout = 10000;
	hps_tim.LD_en = 1;
	hps_tim.INT_en = 1;
	hps_tim.enable = 1;
	HPS_TIM_config_ASM(&hps_tim);
	int start = 0;
	int button_status = 0;
	while(1) {	
		button_status = 0xF & read_PB_data_ASM();
		switch(button_status){
		case 1:
			start = 1;
			break;
		case 2:
			start = 0;
		break;
		case 4:
			ms = 0;
			ms_display = 0;
			sec = 0;
			sec_display = 0;
			min= 0;
			min_display = 0;
			HEX_write_ASM(HEX0|HEX1|HEX2|HEX3|HEX4|HEX5, 0);
			start = 0;
		break;
		}
		if (HPS_TIM_read_INT_ASM(TIM0) && start) {
			HPS_TIM_clear_INT_ASM(TIM0);
			ms+= 10;				
			if (ms >= 1000) {
				ms%= 1000;
				sec++;				
				if (sec>= 60) {
					sec%= 60;
					min++;
					if (min%= 60) min = 0;
				}
			}
			ms_display = ((ms% 100) / 10);
			sec_display = sec% 10 ;
			min_display = min% 10;
			//Get corecsponding digit and convert to ASCII
			HEX_write_ASM(HEX0,ms_display);
			HEX_write_ASM(HEX1, ms/100);
			HEX_write_ASM(HEX2, sec_display);
			HEX_write_ASM(HEX3, sec/10 );
			HEX_write_ASM(HEX4, min_display);
			HEX_write_ASM(HEX5, min/10 );
	}
}
//*/
//Part 3: Interrupt ---------------------------------------------------------------------------------------------------------------
/*
	HPS_TIM_config_t hps_tim;
	hps_tim.tim = TIM0;
	hps_tim.timeout = 10000; //timer 1 timeout
	hps_tim.LD_en = 1;
	hps_tim.INT_en = 1;
	hps_tim.enable = 1;
	HPS_TIM_config_ASM(&hps_tim); //Config timer 1


	HPS_TIM_config_t hps_tim_pb;
	hps_tim_pb.tim = TIM1;
	hps_tim_pb.timeout = 5000;
	hps_tim_pb.LD_en = 1;
	hps_tim_pb.INT_en = 1;
	hps_tim_pb.enable = 1;
	HPS_TIM_config_ASM(&hps_tim_pb); 

	int push_buttons = 0;
	int ms = 0;
	int ms_display = 0;
	int sec = 0;
	int sec_display = 0;
	int min= 0;
	int min_display = 0;
	int start = 0; 
	while(1) {
		if (HPS_TIM_read_INT_ASM(TIM0) && start) {
			HPS_TIM_clear_INT_ASM(TIM0);
			ms+= 10;				
			if (ms >= 1000) {
				ms%= 1000;
				sec++;				
				if (sec>= 60) {
					sec%= 60;
					min++;
					if (min%= 60) min = 0;
				}
			}
			ms_display = ((ms% 100) / 10);
			sec_display = sec% 10 ;
			min_display = min% 10;
			//Get corecsponding digit and convert to ASCII
			HEX_write_ASM(HEX0,ms_display);
			HEX_write_ASM(HEX1, ms/100);
			HEX_write_ASM(HEX2, sec_display);
			HEX_write_ASM(HEX3, sec/10 );
			HEX_write_ASM(HEX4, min_display);
			HEX_write_ASM(HEX5, min/10 );
		}
		if (HPS_TIM_read_INT_ASM(TIM1)) { 
			
			HPS_TIM_clear_INT_ASM(TIM1);
			int x = 0xF & read_PB_data_ASM();
			if ((x & 1) && (!start)) { //Start timer
				start = 1;
			} else if ((x & 2) && (start)) { //Stop timer
				start = 0;
			} else if (x & 4) { //Reset timer
				start = 0; //Reset timer
				ms= 0;
				sec= 0;
				min= 0;
				HEX_write_ASM(HEX0 | HEX1 | HEX2 | HEX3 | HEX4 | HEX5, 0);
			}
		}
	}

//*/

	return 0;
}
