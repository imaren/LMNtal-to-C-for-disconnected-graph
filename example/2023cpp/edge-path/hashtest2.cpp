// 閉包計算
// atom 主導と配列を組み合わせる

// edge-path のやつ
// edge(A,B), path(C,D) \:- B=:=C, ... | path(A,D).

#include <iostream>
#include <unordered_set>
#include <string>
#include <iterator>
#include <fstream>
#include <chrono>
#include <list>
#include <algorithm>

using namespace std;

int h1(int a, int b, int c){
    return a+b+c;
}

int h2(int a, int b, int c){
    return a*2+b*3+c*5;
}

int h3(int a, int b, int c){
    return a*11+b*23+c*53;
}

int h3_2(int a, int b, int c){
    return a*11+b*101+c*1013;
}

int h3_n(int a, int b, int c, int p1, int p2, int p3){
    return a*p1+b*p2+c*p3;
}

int h4_sub(int a){
    return a % 10000000;
}

int h4(int a, int b, int c){
    int tmp = (h4_sub(a) xor 0x123410) * 3.14159 ;
    tmp = (h4_sub(a) xor tmp) * 3.14159;
    return h4_sub((h4_sub(a) xor tmp) *3.14159);
}

int main(int argc, char *argv[]){

    string filename2 = "hashtest2.csv";

    int S = 100;

    ofstream o_file;
    o_file.open(filename2, ios::app);

    unordered_set<int> history;
    unordered_set<int> history2;

    //最大限過密なグラフについて hash 関数のテスト

    /*
        ハッシュ値の重複率を調べる．
        方法がこれで正しそうなことを確認するために cout で重複したときのキーとハッシュ値を出力
        確認用．
    */ 
    // while(true){
    //     int dupl=0;
    //     for (int i = 0; i < S; i++)
    //         {
    //             // std::cout << "now iterate: " << i << endl;
    //             for (int j = 0; j < S; j++)
    //             {
    //                 for (int k = 0; k < S; k++)
    //                 {
    //                     if(!(history.insert(h3_2(i,j,k)).second)){
    //                         cout << i << "," << j << "," << k << "," << h3_2(i,j,k) << endl;
    //                         dupl++;
    //                     }
    //                 }    
    //             }
    //         }
    //     o_file << S << "," << S*S*S << "," << history.size() << "," << dupl << "," << ((double)dupl/(double)(S*S*S))*100 << endl;
    //     history.clear();
    //     S = S + 200;
    //     if(S > 0) break;
    // }

    /*
        h3_n について良さげな素数を調べる
    */
   int primes[] = {
        // 7,11,13,
        // 101, 199, 211, 307, 401, 503, 601, 701, 809,
        // 907, 991, 997, 1009, 1511, 1999, 2003, 2503,
        // 3001, 4001, 5003, 6007, 7001, 8009, 9001, 10007
        90000,300,1
   };
    
    o_file << "p1,p2,p3,";
    o_file << "SIZE,SIZE^3,非重複数,重複要素数,重複率,非重複数（mod S^3 * 3）,重複数（mod S^3 *3）,重複率（mod S^3 * 3）" << endl;
                
    for (int p : primes){
        for (int p2 : primes){
            if(p == p2)continue;
            for (int p3 : primes){
                if(p == p3 || p2 == p3)continue;
                cout << "now: (" << p << "," << p2 << "," << p3 << ")" << endl; 
                int dupl=0;
                int dupl2=0;
                for (int i = 0; i < S; i++){
                    // std::cout << "now iterate: " << i << endl;
                    for (int j = 0; j < S; j++)
                    {
                        for (int k = 0; k < S; k++)
                        {
                            if(!(history.insert(h3_n(i,j,k,p,p2,p3)).second)){
                                dupl++;
                            }
                            if(!(history2.insert(h3_n(i,j,k,p,p2,p3)%(S*S*S)*3).second)){
                                dupl2++;
                            }
                        }    
                    }
                }
                o_file << p << "," << p2 << "," << p3 << "," ;
                o_file << S << "," << S*S*S << "," 
                        << history.size() << "," << dupl << "," << ((double)dupl/(double)(S*S*S))*100  << ","
                        << history2.size() << "," << dupl2 << "," << ((double)dupl2/(double)(S*S*S))*100
                        << endl;
                history.clear();
                history2.clear();
            }
        }
    }   
    
    return 0;
}