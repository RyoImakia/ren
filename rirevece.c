#include <wiringPi.h>
#include <stdio.h>

#define PIN_IRSENSOR 6                          //wiringPi�̔ԍ����w��(GPIO25)

#define MICROS_LEARNINGTIMEOUT 32000            //00.32�b

void setup(void){                               //setup�֐��ŏ�����
        pinMode( PIN_IRSENSOR, INPUT );         //���̓��[�h
}

void loop(void){

        static int s_iState_prev = HIGH;        //������LowHigh���(�f�t�H���g��High)
        static unsigned int s_uiMicros_prev = 0; //�O���LowHigh���̎���

        static int s_iCountHighLow = 0;         //LowHigh�؂�ւ��̃J�E���g

        int iState = digitalRead( PIN_IRSENSOR );//���݂�6�s���̒l��ǂݎ��
        if(iState != s_iState_prev){
                printf("[%d]",iState);
                unsigned int uiMicros = micros();//�v���O�����X�^�[�g���猻�݂܂ł̎��Ԃ�Ԃ�
                if( 0 == s_uiMicros_prev ){     //1��ڂ̂�
                        s_iCountHighLow = 0;
                }
                else{                           //2��ڈȍ~
                        short shMicros_diff = (short)(uiMicros - s_uiMicros_prev);
                        s_iCountHighLow++;
                        printf( "%d, ", shMicros_diff );
                }
                s_uiMicros_prev = uiMicros;     //����̎��Ԃ�����ɌJ��z��
                s_iState_prev = iState;         //�����LowHigh������ɌJ��z��
                return;
        }

        if( s_uiMicros_prev ){                  //0�ȊO
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
                else{                           //2��ڈȍ~
                        short shMicros_diff = (short)(uiMicros - s_uiMicros_prev);
                        s_iCountHighLow++;
                        printf( "%d, ", shMicros_diff );
                }
                s_uiMicros_prev = uiMicros;     //����̎��Ԃ�����ɌJ��z��
                s_iState_prev = iState;         //�����LowHigh������ɌJ��z��
                return;
        }

        if( s_uiMicros_prev ){                  //0�ȊO
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