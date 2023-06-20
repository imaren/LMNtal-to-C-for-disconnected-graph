// edge(!L1,!L2,1), edge(!L2,!L3,1), edge(!L2,!L3,3).

// edge(!X,!Y,I) \ edge(!X,!Y,J) :- I < J | .

// edge(!X,!Y,I), edge(!Y,!Z,J) \
//     :- K = I+J,  uniq(!X,!Y,!Z,I,J)  
//     | edge(!X,!Z,K). 

#include <iostream>
#include <unordered_set>
#include <string>
#include <iterator>
#include <fstream>
#include <chrono>
#include <list>
#include <algorithm>
#include <vector>
#include "flat_hash_map.hpp"

using namespace std;

// hyper link がプログラム全体で new されない場合は，
// コンパイル時に 0 から始まる一意の整数 ID を振ればOK
// しかも，ハイパーリンクの名前は保存する必要はないので，
// IDからハイパーリンク名への逆変換は考える必要が無い

// 出現する可能性のある全てのアトムのファンクタについて作る（手書きなので名称略している）
// 一般化してそこに関数とか入れる必要はないんじゃないかと思っている
// (トランスレート時にアトムの種類ごとの最適化とかできるかも)
// 逆に発火可能なルールが存在しないアトムについては文字列表現だけ持っておけば十分
// (ここで注意するべきなのは、ルールの右辺で生成されうるルールについても見ておくこと)
struct s_edge{
    int src; // hl
    int dst; // hl
    int cost; //int

    void p(){
        cout << " edge(!H" << src << ",!H" << dst << "," << cost << ")";
    }

    void p_i(){
        cout << " edge(" << src << "," << dst << "," << cost << ")";
    }
};

struct edge_elem{
    struct edge_elem *prev;
    struct edge_elem *next;
    s_edge atom;
    list<edge_elem*>::iterator used[3];

    void del(){
        prev->next = next;
        next->prev = prev;
    }
};

struct edge_list{
    struct edge_elem *head;
    struct edge_elem *tail;

    edge_elem *dummy = new edge_elem;
    edge_list(){
        head = dummy;
        tail = dummy;
        head->next = tail; tail->prev = head;
    };

    void add(edge_elem *a){
        if(head->next == tail){
            a->prev=head; a->next=tail;
            head->next=a; tail->prev=a;
        }else{
            tail->prev->next = a;
            a->prev = tail->prev; 
            tail->prev = a; 
            a->next = tail; 
        }
    }

    void print_a(edge_elem *atom, edge_elem *t){
        // atom->atom.p();
        atom->atom.p_i();
        if(atom->next != t){
            cout << ".";
            print_a(atom->next,t);
        }
    }
    
    void print(){
        if(this->head->next != this->tail){
            print_a(this->head->next, this->tail);
            cout << ". @4." << endl;    
        }else{
            cout << "head is equal to tail" << endl;
        }
        return;
    }

};


// 実体を表す構造

// ルール1を表す関数と，ルール1で使うためのコンテナ（実体へのポインタ）
// ルール2についても同様



