#include <wiringPi.h>
#include <stdio.h>

#define PIN_IRSENSOR 6                          //wiringPiの番号を指定(GPIO25)

#define MICROS_LEARNINGTIMEOUT 32000            //00.32秒

void setup(void){                               //setup関数で初期化
        pinMode( PIN_IRSENSOR, INPUT );         //入力モード
}

void loop(void){

        static int s_iState_prev = HIGH;        //初期のLowHigh状態(デフォルトでHigh)
        static unsigned int s_uiMicros_prev = 0; //前回のLowHigh時の時間

        static int s_iCountHighLow = 0;         //LowHigh切り替わりのカウント

        int iState = digitalRead( PIN_IRSENSOR );//現在の6ピンの値を読み取る
        if(iState != s_iState_prev){
                printf("[%d]",iState);
                unsigned int uiMicros = micros();//プログラムスタートから現在までの時間を返す
                if( 0 == s_uiMicros_prev ){     //1回目のみ
                        s_iCountHighLow = 0;
                }
                else{                           //2回目以降
                        short shMicros_diff = (short)(uiMicros - s_uiMicros_prev);
                        s_iCountHighLow++;
                        printf( "%d, ", shMicros_diff );
                }
                s_uiMicros_prev = uiMicros;     //今回の時間を次回に繰り越し
                s_iState_prev = iState;         //今回のLowHighを次回に繰り越し
                return;
        }

        if( s_uiMicros_prev ){                  //0以外
                if( MICROS_LEARNINGTIMEOUT < micros() - s_uiMicros_prev ){      //
                        if( 0 == s_iCountHighLow % 2 ){
                                printf( "\nFailed : Learning\n" );
                        }
                        else{
                                printf( "%d, ", MICROS_LEARNINGTIMEOUT );
                                printf( "\nSucceeded : Learning\n" );
                        }
                        s_uiMicros_prev = 0;
                                    }
                else{                           //2回目以降
                        short shMicros_diff = (short)(uiMicros - s_uiMicros_prev);
                        s_iCountHighLow++;
                        printf( "%d, ", shMicros_diff );
                }
                s_uiMicros_prev = uiMicros;     //今回の時間を次回に繰り越し
                s_iState_prev = iState;         //今回のLowHighを次回に繰り越し
                return;
        }

        if( s_uiMicros_prev ){                  //0以外
                if( MICROS_LEARNINGTIMEOUT < micros() - s_uiMicros_prev ){      //
                        if( 0 == s_iCountHighLow % 2 ){
                                printf( "\nFailed : Learning\n" );
                        }
                        else{
                                printf( "%d, ", MICROS_LEARNINGTIMEOUT );
                                printf( "\nSucceeded : Learning\n" );
                        }
                        s_uiMicros_prev = 0;
                }
        }
}

int main(void){

        wiringPiSetup();
        setup();

        while(1){
                loop();
        }

        return 0;
}