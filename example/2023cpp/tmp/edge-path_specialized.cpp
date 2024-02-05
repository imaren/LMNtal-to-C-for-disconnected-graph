//例題に特化したマッチング手法．ちょっと変えればプロパゲーション全般に対応できると思う．
//逆にプロパゲーション以外には使えない？
//LMNtal のマッチングに使えるようにちょっと改良？

// edge-path のやつ
// edge(A,B), path(C,D) \:- B=:=C, ... | path(A,D).

#include <iostream>
#include <unordered_set>
#include <string>
#include <iterator>
#include <fstream>
#include <chrono>

// #define S 4
using namespace std;



struct col{
    int x;
    int y;
};

int min(int a, int b){
    if(a < b)return a;
    else return b;
}

void print_arr(int** src, int S){
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

void print_arr_color(int** src, int x, int y, int S){
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

bool add_history(unordered_set<string> *history, string str){
    // 要素の追加に失敗したら（既にあったら）false
    return (history->insert(str).second);
}

// 0 だったところを変更したとき，そいつについて再帰的にマッチング成功するかどうか確かめる
void rec_rewrite(int** src1, int** src2, int x, int j, int k, unordered_set<string> *history){
    // edge[x][j] path[j][k] に対してマッチングが成功して， path[x][k] が追加された．
        for (int i = 0; i < x; i++)
        {
            // cout << "edge(" << i << "," << x << ")=" << src1[i][x] << " + path(" << x << "," << k << ")=" << src2[x][k];
            if(src1[i][x] > 0){
                    // ひとまず uniq(i,j,k) にする
                string record = to_string(i) + ":" + to_string(x) + ":" + to_string(k); 
                
                // 履歴に追加できたらマッチング成功
                if(add_history(history, record)){
                    //cout << "   succ rec" << endl;

                    int tmp = src2[i][k];
                    src2[i][k]++;
                    // path[i][k] がはじめて追加されたら
                    if(tmp == 0){
                        // 既に探索が終わっている　i についてマッチングを試す 
                        rec_rewrite(src1, src2, i, j, k, history);
                    }
                }
                /*else{
                    cout << "    hist rec" << endl;
                }*/
            }
            /*else{
                cout << "    fail rec" << endl;
            }*/
        }

    return ;
}



int main(int argc, char *argv[]){

    /* 
    * 入力めんどくさいからとりあえず配列の形で渡すことにする
    * めんどくさいから頂点番号の最大値もわかってることにする
    * 
    * 入力形式
    *   <頂点番号の最大値>
    *   <edge_0> <edge_1>
    *   ...
    *   <edge_0> <edge_1>
    */
    string filename = "input.txt";
    if(argc >= 2){
        filename = argv[1];
    }

    // 出力用ファイル 
    string filename2 = "output.csv";


    ifstream i_file(filename);
    if(!i_file.is_open()){
        cerr << "Could not open input file - " << filename << endl;
        return 1;
    }
    int S;

    i_file >> S; 

    int** edge = new int*[S];
    int** path = new int*[S];

    for (int i = 0; i < S; i++)
    {
        edge[i] = new int[S]; path[i] = new int[S];
        for (int j = 0; j < S; j++)
        {
            edge[i][j] = 0; path[i][j]=0;
        }
        
    }

    int link1, link2;
    while (i_file >> link1)
    {
        i_file >> link2;
        edge[link1][link2]++; path[link1][link2]++;
    }
    
    // input end 

    // cout << "init" << endl;
    // print_arr(edge,S);
    // print_arr(path,S);

    unordered_set<string> history;
    

    //edge に対してループを回して，対応する path を見る

    auto start = std::chrono::steady_clock::now();
    for (int j = 0; j < S; j++)
    {
        for (int i = 0; i < S; i++)
        {
            for (int k = 0; k < S; k++)
            {
                //cout << "edge(" << i << "," << j << ")=" << edge[i][j] << " + path(" << j << "," << k << ")=" << path[j][k];
                
                if(edge[i][j] > 0 && path[j][k] > 0){
                    // ひとまず uniq(i,j,k) にする
                    string record = to_string(i) + ":" + to_string(j) + ":" + to_string(k);     

                    // 履歴に追加できたらマッチング成功
                    if(add_history(&history, record)){
                        // cout << "    ok" << endl;    

                        int tmp = path[i][k];
                        path[i][k]++;
                        // path[i][k] がはじめて追加されたら
                        if(tmp == 0){
                            // 既に探索が終わっている　j についてマッチングを試す
                            // (i,j) = (0,0) ~ (i,j) まで ji loop  
                                rec_rewrite(edge, path, i, j, k, &history);
                        }           
                    }
                    /*else{
                        cout << "    hist" << endl;
                    }*/
                }
                /*else{
                    cout << "    fail" << endl;
                }*/
            }   
        }
    }
    auto end = std::chrono::steady_clock::now();
    long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    ofstream o_file;
    o_file.open(filename2, ios::app);

    o_file << duration << endl;

    // cout << "end calc closure" << endl;

    // print_arr(edge,S);
    // print_arr(path,S);

    // for (int i = 0; i < S; i++)
    // {
    //     delete[] edge[i];
    //     delete[] path[i];
    // }
    // delete[] edge;
    // delete[] path;
    
    
    return 0;
}