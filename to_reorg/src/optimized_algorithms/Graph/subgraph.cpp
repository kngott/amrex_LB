#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <algorithm>

#include "graph.H"
#include "graph_loadbalance.H"
#include <iostream>
#include <vector>
#include <tuple>
#include <map>


Graph_LB::Graph_LB(const Graph& graph, int ranks, long avg_load)
    : G(graph), ranks(ranks), avg_load(avg_load) {}


/*
void Graph_LB::dfs_LoadBalance(const int ranks, const long avg_load)
{
    long bucket_weight = 0, max_weight = -1, total = 0;
    int bucket_idx = 1, current_vertex = 0, min_bucket = 0;
    double efficiency = 0.0, imbalance = 0.0, max_efficiency = -1;

    for(const auto& [vertex, weight]: this->vertices)
    {
        if (visited.find(vertex) == visited.end()) 
        {
            //mark that the vertex has been visited
            visited[vertex] = true;

            //assign nodes to bucket by weights, and get max weight
            max_weight = assignWeights(current_vertex, max_weight);

            std::cout <<"Vertex visited" << vertex << " \n";

            for (const auto& edge: this->edges[current_vertex])
            {
                if (!visited[edge.vertex_2])
                {
                    DFS(edge.vertex_2, this->G, visited);
                }
            }
        }
    }

    efficiency = static_cast<double>(avg_load)/static_cast<double>(max_weight);
    imbalance = static_cast<double>(max_weight)/static_cast<double>(avg_load);

    std::cout << "===== All Vertices Visited ========= \n";
    printBuckets(buckets, weight_map, efficiency, imbalance);
    //return {weight_map, buckets};

    std::cout << "====== Adjust Buckets ======== \n";
}
*/


//traverse the graph using BFS and transfer vertices into buckets based on their weights.
double Graph_LB::bfs_LoadBalance() 
{

    long max_weight = -1, current_weight = 0;
    int  max_bucket = 0, bucket_idx = 0, max_id = 0;
    double efficiency = 0.0, imbalance = 0.0, max_efficiency = -1;
    for(const auto& vertex: G.vertices)
    {

        if (visited.find(vertex.vertex_id) == visited.end()) 
        {
            //mark that the vertex has been visited
            visited[vertex.vertex_id] = true;
            bfs_queue.push(vertex);

            //use bfs to traverse and portion weights
            while (!bfs_queue.empty()) 
            {
                Vertex current_vertex = bfs_queue.front();
                bfs_queue.pop();
                std::cout << "Visited: " << current_vertex.vertex_id << "\n";
               
                //find neighbors of currentVertex
                for (const auto& edge:G.edges.at(current_vertex.vertex_id))
                {
                    if (!visited[edge.vertex_2])
                    {
                        visited[edge.vertex_2] = true;

                        auto it = std::find_if(
                            G.vertices.begin(),G.vertices.end(), [&](const Vertex& v)
                            {
                                return v.vertex_id == edge.vertex_2;
                            });

                        if (it != G.vertices.end())
                        {
                            Vertex vtx = *it;
                            bfs_queue.push(vtx);                   
                        }

                    }
                }

          
                //get weight of current bucket
                if (buckets[bucket_idx].size() > 0)
                {
                    current_weight = 0;
                    for (const auto& vtx: buckets[bucket_idx])
                    {
                        current_weight += vtx.weight; //get weight for current bucket
                    }
                    
                    if (current_weight + current_vertex.weight <= avg_load)
                    {
                    //update bucket with vertex, and then edge gain
                    buckets[bucket_idx].emplace_back(current_vertex); 
                    current_weight += current_vertex.weight;       
                    }
                    else if (bucket_idx < ranks - 1)
                    {
                    bucket_idx++;
                    buckets[bucket_idx].emplace_back(current_vertex);     
                    current_weight = current_vertex.weight;
                    }
                    else 
                    {
                    //update bucket with vertex, and then edge gain
                    buckets[bucket_idx].emplace_back(current_vertex);        
                    current_weight += current_vertex.weight;
                    }
                }
                else
                {
                    buckets[bucket_idx].emplace_back(vertex);
                    current_weight = current_vertex.weight;
                }

                max_weight = std::max(current_weight,max_weight);
                max_bucket = bucket_idx;
                
            }

        }
    }

    std::cout << "===== All Vertices Visited ========= \n";
    //max_id = getBucketTotal();

    getEfficiency();
    std::cout << "inside bfs_LoadBalance \n"; 
    std::cout <<"Efficiency: " << result.second << "\n";

    //std::cout << "====== Adjust Buckets ======== \n\n\n";
    adjustBuckets();

    return result.second;

}

std::pair<int, double> Graph_LB::getEfficiency()
{   
    double eff = 0.0;
    long max_weight = -1;
    int max_id = -1;
    for (int i = 0; i < buckets.size(); ++i) 
    {
        long total_weight = 0;
        std::cout << "Bucket_" << i << " : " ;

        for(const auto& vertex: buckets.at(i))
        {
            std::cout << vertex.vertex_id << ", ";
            total_weight += vertex.weight;
            weight_map[i] = total_weight;
        }

        if (weight_map[i] > max_weight)
        {
            max_weight = weight_map[i];
            max_id = i;
        }
        
        std::cout <<"Bucket weight: " << total_weight << "\n";
    }
    
    //printEfficiency(max_weight);

    eff = static_cast<double>(avg_load)/static_cast<double>(max_weight);

    std::cout << "inside getBucketTotal \n"; 
    std::cout <<"Efficiency: " << eff << "\n";

    return {max_id, eff};
}

//print
void Graph_LB::printEfficiency(long max_weight) 
{
    std::cout << "=== Efficiency Calculation ===\n";
    std::cout << "Load Balance Efficiency: " <<  static_cast<double>(avg_load)/static_cast<double>(max_weight) << "\n"; 
}