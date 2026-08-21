#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <algorithm>

#include "graph.H"


//traverse the graph using BFS and transfer vertices into buckets based on their weights.
//std::tuple< std::unordered_map<int, long>, std::unordered_map<int, std::pair<std::vector<Vertex>, long>>> 
void Graph::bfs_loadbalance(const int ranks, const long avg_load) 
{
    std::unordered_map<int, bool> visited;
    std::queue<int> bfs_queue;
    std::unordered_map<int, std::pair<std::vector<Vertex>, long>> buckets;
    std::unordered_map<int, long> weight_map;

    long bucket_weight = 0, max_weight = -1, total = 0;
    int bucket_idx = 1, current_vertex = 0, min_bucket = 0;
    double efficiency = 0.0, imbalance = 0.0, max_efficiency = -1;

    for(const auto& [vertex, weight]: vertices)
    {
        if (visited.find(vertex) == visited.end()) 
        {
            //mark that the vertex has been visited
            visited[vertex] = true;
            bfs_queue.push(vertex);

            current_vertex = vertex;

            //use bfs to traverse and portion weights
            while (!bfs_queue.empty()) 
            {
                current_vertex = bfs_queue.front();
                bfs_queue.pop();
                std::cout << "Visited: " << current_vertex << "\n";

                //find neighbors of currentVertex
                for (const auto& edge: edges[current_vertex])
                {
                    if (!visited[edge.vertex_2])
                    {
                        visited[edge.vertex_2] = true;
                        bfs_queue.push(edge.vertex_2);
                    }
                }

                if (bucket_weight < avg_load)
                {
                    //update weight map
                    bucket_weight += vertices[current_vertex].weight;
                    weight_map[bucket_idx] = bucket_weight;

                    //update bucket with vertex, and then edge gain
                    buckets[bucket_idx].first.push_back(current_vertex);        
                    buckets[bucket_idx].second = -1;
                } 
                
                else if (bucket_idx < ranks)
                {
                    bucket_weight = vertices[current_vertex].weight;
                    bucket_idx++;
                    weight_map[bucket_idx] = bucket_weight;

                    buckets[bucket_idx].first.push_back(current_vertex);
                    buckets[bucket_idx].second = -1;
                }
                else 
                {
                    bucket_weight += vertices[current_vertex].weight;
                    weight_map[bucket_idx] = bucket_weight;

                    //update bucket with vertex, and then edge gain
                    buckets[bucket_idx].first.push_back(current_vertex);        
                    buckets[bucket_idx].second = -1;

                }

                max_weight = std::max(bucket_weight,max_weight);

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


//print
void Graph::printBuckets(const std::unordered_map<int, std::pair<std::vector<Vertex>,long>>& buckets,
     std::unordered_map<int, long>& weight_map, double efficiency, double imbalance) const
{

    std::cout << "=== Initial Load Balance ===\n\n";

    for (const auto& [bucket_idx, total_weight] : weight_map) 
    {
        std::cout << "Bucket " << bucket_idx << " (Total Weight: " << total_weight << "): \n\n";

    }

    
    for (const auto& [bucket_idx, data] : buckets) 
    {
        std::cout << "Bucket " << bucket_idx << " : " ;
        for (const auto& vertex : data.first) 
        {
            std::cout << vertex.vertex_id << " ";
        }
        std::cout << "\n\n";
    }


    std::cout << "=== Efficiency Calculation ===\n";
    std::cout << "Load Balance Efficiency: " <<  efficiency << "\n"; 
    std::cout << "Load Imbalance Ratio: " << imbalance << "\n\n";

 
}

// adjust partition based on edge weights


