#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <algorithm>

#include "graph.H"

Graph::Graph(std::vector<int> v_ids, std::vector<long> v_wgts)
{
    for (std::size_t i = 0; i < v_ids.size(); ++i) 
    {
        addVertex(v_ids[i], v_wgts[i]);
        edges.emplace(v_ids[i], std::vector<Edge>{});
    }

    //sort the vertex
    std::sort(vertices.begin(), vertices.end(),
          [](const auto& a, const auto& b) {
              if (a.weight == b.weight)
                  return a.vertex_id > b.vertex_id;
              return a.weight > b.weight;
          });

}
    
/*
//individual vertex and weights passed
    Graph::Graph(int v_ids, int v_wgts)
{

    addVertex(v_ids, v_wgts);
    edges.emplace(v_ids, std::vector<Edge>{});

} */

// add a vertex with its weight
void Graph::addVertex(int vertex, long weight) 
{
    Vertex v{vertex, weight};
    vertices.emplace_back(v);
}

      
// Add weighted edges
void Graph::addEdge(int from, int to, long edgeWeight) 
{
    bool alreadyExists = false;

    // check if from edge already exists before adding
    auto& fromEdges = edges[from];
    for (const auto& e : fromEdges)
        if (e.vertex_2 == to) 
        { 
            alreadyExists = true; 
            return; 
        }
    
    /*
    // check reverse edge
    auto& toEdges = edges[to];
    for (const auto& e : toEdges)
        if (e.vertex_2 == from) 
        { 
            alreadyExists = true; 
            return; 
        }    
    */
    
    if (!alreadyExists) 
    {
        edges[from].emplace_back(Edge{from, to, edgeWeight});
        //edges[to].emplace_back(Edge{to, from, edgeWeight});
    }
}


void Graph::print() const 
{
    for (const auto& node : vertices) 
    {
        std::cout << "Node_" << node.vertex_id << "(weight: " << node.weight << ") ->  \n" ;

        auto it = edges.find(node.vertex_id);
        
        if (it != edges.end()) 
        {
            for (const auto& edge : it->second) 
            {
                if (edge.vertex_1 == node.vertex_id)
                {
                    std::cout << "Node_" << edge.vertex_2 << ": " << edge.weight << "\n";
                }
                    
            }
        }

        std::cout << std::endl;
    }
}
