#include<stdio.h>
#include<cmath>
using namespace std;

void grid(int num){
    int i=1,j=1,k=0;
    int count=1;
    printf("edge(%d,%d)",i+k*11,i+k*11+1);
    i++;
    while(count < num){
        if(i<=10){
            count++;
            printf(",edge(%d,%d)",i+k*11,i+k*11+1);
            i++;
        }else if(j<=11){
            count++;
            printf(",edge(%d,%d)",j+k*11,j+k*11+11);
            j++;
        }else{
            i=1;j=1;k++;
        }
    }
}

void test(int num){
    int i=1, j=2, k=0;
    int count=1;
    printf("edge(%d,%d)",i,j);
    i++; j++;
    while(count < num){
        if(j < i+6){
            count++;
            printf(",edge(%d,%d)",i,j);
            j++;
        }
        // if(j < i+21){
        //     count++;
        //     printf(",edge(%d,%d)",i,j);
        //     j++;
        else{
            i=j-1;
        }
    }
}

void test_cost1(int num){
    int i=1, j=2, k=0;
    int count=1;
    printf("edge(%d,%d,1)",i,j);
    i++; j++;
    while(count < num){
        if(j < i+6){
            count++;
            printf(",edge(%d,%d,1)",i,j);
            j++;
        }
        // if(j < i+21){
        //     count++;
        //     printf(",edge(%d,%d)",i,j);
        //     j++;
        else{
            i=j-1;
        }
    }
}

void test2(int num){
    int i=2, j=3, k=0;
    int count=1;
    while(count < num){
        if(j < i+6){
            count++;
            printf(",edge(%d,%d)",i,j);
            j++;
        }else{
            i=j-1;
        }
    }
    printf(",edge(1,2)");
}

void test_hl(int num){
    int i=1, j=2, k=0;
    int count=1;
    printf("edge(!H%d:%d,!H%d:%d)",i,i,j,j);
    i++; j++;
    while(count < num){
        if(j < i+6){
            count++;
            printf(",edge(!H%d:%d,!H%d:%d)",i,i,j,j);
            j++;
        }
        // if(j < i+21){
        //     count++;
        //     printf(",edge(%d,%d)",i,j);
        //     j++;
        else{
            i=j-1;
        }
    }
}

void test_hl2(int num){
    int i=1, j=2, k=0;
    int count=1;
    printf("edge(!H%d,!H%d)",i,j);
    i++; j++;
    while(count < num){
        if(j < i+6){
            count++;
            printf(",edge(!H%d,!H%d)",i,j);
            j++;
        }
        // if(j < i+21){
        //     count++;
        //     printf(",edge(%d,%d)",i,j);
        //     j++;
        else{
            i=j-1;
        }
    }
}


void test_cpp(int num){
    int i=1, j=2, k=0;
    int count=1;
    printf("%d\n",num+2);
    printf("%d %d\n",i,j);
    i++; j++;
    while(count < num){
        if(j < i+6){
            count++;
            printf("%d %d\n",i,j);
            j++;
        }
        // if(j < i+21){
        //     count++;
        //     printf(",edge(%d,%d)",i,j);
        //     j++;
        else{
            i=j-1;
        }
    }
}

void test_cpp_cost1(int num){
    int i=1, j=2, k=0;
    int count=1;
    printf("%d\n",num+2);
    printf("%d %d 1\n",i,j);
    i++; j++;
    while(count < num){
        if(j < i+6){
            count++;
            printf("%d %d 1\n",i,j);
            j++;
        }
        // if(j < i+21){
        //     count++;
        //     printf(",edge(%d,%d)",i,j);
        //     j++;
        else{
            i=j-1;
        }
    }
}

void dummy(int num){
    printf("edge(1,0)");
    int count=1;
    while(count < num){
        count++;
        printf(",edge(1,0)");
    }
}

void school(int num){
    //num の数だけ same/2 を生成する
    //sqrt(num) で少数切り捨て　が招待する人と招待される人の数
    //余った分は1人で招待する
    //100 だったら、10人が10人ずつ招待する
    //10000 だったら、100人が100人ずつ招待する
    //1000 だったら、31 人が31人ずつ招待して、残りは1人が招待する
    //本来は、重複する人数はとても少ないことが期待されるので、
    //前の人の招待した人と、自分が招待した人が1人だけ重複することにする

    printf("school(1,-1)");
    int blocks = sqrt(num);
    int count=1;
    int nextnum=2;
    int remainder = num-blocks*blocks;
    int last;

    for (int i = 1; i < blocks+1; i++)
    {
        if(i==1)continue;
        printf(",school(%d,%d)",i,-i);
        nextnum++;
    }
    if(remainder!=0){
        printf(",school(%d,%d)",nextnum,-nextnum);
        last = nextnum;
        nextnum++;
    }

    for(int i = 1; i < blocks+1; i++){
        for (int j = 0; j < blocks; j++)
        {
            printf(",same(%d,%d)",i,nextnum);
            nextnum++;
        }
        nextnum--;
    }
    if(remainder!=0){
        for (; nextnum-1 < num+1; nextnum++)
        {
            printf(",same(%d,%d)",last,nextnum);
        }
    }
}

void gcd1(int num){
    for (int i = 1; i < num; i++)
    {
        printf("gcd(%d),",3*i);
    }
    printf("gcd(%d)",3*num);
    
}

void gcd2(int num){
    for (int i = num; i > 1; i--)
    {
        printf("gcd(%d),",3*i);
    }
    printf("gcd(3)");
}

int main(void){

    int num = 10000 ;
    test_cpp(num);
    // test_cpp_cost1(num);
    // test_cost1(num);
    // test_hl(num);
    // dummy(num);
    // test(num);
    // school(num);
    // gcd2(num);

    return 0;
}