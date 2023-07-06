/*
参考
https://zenn.dev/reputeless/books/standard-cpp-for-competitive-programming/viewer/dijkstra
*/

#include <iostream>
#include <vector>
#include <utility> // std::pair
#include <queue> // std::priority_queue
#include <functional> // std::greater

constexpr long long INF = (1LL << 60);

// 辺の情報
struct Edge
{
	// 行先
	int to;

	// コスト
	int cost;
};

using Graph = std::vector<std::vector<Edge>>;

// { distance, from }
using Pair = std::pair<long long, int>;

// ダイクストラ法 (1.1 基本実装)
// distances は頂点数と同じサイズ, 全要素 INF で初期化しておく
void Dijkstra(const Graph& graph, std::vector<long long>& distances, int startIndex)
{
	// 「現時点での最短距離, 頂点」の順に取り出す priority_queue
	// デフォルトの priority_queue は降順に取り出すため std::greater を使う
	std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> q;
	q.emplace((distances[startIndex] = 0), startIndex);

	while (!q.empty())
	{
		const long long distance = q.top().first;
		const int from = q.top().second;
		q.pop();

		// 最短距離でなければ処理しない
		if (distances[from] < distance)
		{
			continue;
		}

		// 現在の頂点からの各辺について
		for (const auto& edge : graph[from])
		{
			// to までの新しい距離
			const long long d = (distances[from] + edge.cost);

			// d が現在の記録より小さければ更新
			if (d < distances[edge.to])
			{
				q.emplace((distances[edge.to] = d), edge.to);
			}
		}
	}
}

int main()
{
	int V, E, r;
    int hoge;
    std::cin >> hoge;
	std::cin >> E >> V >> r;

	Graph graph_min(V);
    Graph graph_max(V);
	while (E--)
	{
		int min, s, t, max;
		std::cin >> min >> s >> t >> max;
		graph_min[s].push_back({ t, min });
        graph_min[t].push_back({ s, min });
        graph_max[s].push_back({ t, max });
        graph_max[t].push_back({ s, max });
	}

	std::vector<long long> distances_min(V, INF);
    std::vector<long long> distances_max(V, INF);
	Dijkstra(graph_min, distances_min, r);
    Dijkstra(graph_max, distances_max, r);

	for (size_t i = 0; i < V; i++)
    {
        
    }
    
}