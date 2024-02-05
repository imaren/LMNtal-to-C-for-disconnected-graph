#include <fstream>
#include <vector>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <iostream>
#include <unordered_set>
#include <set>
#include <string>
#include <iterator>
#include <fstream>
#include <chrono>
#include <list>
#include <algorithm>
#include <vector>
#include <regex>
#include <sstream>

using namespace std;

typedef
    boost::adjacency_list<boost::listS, boost::vecS, boost::directedS,
        boost::no_property, boost::property<boost::edge_weight_t, int, boost::property<boost::edge_weight2_t, int>>>
Graph;

typedef pair<int, int> Edge;


template <class Graph>
struct weight_label_writer {
    weight_label_writer(const Graph& g) : graph_(g) {}

    template <class Edge>
    void operator()(std::ostream& out, const Edge& edge) const
    {
        // write(out, edge, boost::get(boost::edge_weight, graph_, edge));
        writef(out, edge, boost::get(boost::edge_weight, graph_, edge), boost::get(boost::edge_weight2, graph_, edge));
    }

private:
    template <class Edge, class Weight>
    void write(std::ostream& out, const Edge& edge, const Weight& weight) const
    {
        out << "[weight="
            << weight
            << ", label="
            << weight
            << "]";
    }
    template <class Edge, class Weight>
    void writef(std::ostream& out, const Edge& edge, const Weight& weight, const Weight& cap) const
    {
        out << "[weight="
            << weight
            << ", label=\""
            << cap
            << "/"
            << weight
            << "\"]";
    }

    const Graph& graph_;
};

template <class Graph>
inline weight_label_writer<Graph> make_weight_label_writer(const Graph& g)
{
    return weight_label_writer<Graph>(g);
}

int main(int argc, char *argv[])
{

    string file1 = "check1.txt";
    vector<Edge> edges;
    vector<Edge> cedge;
    vector<int> weights;
    vector<int> caps;
    int hlnum=0;
    if(argc >= 2){
        file1 = argv[1];
    }
    ifstream if1(file1);
    if(!if1.is_open()){
        cerr << "Could not open input file - " << file1 << endl;
        return 0;
    }
    string tmp,in, arg;
    while(getline(if1, tmp)){
        istringstream st(tmp);
        while(getline(st, in, '.')){
            istringstream args(in);
            string tp = "";
            int cnt = 0;
            Edge e;
            int cas = 0;
            if(in.find("cap") != string::npos)cas = 1;
            if(in.find("flow") != string::npos)cas = 2;
            if(in.find("goal") != string::npos)cas = 3;
            while(getline(args, arg, ',')){
                std::smatch matches;
                regex_search(arg, matches, regex("\\d+"));
                if(cnt == 0){
                    e.first = stoi(matches.str());
                }else if(cnt == 1){
                    e.second = stoi(matches.str());
                    if(cas == 1)edges.push_back(e);
                    if(cas == 2)cedge.push_back(e);
                }else if(cnt == 2){
                    if(cas == 0)continue;
                    if(cas == 1)weights.push_back(stoi(matches.str()));
                    if(cas == 2)caps.push_back(stoi(matches.str()));
                }
                if(cas == 3){
                    hlnum = stoi(matches.str()) +1;
                }
                cnt++;
            }
        }
    }
 
    //エッジ、重み、重みの数を入れる。
    // Graph g(edges.begin(), edges.end(), weights.begin(), hlnum);
    // Graph g2(cedge.begin(), cedge.end(), caps.begin(), hlnum);
    Graph g(edges.begin(), edges.end(), weights.begin(),  hlnum);
    boost::graph_traits<Graph>::edge_iterator eit, eend;
    int i = 0;
    for (tie(eit, eend) = boost::edges(g); eit != eend; ++eit) {
        int src = boost::source(*eit, g);
        int trg = boost::target(*eit, g);
        for(auto j =0; j < cedge.size(); j++){
            if(src == cedge[j].first && trg == cedge[j].second){
                boost::put(boost::edge_weight2, g, *eit, caps[j]);
                break;
            }
        }
            
    }

    // graphvizとして出力
    ofstream file("tmp.dot");
    ofstream file2("tmp2.dot");
    boost::write_graphviz(file, g, boost::default_writer(), make_weight_label_writer(g));
    // boost::write_graphviz(file2, g2, boost::default_writer(), make_weight_label_writer(g2));
    
    return 0;
}