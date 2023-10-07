#include<stdio.h>
#include<cmath>
#include<random>
#include<vector>
#include<iostream>
#include<list>
using namespace std;

void uniform_spath(int num){
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    //一様分布
    std::uniform_real_distribution<float> dist(0, num);
    printf("edge(%d,%d)",(int)dist(engine),(int)dist(engine));
    for (int i = 1; i < num; i++)
    {
            printf(", edge(%d,%d,%d)\n",(int)dist(engine),(int)dist(engine),(int)dist(engine));
    }   
}

void uniform_cpp_spath(int num){
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    //一様分布
    std::uniform_real_distribution<float> dist(0, num);
    printf("%d\n",num+2);
    for (int i = 0; i < num; i++)
    {
            printf("%d %d %d\n",(int)dist(engine),(int)dist(engine),(int)dist(engine));
    }   
}


/**
 * time-point (簡単なスケジューリング)問題において、初期状態を作るための初期状態の条件
 * 
 * 開始地点（計算の起点）は 1つである（今回は id 0 を開始地点とするが id 0 の地点が開始地点でなくてもよい）
 * 地点Aから地点Bに遷移するための最小・最大時間が整数で与えられ、distance(min,A,B,max)で表現される
 *  最小時間 <= 最大時間 でなければならない
 * distance(min,A,B,max)　はタスクであると考えてよい．各地点ではタスクが複数発行される場合がある．
 *  つまり、1つの地点からいくつでも distance edge が出入りしても良い
 * サイクルを含んではならない
 * 地点Aから地点Bに行くための経路が複数存在するとき全ての最小・最大時間の区間に交わりがなければならない
 *  例：1~5, 4~6 なら交わりがある．1~3, 5~7 なら交わりがない
 *  生成しながら、各地点までの最小・最大時間を持っておく．
 * distance(min,A,B,max)は、A で発行され B への遷移が必要な全てのタスクが最低 min 分、最大 max 分で終了することを意味する
 *  つまり…今回は多重辺は許さない．
 * ルートは単一とは限らない．後ろ向きのプロパゲーションのために、逆向きの辺を一定確率で生成する．ただし、逆向きの辺は正向きの辺よりも十分少ない
 *  どのくらいにする…？
 * 
 * 生成アルゴリズム↓
 * 
 *  1. 辺の本数を決める(最大4本にする)
 *  1: 100%
 *  +1: 20%
 *  +1: 5%
 *  +1: 1%
 *  
 *  2. 辺の伸び方を決める
 *  順方向: 70%
 *  逆方向: 30% 
 * 
 *  3. 順方向は枝分かれするか、どこかに合流するか決める．その時、id が自分よりも大きいノードに向かってしか生えない．
 * 　1本目の辺と逆方向の辺は、次の id に向かって生える（今まで生成された辺の id の中で最も大きいもの + 1 ）
 *   2本目以降の辺は順方向なら 自身の id ~ 生成される予定のid までの範囲でランダムな id に向かって辺を生やす．
 *   このとき、近い方が確率が高く、遠い方が確率が低い
 * 
 * */ 
vector<vector<bool>> make_edge(int num_E, int num_V){
    vector<vector<bool>> edge(num_V, vector<bool>(num_V,false));

    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    //一様分布
    std::uniform_real_distribution<float> rnd(0, 99);

    list<int> from_list;
    int id=1;

    from_list.push_back(0);

    int from;
    int to;

    /**
     * 
     * はじめに頂点の数まで辺を伸ばす
     *  順方向が80%, 逆方向が20%
     *  順方向に辺が伸びた場合は、
     *   10% で枝分かれする
     *  逆方向の辺が伸びた場合は、
     *   70% 逆方向に伸びた辺の元の頂点から次の辺を伸ばす
     *   20% 逆方向に伸びた辺の元の頂点と先の頂点から次の辺を伸ばす
     *   10% 逆方向に伸びた辺の先の頂点から次の辺を伸ばす
    **/
    for(int i = 1; i < num_V; i++){
        from = from_list.front();
        from_list.pop_front();
        to = i;

        float chance  = rnd(engine);
        if(chance < 80){
            edge[from][to] = true;
            from_list.push_back(to);
            chance = rnd(engine);
            if(chance < 10){
                from_list.push_back(from);
            }
        }else{
            edge[to][from] = true;
            chance = rnd(engine);
            if(chance < 70){
                from_list.push_back(from);
            }else if(chance < 90){
                from_list.push_back(from);
                from_list.push_back(to);
            }else{
                from_list.push_back(to);
            }
        }
    }

    /**
     * 次に残りの辺を適当に伸ばす
     *  辺を伸ばす元の頂点を抽選する
     *   num_V - 2 まで
     *   辺を伸ばす先の頂点を抽選する
     *    自分の頂点番号 < 先の頂点 < num_V - 1
     *    すでに逆向きの辺が生成されている場合があるので、チェックして該当したら再抽選
     * 
     * 最後に、ちょうどいい辺のコストを適当に決める
    */
   num_E -= (num_V - 1);
    while(true){
        if(num_E <= 0){
            break;
        }

        std::uniform_real_distribution<float> rnd_from(0, num_V-2);
        from = rnd_from(engine);
        std::uniform_real_distribution<float> rnd_to(from+1, num_V-1);
        to = rnd_to(engine);

        if(edge[from][to] == 1){
            //多重辺禁止
            continue;
        }else if(edge[to][from] == 1){
            //双方向禁止
            continue;
        }
        edge[from][to] = true;
        num_E--;
    }
    return edge;
}

