#include <XC.h>
#include <pic16f84a.h>

//設定
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON

#define _XTAL_FREQ 10000000

#define LEFT 1
#define RIGHT 2
#define OUT 3

void main() {
    TRISA = 0x1F;
	TRISB = 0x00;

	int flg = 0;
	int course = OUT;	//仮設定直進
	int sensor;

    while(PORTAbits.RA4 == 0) {
        //初期化
		PORTB = 0b00000000;
    }

    while(PORTAbits.RA4 || !flg) {
		flg = PORTAbits.RA4;

        while(1) {  //ループ
			//sensor = ~PORTA;	//白黒反転
			
			if(PORTAbits.RA1 && PORTAbits.RA2) {	//前進
				PORTBbits.RB1 = PORTBbits.RB2 = 1;	//LED on
				PORTB = 0b10010000;
		        __delay_ms(0.5);
		        PORTB = 0b00000000;
		        __delay_ms(0.4);
				PORTBbits.RB1 = PORTBbits.RB2 = 0;	//LED off
			}else if(!PORTAbits.RA0) {
				PORTBbits.RB2 = PORTBbits.RB3 = 1;	//LED on
				//左旋回(右モータ回転)
				PORTB = 0b10000000;
				__delay_ms(0.5);

        		PORTB=0b00000000;
       			 __delay_ms(0.4);
       			course = LEFT;
				PORTBbits.RB2 = PORTBbits.RB3 = 0;	//LED off
			}else if(!PORTAbits.RA3) {
				PORTBbits.RB0 = PORTBbits.RB1 = 1;	//LED on
				//右旋回(左モータ回転)
				PORTB = 0b00010000;
				__delay_ms(0.5);

        		PORTB=0b00000000;
       			__delay_ms(0.4);
       			course = RIGHT;
				PORTBbits.RB0 = PORTBbits.RB1 = 0;	//LED off
			}
        }
    }
}