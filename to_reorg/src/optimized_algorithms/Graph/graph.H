#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>


struct Vertex 
{
    int vertex_id;
    long weight;

    Vertex()
        : vertex_id(-1), weight(0) {}

    Vertex(int id, long weight = 1)
        : vertex_id(id), weight(weight) {}
};

//structure for edges
struct Edge 
{
    int    vertex_1;
    int    vertex_2;
    long   weight;   
    Edge(int from, int to, long weight = 1)
        : vertex_1(from), vertex_2(to), weight(weight) {}
};

//structure for graph
struct Graph 
{
    //Create data structure for node and edges with their respective weights
    std::vector<Vertex> vertices; 
    std::unordered_map<int, std::vector<Edge>> edges;

    Graph(std::vector<int> v_ids, std::vector<long> v_wgts);
 
    // methods for graph
    void addVertex(int,long);
    void addEdge(int,int,long);
    void print() const;
    //void bfs_loadbalance(const int, const long);
    //void printBuckets(const std::unordered_map<int, std::pair<std::vector<Vertex>, long>>& buckets,
      //  std::unordered_map<int, long>& weight_map, double, double) const;
};




