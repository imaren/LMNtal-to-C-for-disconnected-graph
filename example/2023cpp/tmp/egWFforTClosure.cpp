//ワーシャルフロイドを閉包計算に使うやつの例
//LMNtal のマッチングにはこのままは使えない（uniq の引数によって変わってくるから）
//ただの閉包の計算（個数を数えない）ならこれでもよさげ

#include <iostream>

#define S 4
using namespace std;

int min(int a, int b){
    if(a < b)return a;
    else return b;
}

int main(void){
    int a[4][4] = {
        {0,1,0,0},{0,0,1,0},{0,0,0,1},{0,0,0,0}
    };

    for (int k = 0; k < S; k++)
    {
        cout << "K=" << k << endl;
        for(int i = 0; i < S; i++){
            for (int j = 0; j < S; j++)
            {
                a[i][j] = a[i][j] + min(a[i][k],a[k][j]);
            }   
        }
        for(int i = 0; i < S; i++){
            for (int j = 0; j < S; j++)
            {
                cout << a[i][j] << " ";
            }   
            cout << endl;
        }
        cout << endl;
    }
    return 0;
}