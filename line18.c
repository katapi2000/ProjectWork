#include <XC.h>
#include <pic16f84a.h>

//PICのconfig設定
#pragma config FOSC = HS		//オシレータモードHS
#pragma config WDTE = OFF		//ウォッチドッグタイマ無効
#pragma config PWRTE = ON		//パワーアップタイマ有効

#define _XTAL_FREQ 10000000		//周波数クロックの定義

#define LEFT 1
#define RIGHT 2
#define OUT 3

void main() {
    TRISA = 0x1F;		//PORTA入出力方向の設定
	TRISB = 0x00;		//PORTB入出力方向の設定

    int flg = 0;
	int sensor;
	int course = OUT;	//仮設定

    while(PORTAbits.RA4 == 0) {
        //ポート初期化
		PORTB = 0b00000000;
    }

    while(PORTAbits.RA4 || !flg) 		//スタートスイッチ待機

        flg = PORTAbits.RA4;

    while(1) {  //ループ
		sensor = ~PORTA & 0b1111;	//白黒反転
		//'1'がライン検出(白)、'0'が黒

		switch(sensor) {
			case 0b0110:
			case 0b1001:
				//前進
				PORTBbits.RB1 = PORTBbits.RB2 = 1;	//確認用LED on
				PORTBbits.RB4 = 1;	//左モータON
				PORTBbits.RB5 = 0;
				PORTBbits.RB6 = 0;
    			PORTBbits.RB7 = 1;	//右モータON
				__delay_ms(3);		//wait 3ms
	    		PORTBbits.RB4 = 0;	//左モータOFF
    			PORTBbits.RB7 = 0;	//右モータOFF
				__delay_ms(4);		//wait 4ms
				PORTBbits.RB1 = PORTBbits.RB2 = 0;	//確認用LED off
				break;

			case 0b0100:
			case 0b1000:
			case 0b1100:
			case 0b1010:
				//左旋回
				//右モータ回転
				PORTBbits.RB2 = PORTBbits.RB3 = 1;	//確認用LED on
				PORTBbits.RB7 = 1;	//右モータON
	      		__delay_ms(2.5);	//wait 2.5ms
		    	PORTBbits.RB7 = 0;	//右モータOFF
    			__delay_ms(3);		//wait 3ms
				PORTBbits.RB2 = PORTBbits.RB3 = 0;	//確認用LED off
				course = RIGHT;		//右センサコースアウト検出
				break;

			case 0b0001:
			case 0b0010:
			case 0b0011:
			case 0b0101:
				//右旋回
				PORTBbits.RB0 = PORTBbits.RB1 = 1;	//確認用LED on
				PORTBbits.RB4 = 1;	//左モータON
				__delay_ms(2.5);	//wait 2.5ms
	    		PORTBbits.RB4 = 0;	//左モータOFF
		    	__delay_ms(3);		//wait 3ms
				PORTBbits.RB0 = PORTBbits.RB1 = 0;	//確認用LED off
				course = LEFT;	//左センサコースアウト検出
				break;

			//コースアウト処理
			case 0b0000:
				if(course == RIGHT) {			//右センサコースアウト検出
					//左旋回
					PORTBbits.RB2 = PORTBbits.RB3 = 1;	//確認用LED on
					PORTBbits.RB7 = 1;	//右モータON
	      			__delay_ms(3);		//wait 3ms
		    		PORTBbits.RB7 = 0;	//右モータOFF
    				__delay_ms(5);		//wait 5ms
					PORTBbits.RB2 = PORTBbits.RB3 = 0;	//確認用LED off
				}else if(course == LEFT) {		//左センサコースアウト検出
					//右旋回
					PORTBbits.RB0 = PORTBbits.RB1 = 1;	//確認用LED on
					PORTBbits.RB4 = 1;	//左モータON
					__delay_ms(3);		//wait 3ms
	    			PORTBbits.RB4 = 0;	//左モータOFF
		    		__delay_ms(5);		//wait 5ms
					PORTBbits.RB0 = PORTBbits.RB1 = 0;	//確認用LED off
				}else if(course == OUT) {		//前進
					PORTBbits.RB1 = PORTBbits.RB2 = 1;	//確認用LED on
					PORTBbits.RB4 = 1;	//左モータON
					PORTBbits.RB5 = 0;
					PORTBbits.RB6 = 0;
    				PORTBbits.RB7 = 1;	//右モータON
					__delay_ms(3);		//wait 3ms
		    		PORTBbits.RB4 = 0;	//左モータOFF
   		 			PORTBbits.RB7 = 0;	//右モータOFF
					__delay_ms(5);		//wait 5ms
					PORTBbits.RB1 = PORTBbits.RB2 = 0;	//確認用LED off
				}
				break;
				
				//ゴール時に全ての確認用LEDを点滅させる
				case 0b1111:
					PORTBbits.RB0 = PORTBbits.RB1 = PORTBbits.RB2 = PORTBbits.RB3 = 1;	//確認用LED on
					__delay_ms(100);	//wait 100ms
					PORTBbits.RB0 = PORTBbits.RB1 = PORTBbits.RB2 = PORTBbits.RB3 = 0;	//確認用LED off
					__delay_ms(100);	//wait 100ms
					break;
		}
    }
}