//参考
//https://zenn.dev/reputeless/books/standard-cpp-for-competitive-programming/viewer/floyd-warshall#1.1-%E5%9F%BA%E6%9C%AC%E5%AE%9F%E8%A3%85


#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <limits.h>
#include <chrono>

using namespace std;

constexpr int INF = 10000000;

// ワーシャルフロイド法 (1.1 基本実装)
// 負閉路が存在する場合 true を返す
bool FloydWarshall(std::vector<std::vector<int>>& distances)
{
	const size_t v = distances.size();

	for (size_t i = 0; i < v; ++i)
	{
		for (size_t from = 0; from < v; ++from)
		{
			for (size_t to = 0; to < v; ++to)
			{
				if ((distances[from][i] < INF) && (distances[i][to] < INF))
				{
					distances[from][to] = std::min(distances[from][to], (distances[from][i] + distances[i][to]));
				}
			}
		}
	}

	// for (size_t i = 0; i < v; ++i)
	// {
	// 	// 負閉路が含まれている場合, distances[i][i] が負になるような i が存在する
	// 	if (distances[i][i] < 0)
	// 	{
	// 		return true;
	// 	}
	// }

	return false;
}

void print_distance(vector<vector<int>> distance){
    int v = distance.size();
    for (size_t i = 0; i < v; i++)
    {
        for (size_t j = 0; j < v; j++)
        {
            if(i == j)continue;
            if(distance[i][j] == INF)continue;
            cout << " edge(" << i << "," << j << "," << distance[i][j] << ").";
        }
        
    }
    
}

int main(int argc, char *argv[]){
    string filename = "input.txt";
    if(argc >= 2){
        filename = argv[1];
    }

    // 出力用ファイル 
    string filename2 = "result_wf.csv";


    ifstream i_file(filename);
    if(!i_file.is_open()){
        cerr << "Could not open input file - " << filename << endl;
        return 1;
    }
    int S1;
    i_file >> S1; 

    constexpr size_t S = VAR +2;

    int link1, link2, link3;

    vector<vector<int>> distance(S,vector<int>(S, INF));

    while(i_file >> link1){
        i_file >> link2;
        i_file >> link3;
        if(distance[link1][link2] > link3){
            distance[link1][link2]=link3;
        }
        
    }

    for (size_t i = 0; i < S; i++)
    {
        distance[i][i]=0;
    }
    

    auto start = std::chrono::steady_clock::now();

    FloydWarshall(distance);

    auto end = std::chrono::steady_clock::now();
    long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    ofstream o_file;
    o_file.open(filename2, ios::app);

    o_file << duration << ",";

    // print_distance(distance);
    // cout << " @4." << endl;

    return 0;
}