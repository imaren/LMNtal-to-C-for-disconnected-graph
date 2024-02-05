//ワーシャルフロイドを閉包計算に使うやつの例
//LMNtal のマッチングに使えるようにちょっと改良？

#include <iostream>

#define S 4
using namespace std;

struct col{
    int x;
    int y;
};

int min(int a, int b){
    if(a < b)return a;
    else return b;
}

void print_arr(int src[][S]){
    for (int i = 0; i < S; i++)
    {
        for (int j = 0; j < S; j++)
        {
            cout << src[i][j] << "  " ;
        }
        cout << endl;
    }
    cout << endl;
}

void print_arr_color(int src[][S], int x, int y){
    for (int i = 0; i < S; i++)
    {
        for (int j = 0; j < S; j++)
        {
            if(i == x && j == y){
                cout << "\033[31m" << src[i][j] << "\033[m  ";
            }else{
                cout << src[i][j] << "  " ;
            }
        }
        cout << endl;
    }
    cout << endl;
}

// zero だったところを変更したとき，そいつについて再帰的にマッチング成功するかどうか確かめる
void rec_rewrite(int src[][S], int x, int y, int k){
    for (int i = 0; i < k; i++)
    {
        if(src[y][i]>=1){
            cout << "rec1:(" << x << "," << y << "),(" << y << "," << i << ")" << endl;
            if(src[x][i]==0){
                src[x][i]=1;
                print_arr(src);
                rec_rewrite(src,x,i,k);
            }else{
                print_arr(src);
                src[x][i]++;
            }
        } 
    }
    for (int i = 0; i < k; i++)
    {
        if(x == y && y == i)continue;
        if(src[i][x]>=1){
            cout << "rec2:(" << i << "," << x << "),(" << x << "," << y << ")" << endl;
            if(src[i][y]==0){
                src[i][y]=1;
                print_arr(src);
                rec_rewrite(src,i,x,k);
            }else{
                print_arr(src);
                src[i][y]++;
            }
            
        } 
    }
    return ;
}

int main(void){
    int a[4][4] = {
        {0,1,0,0},{1,0,1,0},{0,0,0,0},{0,0,0,0}
    };
    cout << "init" << endl;
    print_arr(a);
    for (int k = 0; k < S; k++)
    {
        std::cout << "K=" << k << endl;
        for(int i = 0; i < S; i++){
            for (int j = 0; j < S; j++)
            {
                if(a[i][k]>=1 && a[k][j]>=1){
                    std::cout << "loop:(" << i << "," << k << "),(" << k << "," << j << ")" << endl;
                    if(a[i][j]==0){
                        a[i][j]++;
                        print_arr_color(a,i,j);
                        rec_rewrite(a,i,j,k);
                    }else{
                        print_arr_color(a,i,j);
                        a[i][j]++;
                    }
                    
                }else{
                    print_arr_color(a,i,j);
                }
            }   
        }
        print_arr(a);
        std::cout << endl;
    }
    return 0;
}