#include <vector>
#include <random>

constexpr int size = 5000;

static void stdhash(benchmark::State& state){
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());

    std::uniform_real_distribution<int> rnd(0, size);
    
    for(auto _ : state){
        int a = rnd(engine); 
        int b = rnd(engine);
        int c = rnd(engine);
        
        int hash = std::hash(a,b,c);
    }
}