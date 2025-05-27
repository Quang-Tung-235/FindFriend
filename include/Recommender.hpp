#pragma once
#include "Graph.hpp"
#include <vector>
using namespace std;
class Recommender {
public:
    struct Candidate { string id; double score; };
    explicit Recommender(const Graph& g): g_(g) {}
    vector<Candidate> suggest(const string& uid, size_t k=5,
                                   double wMutual=1.0, double wJaccard=1.0) const;
private:
    const Graph& g_;
};