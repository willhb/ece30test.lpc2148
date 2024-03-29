#include <stdio.h>
#include <lpc2148/lpc214x.h>
#include <lpc2148/openlpc.h>

/**************************************
*       OpenLPC 2148 ECE30 Lab Test Routine
*		Test functionality:
*		VU Meter LEDs: P0.0 -> P0.7
*		Debug LEDs: P1.20, P1.26
*		Microphone: MIC
*		Headphones: AOUT
*
*		All LEDs blink twice on boot.
*			Button Open:
*			Microphone is passed through to the Headphone port
*			All LEDs are ON!
*		Button Closed:
*			Headphones output sine wave
*			Microphone acts as a VU meter with LEDs 0-7
*			Debug LEDs are off
***************************************/


unsigned int sine_wave[256] = {
  0x80, 0x83, 0x86, 0x89, 0x8C, 0x90, 0x93, 0x96,
  0x99, 0x9C, 0x9F, 0xA2, 0xA5, 0xA8, 0xAB, 0xAE,
  0xB1, 0xB3, 0xB6, 0xB9, 0xBC, 0xBF, 0xC1, 0xC4,
  0xC7, 0xC9, 0xCC, 0xCE, 0xD1, 0xD3, 0xD5, 0xD8,
  0xDA, 0xDC, 0xDE, 0xE0, 0xE2, 0xE4, 0xE6, 0xE8,
  0xEA, 0xEB, 0xED, 0xEF, 0xF0, 0xF1, 0xF3, 0xF4,
  0xF5, 0xF6, 0xF8, 0xF9, 0xFA, 0xFA, 0xFB, 0xFC,
  0xFD, 0xFD, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFD,
  0xFD, 0xFC, 0xFB, 0xFA, 0xFA, 0xF9, 0xF8, 0xF6,
  0xF5, 0xF4, 0xF3, 0xF1, 0xF0, 0xEF, 0xED, 0xEB,
  0xEA, 0xE8, 0xE6, 0xE4, 0xE2, 0xE0, 0xDE, 0xDC,
  0xDA, 0xD8, 0xD5, 0xD3, 0xD1, 0xCE, 0xCC, 0xC9,
  0xC7, 0xC4, 0xC1, 0xBF, 0xBC, 0xB9, 0xB6, 0xB3,
  0xB1, 0xAE, 0xAB, 0xA8, 0xA5, 0xA2, 0x9F, 0x9C,
  0x99, 0x96, 0x93, 0x90, 0x8C, 0x89, 0x86, 0x83,
  0x80, 0x7D, 0x7A, 0x77, 0x74, 0x70, 0x6D, 0x6A,
  0x67, 0x64, 0x61, 0x5E, 0x5B, 0x58, 0x55, 0x52,
  0x4F, 0x4D, 0x4A, 0x47, 0x44, 0x41, 0x3F, 0x3C,
  0x39, 0x37, 0x34, 0x32, 0x2F, 0x2D, 0x2B, 0x28,
  0x26, 0x24, 0x22, 0x20, 0x1E, 0x1C, 0x1A, 0x18,
  0x16, 0x15, 0x13, 0x11, 0x10, 0x0F, 0x0D, 0x0C,
  0x0B, 0x0A, 0x08, 0x07, 0x06, 0x06, 0x05, 0x04,
  0x03, 0x03, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03,
  0x03, 0x04, 0x05, 0x06, 0x06, 0x07, 0x08, 0x0A,
  0x0B, 0x0C, 0x0D, 0x0F, 0x10, 0x11, 0x13, 0x15,
  0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24,
  0x26, 0x28, 0x2B, 0x2D, 0x2F, 0x32, 0x34, 0x37,
  0x39, 0x3C, 0x3F, 0x41, 0x44, 0x47, 0x4A, 0x4D,
  0x4F, 0x52, 0x55, 0x58, 0x5B, 0x5E, 0x61, 0x64,
  0x67, 0x6A, 0x6D, 0x70, 0x74, 0x77, 0x7A, 0x7D
};

int	main (void)
{
	
	FIO1DIR |= 1 << 26 | 1 << 20; //LEDs are outputs
	FIO1CLR |= 1 << 26 | 1 << 20; //Turn on LEDs
	
	FIO0DIR |= 0xFF; // bits 0 -> 7 are outputs
	FIO0CLR |= 0xFF; // turn them all on
	
	PINSEL1 |= ((1 << 24) | (1 << 19));//0.28 to ADC0.1, 0.25 to DAC
	AD0CR = (1 << 21);
	AD0CR |= (1<<1) | (150 << 8) | (1 << 16) | (1 << 17); //AD0.1 only, clk div 150, burst mode, 9 bits, active.
	AD0CR |= 1 << 24; //START!

	delay_ms(250);
	FIO1SET |= 1 << 26 | 1 << 20; //Turn off LEDs
	FIO0SET |= 0xFF; // turn them all off
	delay_ms(250);
	FIO1CLR |= 1 << 26 | 1 << 20; //Turn on LEDs
	FIO0CLR |= 0xFF; // turn them all on
	delay_ms(250);
	FIO1SET |= 1 << 26 | 1 << 20; //Turn off LEDs
	FIO0SET |= 0xFF; // turn them all off
	delay_ms(250);
	FIO1CLR |= 1 << 26 | 1 << 20; //Turn on LEDs
	FIO0CLR |= 0xFF; // turn them all on
	delay_ms(250);
	FIO1CLR |= 1 << 26 | 1 << 20;
	long int adc_result = 0; //clear to 0
	
	unsigned long int i;
	unsigned long int p;
	int adc_offset = 0;
	
	i = 0;
	p = 0;
	int adc_peak = 0;
	
	while(1){
				 
				adc_result = (AD0DR1>>6) & 0x3FF;
				
				if(!(FIO0PIN & 1<<14)){
					FIO1CLR |= 1 << 26 | 1 << 20; //Turn on LEDs
					FIO0PIN = 0x0;
					DACR = (adc_result << 6);
				} else {
						
					FIO1SET |= 1 << 26 | 1 << 20; //Turn off LEDs
					adc_offset = adc_result - 512;
					
					if(adc_offset < 0){
						adc_offset = - adc_offset;
					}
					
					//if(adc_offset > adc_peak){
						adc_peak = adc_offset;
					//}
					
					printf("adc_peak: %d \n\r", adc_offset);
					
					if(adc_peak > 256){
						FIO0PIN = ~0xFF; // turn them all on
					} else if (adc_peak > 128){
						FIO0PIN = ~0x7F;
					}	else if (adc_peak > 64){
						FIO0PIN = ~0x3F;
					}	else if (adc_peak > 32){
						FIO0PIN = ~0x1F;
					}	else if (adc_peak > 16){
						FIO0PIN = ~0xF;
					} 	else if (adc_peak > 8){
		    			FIO0PIN = ~0x7;
		    		}	else if (adc_peak > 4){
		    			FIO0PIN = ~0x3;
		    		}	else if (adc_peak > 2){
		    			FIO0PIN = ~0x1;
		    		}	else {
		    			FIO0PIN = ~0x0;
		    		}
					
					
				}

				
		
	}
	
	
	//we never return.

}