int main(int argc, char *argv[]){
    string filename = "input.txt";
    if(argc >= 2){
        filename = argv[1];
    }

    // 出力用ファイル 
    string filename2 = "result_array_list_flathash.csv";


    ifstream i_file(filename);
    if(!i_file.is_open()){
        cerr << "Could not open input file - " << filename << endl;
        return 1;
    }
    int S1;


    i_file >> S1; 

    constexpr size_t S = VAR +2;

    // edge_list elist = edge_list();
    // vector<vector<vector<edge_elem*>>> rule1(S, vector<vector<edge_elem*>>(S));
    // vector<vector<edge_elem*>> rule2_1(S);
    // vector<vector<edge_elem*>> rule2_2(S);
    // vector<edge_elem*> rule1[S*S];
    // vector<edge_elem*> rule2_1[S];
    // vector<edge_elem*> rule2_2[S];
    // array<array<vector<edge_elem*>,S>,S> rule1;
    // array<vector<edge_elem*>,S> rule2_1;
    // array<vector<edge_elem*>,S> rule2_2;
    // array<vector<edge_elem*>,S*S> rule1;
    // array<vector<edge_elem*>,S> rule2_1;
    // array<vector<edge_elem*>,S> rule2_2;
    
    list<s_edge> push_atom_edge;

    int link1, link2, link3;

    while(i_file >> link1){
        i_file >> link2;
        i_file >> link3;
        s_edge in = {link1,link2,link3};
        push_atom_edge.push_back(in);
    }

    
    auto start = std::chrono::steady_clock::now();

    edge_list elist = edge_list();
    // vector<vector<vector<edge_elem*>>> rule1(S, vector<vector<edge_elem*>>(S));
    // array<vector<edge_elem*>,S> rule2_1;
    // array<vector<edge_elem*>,S> rule2_2;
    vector<vector<list<edge_elem*>>> rule1(S, vector<list<edge_elem*>>(S));
    list<edge_elem*> rule2_1[S];
    list<edge_elem*> rule2_2[S];

    ska::flat_hash_set<string> history;
    s_edge edge_tmp;
    while(true){
        if(push_atom_edge.empty())break;

        edge_tmp = push_atom_edge.front();
        push_atom_edge.pop_front();
        
        //rule1
            // edge(!X,!Y,I) \ edge(!X,!Y,J) :- I < J | .
        bool r1_f = false;
        int s = edge_tmp.src; int d = edge_tmp.dst;

        for (auto i = rule1[s][d].begin(); i != rule1[s][d].end();){
            if(edge_tmp.cost >= (*i)->atom.cost){
                r1_f = true; break;
            }
            else{
                rule2_1[s].erase((*i)->used[1]);
                rule2_2[d].erase((*i)->used[2]);
                (*i)->del();
                delete(*i);
                i = rule1[s][d].erase(i);
            }
        }
        if(r1_f)continue;
        //rule2 
            // edge(!X,!Y,I), edge(!Y,!Z,J) \
            //     :- K = I+J,  uniq(!X,!Y,!Z,I,J)  
            //     | edge(!X,!Z,K). 
        //rule2_1
        for (auto i : rule2_1[d])
        {
            string nhist = to_string(s)+ ":"+to_string(d)+":"+to_string(i->atom.dst)+":"+to_string(i->atom.cost)+":"+to_string(edge_tmp.cost);
            if(history.insert(nhist).second){
                s_edge push = {s,i->atom.dst,edge_tmp.cost+i->atom.cost};
                push_atom_edge.push_back(push);
            }
        }
        //rule2_2
        for (auto i : rule2_2[s])
        {
            string nhist = to_string(i->atom.src)+ ":"+to_string(s)+":"+to_string(d)+":"+to_string(edge_tmp.cost)+":"+to_string(i->atom.cost);
            if(history.insert(nhist).second){
                s_edge pedge = {i->atom.src,d,edge_tmp.cost+i->atom.cost};
                push_atom_edge.push_back(pedge);
            }
        }

        //add
        // cout << "add" << endl;
        edge_elem *n = new edge_elem;
        // cout << edge_tmp.src <<" " << edge_tmp.dst << " " << edge_tmp.cost << endl;
        n->atom = edge_tmp;
        rule1[edge_tmp.src][edge_tmp.dst].push_back(n);
        n->used[0]=rule1[edge_tmp.src][edge_tmp.dst].end();
        --n->used[0];
        rule2_1[edge_tmp.src].push_back(n);
        n->used[1]=rule2_1[edge_tmp.src].end();
        --n->used[1];
        rule2_2[edge_tmp.dst].push_back(n);
        n->used[2]=rule2_2[edge_tmp.dst].end();
        --n->used[2];

        // cout << &n->used[0] << endl << &n->used[1] << endl;
        elist.add(n);
    }
    auto end = std::chrono::steady_clock::now();
    long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    ofstream o_file;
    o_file.open(filename2, ios::app);

    o_file << duration << ",";

    // elist.print();
    return 0;
}