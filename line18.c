#include <XC.h>
#include <pic16f84a.h>

//設定
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON

#define _XTAL_FREQ 10000000

void main() {
    TRISA = 0x1F;
	TRISB = 0x00;

    int flg = 0;
	int sensor;

    while(PORTAbits.RA4 == 0) {
        //初期化
		PORTB = 0b00000000;
    }

    while(PORTAbits.RA4 || !flg) 

        flg = PORTAbits.RA4;

    while(1) {  //ループ
		sensor = ~PORTA & 0b1111;	//白黒反転

		switch(sensor) {	//1が白(ライン)
			case 0b1100:
			case 0b0011:
			case 0b0000:
			case 0b0110:	//RA1,RA2白(前進)
				PORTBbits.RB1 = PORTBbits.RB2 = 1;	//LED on
				PORTBbits.RB4 = 1;	//左
				PORTBbits.RB5 = 0;
				PORTBbits.RB6 = 0;
    			PORTBbits.RB7 = 1;	//右
				__delay_ms(4);
	    		PORTBbits.RB4 = 0;	//左
    			PORTBbits.RB7 = 0;	//右
				__delay_ms(5);
				PORTBbits.RB1 = PORTBbits.RB2 = 0;	//LED off
				break;

			case 0b0100:	//RA2白
			case 0b1000:	//RA3白	
				//右モータ回転
				PORTBbits.RB2 = PORTBbits.RB3 = 1;	//LED on
				PORTBbits.RB7 = 1;	//右
	      		__delay_ms(4);
		    	PORTBbits.RB7 = 0;	//右
    			__delay_ms(5);
				PORTBbits.RB2 = PORTBbits.RB3 = 0;	//LED off
				break;

			case 0b0001:	//RA0白
			case 0b0010:	//RA1白
				//左モータ回転
				PORTBbits.RB0 = PORTBbits.RB1 = 1;	//LED on
				PORTBbits.RB4 = 1;	//左
				__delay_ms(4);
	    		PORTBbits.RB4 = 0;	//左
		    	__delay_ms(5);
				PORTBbits.RB0 = PORTBbits.RB1 = 0;	//LED off
				break;
		}
    }
}