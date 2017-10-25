
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <signal.h>
#include <sys/time.h>

int readable = 1;       // �񓯊��ŃC�x���g�����������ꍇ�A�R�[���o�b�N�ɂ��0�ɐݒ肳���
int read_pin = 7;       // ���̓s���ԍ�(wiringpi�̔ԍ�)
int interval = 10;      // �p�����Ԕ���̊Ԋu(us)
int max_wait = 40000;   // �ő�p������(us)

void signalCallBackHandler(int sig){

    readable = 0;
}

double getMoment(){

    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((double)(tv.tv_sec) * 1000000 + (double)(tv.tv_usec));
}

int getInterval(double t1, double t2){

    return (int)(t2-t1);
}


int getTime(int status){

    int count = 0;
    int max = max_wait / interval;
    double start, end;

    start = getMoment();
    while( digitalRead(read_pin) == status ){

        delayMicroseconds(interval);
        count++;
        //�ő�p�����ԓ�����Ԃ��������瑗�M�͏I�����Ă���Ɣ��f
        if(count > max){ break; }
    }
    end = getMoment();
    
        return getInterval(start, end);
}



int scan(FILE *fp){

    // ������W���[���͎�������LOW�ɂȂ�
    if(!digitalRead(read_pin)){ return 1; }

    int on, off;

    // ���M���J�n�����܂őҋ@
    while( readable && digitalRead(read_pin) ){}

    // ��͊J�n
    while( readable ){
        on = getTime(0);
        off = getTime(1);
        fprintf(fp, "%6d %6d\n", on, off);

        //�ő�p�����ԓ�����Ԃ��������瑗�M�͏I�����Ă���Ɣ��f
        if(off > max_wait){ break; }
        
          }

    return 0;
}




int main(int argc, char *argv[]){

    int result;

    // �X�L�����f�[�^�����������t�@�C���̃|�C���^���擾
    FILE *fp;
    char *fileName = "ir.data.txt";

    if(argc >= 2){ fileName = argv[1]; }

    if((fp = fopen(fileName, "w")) == NULL){
        printf("can't open file : %s\n", fileName);
        exit(1);
    }
    printf("write file: %s\n", fileName);
    
       // signal�֐��́A�V�O�i���i�񓯊��C�x���g�j�����������Ƃ��ɁA
    // ���̃V�O�i������M���āA�V�O�i�����L�̏������s���V�O�i�������֐��i�V�O�i���n���h���j��o�^���܂��B
    if(signal(SIGINT, signalCallBackHandler) == SIG_ERR){
        printf("can't set signal\n");
        exit(1);
    }

    // wiringpi�̃Z�b�g�A�b�v
    if(wiringPiSetup() == -1){
        printf("error wiringPi setup\n");
        exit(1);
    }

    // �ǂݎ��s���ԍ�
    if(argc >= 3){
        read_pin = atoi(argv[2]);
    }
    pinMode(read_pin, INPUT);
    printf("scaning pin: %d (wiringpi)\n", read_pin);

    // �ő�p������
    if(argc >= 4){
            max_wait = atoi(argv[3]) * 1000;
    }
    printf("max keep time: %d(ms)\n", max_wait/1000);

    // ��������
    printf("Infrared LED scanning start.\n");
    printf("Pressed Ctrl+C, this program will exit.\n");

    // �X�L�����J�n
    result = scan(fp);

    fclose(fp);

    if(result || !readable){
        printf("\n\n!!! could not scanning. quit.\n\n");
    } else {
        printf("\nScanning has been done.\n\n");
    }

    return 0;
}