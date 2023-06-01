/*
    imagawa
    int 型の数値を n 個受け取って、それに対する hash table を作成する
    わかりやすい概要解説があった
    https://www.docswell.com/s/kumagi/ZGXXRJ-hash-table-world-which-you-dont-know#p30
    closed addressing(open hash) : 遅いけど安定 (ポインタで繋ぐのでキャッシュヒットしない)
    open addressing(closed hash) : 速い．（キャッシュにヒットする可能性が高い）
                     履歴管理に使うだけなので削除が起こらない．
    Cuckoo hashing : 検索が速い. 2つのテーブルを使う．
                　　 挿入時に既に要素があったら既にある要素を蹴りだして逆のテーブルに挿入しようとする．
                     cchr ではこれが使われている．多分、マッチング成功するよりも履歴で弾く方が多いから？
                     密度が高い時極端に性能が悪くなる
    Hopscotch      : open + closed 合わせ技．高密度でも動く．

    とりあえずサイズ決め打ち？？？？？
    リハッシュのことを考えずに済むが汎用性は全くない（メモリ的に）

*/

#include <cstdint>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <iostream>

// TODO: コンパイル時にサイズがいい感じに設定できるような例題の場合はそこで決め打ちしてしまう
// TODO: より汎用的にするためにはハッシュ表のサイズは可変にする必要がある
constexpr int SIZE = 100000000;

// コンパイル時にアトムの引数が取りうる最大値がわかる場合
constexpr int ARG_MAX = 300;


namespace ihash{

    class hash_table{
        public:
        hash_table(){
            this->bucket_size = SIZE;
            this->item_size = 1;
            this->table = (int32_t *)malloc(sizeof(int32_t) * bucket_size * item_size);
            this->table2 = (int32_t *)malloc(sizeof(int32_t) * bucket_size * item_size);
            if(table2 == NULL) {
                std::cout << "cannot malloc table2 size: " << bucket_size << "*" << item_size << std::endl; 
            }
            for (int i = 0; i < bucket_size*item_size; i++)
            {
                table[i] = INT_MIN; table2[i] = INT_MIN;
            }
            
        }

        hash_table(uint64_t bsize, uint16_t isize){
            this->bucket_size = bsize;
            this->item_size = isize;
            this->table = (int32_t *)malloc(sizeof(int32_t) * bucket_size * item_size);            
            this->table2 = (int32_t *)malloc(sizeof(int32_t) * bucket_size * item_size);
            if(table2 == NULL) {
                std::cout << "cannot malloc table2 size: " << bucket_size << "*" << item_size << std::endl; 
            }
            for (int i = 0; i < bucket_size*item_size; i+=item_size)
            {
                table[i] = INT_MIN; table2[i] = INT_MIN;
            }
        }

        ~hash_table(){
            free(table);
            free(table2);
        }

        uint32_t times(uint32_t n, uint32_t a){
            // n^a
            uint32_t ans = n;
            for (int i = 1; i < a; i++)
            {
                ans = ans * n;
            }
            return ans;
        }

        uint32_t hashA(int32_t *items){
            uint32_t value=items[0];
            for (int i = 1; i < this->item_size; i++)
            {
                value += items[i] * times(ARG_MAX,i);
            }
            return value % bucket_size;
        }

        uint32_t hashB(int32_t *items){
            uint32_t value=0;
            for (int i = 0; i < this->item_size; i++)
            {
                value += items[i] * primes[i];
            }
            return value % bucket_size;
        }

        bool insert(int32_t *key){
            return emplaceA(key);
        }

        bool emplaceA(int32_t *key){
            uint32_t value = hashA(key); 

            if(table[value*item_size] == INT_MIN){
                memcpy(table+value*item_size, key, sizeof(int32_t)*item_size);
                return true;
            }

            if(!compareA(key,hashA(key))){
                int32_t *cuckoo = (int32_t *)malloc(sizeof(int32_t)*item_size);
                memcpy(cuckoo, table+value*item_size, sizeof(int32_t)*item_size); 
                emplaceB(cuckoo);
                memcpy(table+value*item_size, key, sizeof(int32_t)*item_size);
                return true;
            }
            return false;
        }

        bool emplaceB(int32_t *key){
            uint32_t value = hashB(key); 

            if(table2[value*item_size] == INT_MIN){
                memcpy(table2+value*item_size, key, sizeof(int32_t)*item_size);
                return true;
            }
            if(!compareB(key,value)){
                int32_t *cuckoo = (int32_t *)malloc(sizeof(int32_t)*item_size);
                memcpy(cuckoo, table2+value*item_size, sizeof(int32_t)*item_size); 
                emplaceA(cuckoo);
                memcpy(table2+value*item_size, key, sizeof(int32_t)*item_size);
                return true;
            }
            return false;
        }

        bool compareA(int32_t *item, uint32_t hash){
            for(int i = 0; i < item_size; i++){
                // not same
                if(item[i] ^ table[this->item_size * hash + i])return false;
            }
            //same
            return true;
        }

        bool compareB(int32_t *item, uint32_t hash){
            for(int i = 0; i < item_size; i++){
                // not same
                if(item[i] ^ table2[this->item_size * hash + i])return false;
            }
            //same
            return true;
        }

      private:
        uint64_t bucket_size;
        uint16_t item_size;
        int32_t *table;
        int32_t *table2;
        uint32_t primes[5] = {
            1153, 1171, 1187, 1229, 1237
        };

    };
}


