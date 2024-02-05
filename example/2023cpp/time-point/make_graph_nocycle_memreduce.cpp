#include<stdio.h>
#include<cmath>
#include<random>
#include<vector>
#include<iostream>
#include<list>
#include<algorithm>
#include<tuple>
#include<unordered_set>
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


int findedge(vector<pair<int,int>> vec, pair<int,int> key){
    int left = 0, right = vec.size(), mid;
    while(left < right){
        mid = (left+right)/2;
        if(vec[mid] == key){
            return mid;
        }
        if(vec[mid].first == key.first){
            if(vec[mid].second > key.second){
                right = mid;
            }else{
                left = mid + 1;
            }
        }
        else if(vec[mid].first > key.first){
            right = mid;
        }else if(vec[mid].first < key.first){
            left = mid + 1;
        }
    }
    return -1;
}

int findedge2(vector<pair<int,int>> vec, pair<int,int> key){
    for(int i = 0; i < vec.size(); i++){
        if(vec[i] == key) return i;
    }
    return -1;
}

vector<pair<int,int>> make_edge(int num_E, int num_V){
    constexpr int MAX = 100000000;
    // vector<vector<bool>> edge(num_V, vector<bool>(num_V,false));
    vector<pair<int,int>> edge;
    vector<int> group(num_V,MAX);

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
            // edge[from][to] = true;
            edge.push_back({from,to});
            from_list.push_back(to);
            chance = rnd(engine);
            if(chance < 10){
                from_list.push_back(from);
            }
        }else{
            // edge[to][from] = true;
            edge.push_back({to,from});
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
   num_E -= (num_V - 2);
    while(true){
        
        if(num_E <= 0){
            break;
        }
        std::uniform_real_distribution<float> rnd_from(0, num_V-2);
        from = rnd_from(engine);
        std::uniform_real_distribution<float> rnd_to(from+1, num_V-1);
        to = rnd_to(engine);
        
        if(findedge2(edge, {from,to}) != -1){
            //多重辺禁止
            continue;
        }else if(findedge2(edge, {to,from}) != -1){
            //双方向禁止
            continue;
        }
        // edge[from][to] = true;
        edge.push_back({from,to});
        num_E--;
    }

    group[0] = 0;
    int count = 0;
    list<int> check;
    // vector<vector<int>> groupbyid(10);
    // groupbyid[0].push_back(0);

    unordered_set<int> remain;
    for(int i = 1; i  < num_V; i++){
        remain.insert(i);
    }
    // まず、 0 から順方向に辿れる辺は、group 0 に入れる
    sort(edge.begin(), edge.end());
    for (int i = 0; i < num_V; i++){
        if(findedge(edge, {0,i}) != -1){
            count++;
            group[i] = 0;
            remain.erase(i);
            check.push_back(i);
        } 
    } 

    //group が決定した頂点に向かう辺を調べる
    int node;
    while(true){
        if(check.empty()){
            break;
        }
        // if(count >= num_V){
        //     break;
        // }
        if(remain.empty()){
            break;
        }
        node = check.front();
        check.pop_front();
        
        for(auto i = remain.begin(); i != remain.end();){
            if(findedge(edge, {node,*i}) != -1){
                    group[*i] = group[node];
                    count++;
                    check.push_front(*i);
                    i = remain.erase(i);
            }else if(findedge(edge, {*i,node}) != -1){
                if(group[*i] == MAX){
                    group[*i] = group[node]+1;
                    count++;
                    check.push_back(*i);
                }
                ++i;
            }else{
                ++i; 
            }
        }

        // for(int i = 0; i < num_V; i++){
        //     if(findedge(edge, {node,i}) != -1){
        //             group[i] = group[node];
        //             count++;
        //             cout << "node{ " << i << " }is set group: " << group[i] << endl;
        //             groupbyid[group[i]].push_back(i);
        //             check.push_back(i);
        //     }else if(findedge(edge, {i,node}) != -1){
        //         if(group[i] == MAX){
        //             group[i] = group[node]+1;
        //             if(group[node]+1 > groupbyid.size()){
        //                 groupbyid.resize(group[node]+1);
        //             }
        //             count++;
        //             cout << "node{ " << i << " }is set group: " << group[i] << endl;
        //             groupbyid[group[node]+1].push_back(i);
        //             check.push_back(i);
        //         }
        //     }
            
        // }
    }
    // for(int i = 0; i < num_V; i++){
    //     cout << group[i] << " ";
    // }
    // cout << endl;

    // group が決定したら、同グループで i -> j の辺が存在したとき、i > j なら、i <- j の辺に変える
    for(auto e = edge.begin(); e != edge.end();){
        if(e->first > e->second){
            if(group[e->first] == group[e->second]){
                e = edge.erase(e);
            }else{
                ++e;
            }
        }else{
            ++e;
        }
    }
    
    return edge;
}

vector<tuple<int,int,int,int>> decide_cost(vector<pair<int,int>> edge, int num){
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
    vector<tuple<int,int,int,int>> cost;
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    //一様分布
    std::uniform_real_distribution<float> rnd(0, 99);

    for(auto e: edge){
        int i = e.first;
        int j = e.second;
        if(j == i+1 || j == i-1){
            cost.push_back({1,i,j,5});
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
                    cost.push_back({min,i,j,max});
        }
    }

    return cost;
}

void cpp_timepoint(int num_E, int num_V){
    vector<pair<int,int>> edge = make_edge(num_E, num_V);

    vector<tuple<int,int,int,int>> cost = decide_cost(edge, num_V);
    // start_1 atom の数
    cout << 1 << endl;
    // distance_4 atom の数 (edge の数) 
    cout << num_E << endl;
    // hl id の数 (node の数)
    cout << num_V << endl;

    for(auto i: cost){
        cout << get<0>(i) << " " << get<1>(i) << " " << get<2>(i) << " " << get<3>(i) << endl;
    }
}

void lmn_timepoint(int num_E, int num_V){
    vector<pair<int,int>> edge = make_edge(num_E, num_V);

    vector<tuple<int,int,int,int>> cost = decide_cost(edge, num_V);

    cout << "start(!H0).";
    for(auto i: cost){
        cout << "distance(" << get<0>(i) << ",!H" << get<1>(i) << ",!H" << get<2>(i) << "," << get<3>(i) << ").";
    }
    cout << endl;
}

void timepoint_both(int num_E, int num_V){
    vector<pair<int,int>> edge = make_edge(num_E, num_V);

    vector<tuple<int,int,int,int>> cost = decide_cost(edge, num_V);
    // start_1 atom の数
    cout << 1 << endl;
    // distance_4 atom の数 (edge の数) 
    cout << cost.size() << endl;
    // hl id の数 (node の数)
    cout << num_V << endl;
    //start !H0
    cout << "0" << endl;

    for(auto i: cost){
        cout << get<0>(i) << " " << get<1>(i) << " " << get<2>(i) << " " << get<3>(i) << endl;
    }
    cout << endl;

    cout << "start(!H0).";
    for(auto i: cost){
        cout << "distance(" << get<0>(i) << ",!H" << get<1>(i) << ",!H" << get<2>(i) << "," << get<3>(i) << ").";
    }
    cout << endl;
}

int sigma(int a){
    int ans = 0;
    for(int i = 1; i < a+1; i++){
        ans+=i;
    }
    return ans;
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
    // timepoint_both(3*num, num);
    // timepoint_both(sigma(num)/2, num);
    return 0;
}