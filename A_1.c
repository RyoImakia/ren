#include <stdio.h>

#define Studen 3
#define str "‚Ì“_”‚ğ“ü—Í‚µ‚Ä‚­‚¾‚³‚¢ :"
#define subject_1 "‘Œê"
#define subject_2 "—‰È"
#define subject_3 "Ğ‰ï"

void Sum(int a, int b, int c){

    int sum = a+b+c;
    printf("‡Œv:%d ",sum);
}

void Avarage(int a, int b,int c){

    int avarage = (a+b+c)/Studen;
    printf("•½‹Ï;%d",avarage);
}

int main(void){


    int kokugo,rika,syakai;

    printf("%s%s",subject_1, str);
    fflush(stdout); scanf("%d\n", &kokugo);

    printf("%s%s",subject_2, str);
    scanf("%d\n", &rika);

    printf("%s%s",subject_3, str);
    scanf("%d\n", &syakai);

    Sum(kokugo,rika,syakai);
    Avarage(kokugo, rika, syakai);
}