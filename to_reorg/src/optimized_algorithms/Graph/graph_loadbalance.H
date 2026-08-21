#pragma once

#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>

#include "Graph.H"


struct gainVertexData
{
    Vertex vertex;
    int vertex_gain;
};


struct Graph_LB
{
    const Graph& G;

    int ranks;
    long avg_load;

    std::unordered_map<int, bool> visited;
    std::queue<Vertex> bfs_queue;
    std::unordered_map<int, std::vector<Vertex>> buckets;
    std::unordered_map<int, long> weight_map;
    std::pair<int, double> result;
    std::map<int, std::queue<Vertex>> gain_map;


    explicit Graph_LB(const Graph& graph, int ranks, long avg_load);
    
    // methods
    double bfs_LoadBalance();
   // void dfs_LoadBalance(const int, const long);
    long  assignWeights(const long& vtx, long max_weight);
    void printEfficiency(long max_weight);     
    void adjustBuckets();
    std::pair<int, double> getEfficiency();
    int calculateGain(const Vertex& vertex, int bucket_idx);
    gainVertexData checkVertexGain(const Vertex& vertex_move, int move_bucket_idx, int vertex_move_gain);
    std::pair<int, int> checkBucketGain(const Vertex& vertex_move, int move_bucket_idx);
    void moveVertex(const Vertex& vertex_move, int move_bucket_idx, int bucket_receive_id);
    
};