vector<vector<pair<int,int>>> decide_cost(vector<vector<bool>> edge, int num){
    /** 最低時間, 最大時間 の組を作る
     *  最低時間
     *    i <-> i+1 の辺なら 1 にする 
     *    i <-> j (j > i+1) の辺なら 1 か j-i+1 か 50% vs 50%
     *    (i -> ... -> j の辺と i -> j の辺で、大小関係のみが大切（具体的な数字は関係ない）はず)
     *  最大時間
     *    i -> i+1 の辺なら 3 にする
     *    i -> j (j > i+1) の辺なら  (j-i)*3 + 1 か (j-i)*3 - 1 50% vs 50%
     *    (最低時間との矛盾を避けつつ、上記と同様大小関係のみが大切という仮定の元)
     **/ 
    vector<vector<pair<int,int>>> cost(num, vector<pair<int,int>>(num,{-1,-1}));
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    //一様分布
    std::uniform_real_distribution<float> rnd(0, 99);

    for (int i = 0; i < num; i++)
    {
        for(int j = 0; j < num; j++){
            if(edge[i][j]){
                if(j == i+1 || j == i-1){
                    cost[i][j] = {1,5};
                }else{
                    float chance = rnd(engine);
                    int min,max;
                    if(chance < 50){
                        min = abs(j-i)+1;
                    }else{
                        min = 1;
                    }
                    chance = rnd(engine);
                    if(chance < 50){
                        max = abs(j-i)*5+1;
                    }else{
                        max = abs(j-i)*5-1;
                    }
                    cost[i][j] = {min,max};
                }
                
            }
        }
    }
    return cost;
}

void cpp_timepoint(int num_E, int num_V){
    vector<vector<bool>> edge = make_edge(num_E, num_V);

    vector<vector<pair<int,int>>> cost = decide_cost(edge, num_V);
    // start_1 atom の数
    cout << 1 << endl;
    // distance_4 atom の数 (edge の数) 
    cout << num_E << endl;
    // hl id の数 (node の数)
    cout << num_V << endl;

    for(int i = 0; i < num_V; i++){
        for(int j = 0; j < num_V; j++){
            if(edge[i][j]){
                cout << cost[i][j].first << " " << i << " " << j << " " << cost[i][j].second << endl; 
            }
        }
    }
}

void lmn_timepoint(int num_E, int num_V){
    vector<vector<bool>> edge = make_edge(num_E, num_V);

    vector<vector<pair<int,int>>> cost = decide_cost(edge, num_V);

    cout << "start(!H0).";
    for(int i = 0; i < num_V; i++){
        for(int j = 0; j < num_V; j++){
            if(edge[i][j]){
                cout << "distance(" << cost[i][j].first << ",!H" << i << ",!H" << j << "," << cost[i][j].second << ")." ; 
            }
        }
    }
    cout << endl;
}

void timepoint_both(int num_E, int num_V){
    vector<vector<bool>> edge = make_edge(num_E, num_V);

    vector<vector<pair<int,int>>> cost = decide_cost(edge, num_V);
    // start_1 atom の数
    cout << 1 << endl;
    // distance_4 atom の数 (edge の数) 
    cout << num_E << endl;
    // hl id の数 (node の数)
    cout << num_V << endl;
    //start !H0
    cout << "0" << endl;

    for(int i = 0; i < num_V; i++){
        for(int j = 0; j < num_V; j++){
            if(edge[i][j]){
                cout << cost[i][j].first << " " << i << " " << j << " " << cost[i][j].second << endl; 
            }
        }
    }
    cout << endl;

    cout << "start(!H0).";
    for(int i = 0; i < num_V; i++){
        for(int j = 0; j < num_V; j++){
            if(edge[i][j]){
                cout << "distance(" << cost[i][j].first << ",!H" << i << ",!H" << j << "," << cost[i][j].second << ")." ; 
            }
        }
    }
    cout << endl;
}



int main(int argc, char *argv[]){

    int num = 10 ;
    string in;
    if(argc >= 2){
        in = argv[1];
        num = stoi(in);
    }
    // lmn_timepoint(2*num, num);
    // cpp_timepoint(2*num, num);
    timepoint_both(2*num, num);

    return 0;
}