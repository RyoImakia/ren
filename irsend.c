

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>

#define BUF_LEN   256

int write_pin = 0;

int Hz = 38;            // ���g��(KHz)
int duty_num = 1;       // duty��(���q)
int duty_denomi = 3;    // duty��(����)
int unit = 26;          // ���j�b�g��us
int duty_high = 9;      // HIGH����
int duty_low = 17;      // LOW����
int repeat = 1;         // ���M��



void readAndSend(FILE *fp){

    char buf[BUF_LEN];
    int i, j, length = 0;
    int *on, *off;

    while( fgets(buf, BUF_LEN, fp) != NULL){ length++; }
    on = (int *)calloc(length, sizeof(int));
    off = (int *)calloc(length, sizeof(int));

    rewind(fp);

    for(i=0; i<length; i++){
        fscanf(fp,"%d %d", &on[i], &off[i]);
    }

    // ���M�i��������repeat�񑗐M����)
    for(j=0; j<repeat; j++){

        printf("send data.\n");

        for(i=0; i<length; i++){
            output(on[i], off[i]);
        }
        
                usleep(50000);
    }

    // �A���P�[�g���������������
    free(on);
    free(off);
}

void output(int on_time, int off_time){

    // �ԊO���_��
    high(on_time);

    // �ԊO������
    digitalWrite(write_pin, 0);
    delayMicroseconds(off_time);
}


void high(int on_time){

    // �p���X�M���ɕϊ����đ��M
    int i;
       int count = on_time/unit;
    for(i=0; i<count; i++){

        digitalWrite(write_pin, 1); //high
        delayMicroseconds(duty_high);

        digitalWrite(write_pin, 0); //low
        delayMicroseconds(duty_low);
    }
}


int main(int argc, char *argv[]){

        // ���M�f�[�^�t�@�C��
        FILE *fp;
        char *fileName = "ir.data.txt";
        if(argc >= 2){ fileName = argv[1]; }

        if((fp = fopen(fileName, "r")) == NULL){
                printf("can't open file: %s\n", fileName);
                exit(1);
        }
               printf("read file: %s\n", fileName);

        // wiringpi�̃Z�b�g�A�b�v
        if(wiringPiSetup() == -1){
                printf("error wiringPi setup\n");
                exit(1);
         }

         // ���M�J��Ԃ���
        if(argc >= 3){
                repeat = atoi(argv[2]);
        }

        // ���M�s���ԍ�
        if(argc >= 4){
                write_pin = atoi(argv[3]);
        }

        pinMode(write_pin, OUTPUT);
        printf("output pin: %d (wiringpi)\n", write_pin);

        // ���g���̐ݒ�
              if(argc >= 5){ Hz = atoi(argv[4]); }

        // duty��̕��q
        if(argc >= 6){ duty_num = atoi(argv[5]); }

        // duty��̕���
        if(argc >= 7){ duty_denomi = atoi(argv[6]); }

        // unit��
        unit = (1.0f / (Hz * 1000)) * 1000000;
        duty_high = roundf(((float)unit / duty_denomi) * duty_num);
        unit = (int)unit;     //us
        duty_low = unit - duty_high;

        printf("unit: %dms duty:%d-%d\n", unit, duty_high, duty_low);

        // ��������
        printf("send infrared signal.\n");

        // �f�[�^�ǂݍ��݂ƐԊO���̑��M
        readAndSend(fp);

        printf("\ndone\n");
                printf("\ndone\n");
        fclose(fp);

        return 0;
}