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


int Graph_LB::calculateGain(const Vertex& vertex, int bucket_idx)
{
    int internal = 0, external = 0, gain = 0;

    //find the gain of the vertex in the current bucket
    for (const auto& edge : G.edges.at(vertex.vertex_id)) 
    {
        std::cout << "bucket: " << bucket_idx << " vertex " << vertex.vertex_id << "\n";

        auto it = std::find_if(buckets[bucket_idx].begin(),buckets[bucket_idx].end(),
        [&](const Vertex& v)
        {
            return v.vertex_id == edge.vertex_2;
        });

        if (it != buckets[bucket_idx].end())
        {
            internal++;
           // std::cout <<"Internal gain: " << internal << "\n";
           // std::cout << "internal vertex: " << edge.vertex_2 << "\n";
        }
        else 
        {
            external++;
           // std::cout <<"External gain: " << external << "\n";
           // std::cout << "external vertex: " << edge.vertex_2 << "\n";
        }   
        gain = external - internal;
    }
         
    std::cout << "Vertex:" << vertex.vertex_id << "-->" << " Edge Gain: " << gain << "\n";
    return (gain);
}


gainVertexData Graph_LB::checkVertexGain(const Vertex& vertex_move, int move_bucket_idx, int vertex_move_gain)
{
    Vertex new_vertex_move;
    int move_gain;
  
    //check for vertex with highest gain in the bucket with the maximum weight
    
    move_gain = calculateGain(vertex_move, move_bucket_idx);
                
    if(vertex_move_gain == -9999) //gain is calculated for the first time
    {
        vertex_move_gain = move_gain;
        new_vertex_move = vertex_move;
        gain_map[vertex_move_gain].push(new_vertex_move);
    }
    else if (move_gain > vertex_move_gain) 
    {
        vertex_move_gain = move_gain;
        new_vertex_move = vertex_move;
        gain_map[vertex_move_gain].push(new_vertex_move);
    } 
    else if (move_gain == vertex_move_gain) 
    {
        if (vertex_move.weight > new_vertex_move.weight) //move larger weights 
        {
            new_vertex_move = vertex_move;
            gain_map[vertex_move_gain].push(new_vertex_move); // make a vector of gain maps 
        }
        else
        {
            gain_map[vertex_move_gain].push(new_vertex_move);
        }
    }

    std::cout << "vertex to move: " << new_vertex_move.vertex_id << " & " << new_vertex_move.weight << "\n\n";

    return {new_vertex_move,vertex_move_gain};
}

std::pair<int, int> Graph_LB::checkBucketGain(const Vertex& vertex_move,int move_bucket_idx)
{
    
    // Check all the buckets to find bucket with the best gain to move the vertex to
   int  bucket_receive_id = -1, bucket_receive_gain = 100000, receive_gain=0;

    for (int bkt_id = 0; bkt_id < ranks; ++bkt_id) 
    {

        if (bkt_id != move_bucket_idx) 
        { 
            // std::cout << move_bucket_idx << "," << bkt_id << "; " << vertex_move.vertex_id << "& \n";
            receive_gain = calculateGain(vertex_move.vertex_id, bkt_id); // Assuming we check the first vertex in the bucket for gain
        }
        else
        {
            continue;
        }

        if (receive_gain < bucket_receive_gain)
        {
            bucket_receive_gain = receive_gain;
            bucket_receive_id = bkt_id;
        }
        
    }

     std::cout << "Best bucket to move to: " << bucket_receive_id <<  " and Recieve Gain: " << bucket_receive_gain << "\n";
     return{bucket_receive_id, bucket_receive_gain};
}

void Graph_LB::moveVertex(const Vertex& vertex_move, int move_bucket_idx, int bucket_recieve_id)
{
    //move
    // Remove vertex from the current bucket
    auto& current_bucket = buckets[move_bucket_idx];
    current_bucket.erase(std::remove_if(current_bucket.begin(), current_bucket.end(),
        [vertex_move](const Vertex& v) { return v.vertex_id == vertex_move.vertex_id; }), 
        current_bucket.end());

    // Add vertex to the best bucket
    buckets[bucket_recieve_id].emplace_back(vertex_move);

    // Update weight maps
    weight_map[move_bucket_idx] -= vertex_move.weight;
    weight_map[bucket_recieve_id] += vertex_move.weight;

    std::cout << "Moved vertex: " << vertex_move.vertex_id << " from bucket: " << move_bucket_idx 
            << " to bucket:" << bucket_recieve_id << " for better gain.\n";
}



//adjust by vertices by gain, and then by weight. Move the vertex with the highest gain to the bucket with the lowest gain.
void Graph_LB::adjustBuckets()
{
    long max_weight = -1;
    int count = 0, move_bucket_idx, move_gain,receive_gain, bucket_receive_id, bucket_receive_gain, vertex_just_moved, vertex_move_gain, bucket_just_received;
    bool found, vertex_not_moved = false;
   
    Vertex to_pop, temp;
    Vertex vertex_move;
    std::vector<Vertex> lock_vertex;
    gainVertexData gain_result;   
    std::pair<int,int> bucket_result;

    //track gains by sorting vertices in asceinding order

    //std::stack<int>  
    

    while (result.second < 0.98)
    {
        
    //always start with the bucket with the maximum weight
        result = Graph_LB::getEfficiency();
        move_bucket_idx = result.first;
        std::cout << "Max Bucket: " << move_bucket_idx << "\n";

        //find the element with highest gain to move. If there is a tie, move the heaviest element

        vertex_move_gain = -9999;

        std::cout << "**********************Print vertices in bucket****************************** \n\n";
        std::cout << "Bucket" << move_bucket_idx << "\n";
        for (const auto& vertex : buckets[move_bucket_idx]) 
        {
            std::cout << vertex.vertex_id << " ";
        }
        std::cout <<"\n";

        //find the vertex with the highest gain to move
        for (const auto& vertex : buckets[move_bucket_idx]) 
        {
            std::cout << vertex.vertex_id << "<------------------------ \n";

            //if the vertex was just moved dont move it again
            if (vertex.vertex_id == vertex_just_moved) 
            {
                std::cout << "Vertex just moved, skipping: " << vertex.vertex_id << "\n";
                continue;   
            }

            // if the vertex was not moved
            else if (vertex_not_moved == true)
            {
                //check if the vertex is in the locked vertex list, if it is, pop it out and check the next vertex in the gain map
                found = std::ranges::any_of(lock_vertex, [&](const auto& lv) {
                return lv.vertex_id == vertex.vertex_id; });

                if (found) 
                {
                    temp = gain_map[vertex_move_gain].front();

                    //check that it is the locked vertex before deleting
                
                    if(temp.vertex_id == vertex.vertex_id)
                    {
                        gain_map[vertex_move_gain].pop();

                    //update the move vertex and calculate gain
                        if(!gain_map[vertex_move_gain].empty())
                        {
                            std::cout <<"************************************************************** \n";
                            std::cout << "Update vertex when moved gain queue is not empty \n";
                            vertex_move = gain_map[vertex_move_gain].front();
                            gain_map[vertex_move_gain].pop();
                            std::cout << "If: " << vertex_move.vertex_id << "\n";
                            gain_result = checkVertexGain(vertex_move,move_bucket_idx,vertex_move_gain);
                        }
                        else 
                        {
                            std::cout <<"************************************************************** \n";
                            std::cout << "Update vertex when moved gain queue is empty \n";
                            //if the gain que is empty, check for the next gain greater than 0
                            for (auto&[gain, vertex_que]: gain_map)
                            {
                                if (gain >= 0 && !vertex_que.empty())
                                {
                                    vertex_move = vertex_que.front();
                                    vertex_que.pop();
                                    std::cout << "else: " << vertex_move.vertex_id << "\n";
                                    break;
                                }
                            }
                            gain_result = checkVertexGain(vertex_move,move_bucket_idx,vertex_move_gain);
                        }
                    }
                   
                }

            }
            //when vertex is not marked as moved i.e false
            else 
            {
                std::cout <<"************************************************************** \n";
                std::cout << "Update vertex when vertex not moved \n";
                gain_result = checkVertexGain(vertex,move_bucket_idx,vertex_move_gain);

                if (gain_result.vertex_gain > vertex_move_gain)
                {
                    vertex_move_gain = gain_result.vertex_gain;
                    vertex_move = gain_result.vertex;
                }
            }

            std::cout << "all vertices checked \n";
        }
        

        //set the bucket to move to and the gain of that bucket
        bucket_result = checkBucketGain(vertex_move,move_bucket_idx);
        bucket_receive_id = bucket_result.first;
        bucket_receive_gain = bucket_result.second;
      

        // Move the vertex to the best bucket if it improves the gain

        // check when vertex is >= 0, and when it is less than 0
        if (vertex_move_gain >= 0)
        {
            // move if bucket_recieve gain is less than vertex_move_gain
            std::cout << "************ Move Vertex >= 0 ************ \n";
            std::cout << "Vertex Move Gain: " << vertex_move_gain << "\n";
            std::cout << "Bucket Recieve Gain: " << bucket_receive_gain << "\n";

             if (bucket_receive_gain < vertex_move_gain)
             {
                // move function
               moveVertex(vertex_move, move_bucket_idx, bucket_receive_id);

               //set the vertex_not_moved to false, and update the vertex_just_moved and bucket_just_received
                vertex_not_moved = false;
                vertex_just_moved = vertex_move.vertex_id;
                bucket_just_received = bucket_receive_id;
            }

            else if (bucket_receive_gain == vertex_move_gain) //if both are equal move only if efficiency improves
            {
                std::cout << "Recieve and Move gain are equal" << bucket_receive_gain << " & " << vertex_move_gain << "\n";
                double current_efficiency = weight_map[move_bucket_idx]/avg_load;
                double recieve_efficiency = (weight_map[bucket_receive_id] + vertex_move.weight)/avg_load;
                if(recieve_efficiency <= current_efficiency)
                {
                    moveVertex(vertex_move, move_bucket_idx, bucket_receive_id);
                    //set the vertex_not_moved to false, and update the vertex_just_moved and bucket_just_received
                    vertex_not_moved = false;
                    vertex_just_moved = vertex_move.vertex_id;
                    bucket_just_received = bucket_receive_id;
                }
            }

            else 
            {
                std::cout << "No beneficial move found for vertex " << vertex_move.vertex_id << "\n";
                vertex_not_moved = true;
                lock_vertex.emplace_back(vertex_move);
            }
            count++;
        }

        else if (vertex_move_gain < 0)
        {
            std::cout << "************ Move Vertex < 0 ************ \n";
            std::cout << "Vertex Move Gain: " << vertex_move_gain << "\n";
            std::cout << "Bucket Recieve Gain: " << bucket_receive_gain << "\n";
        
            if (std::abs(bucket_receive_gain) < std::abs(vertex_move_gain))
            {
                moveVertex(vertex_move, move_bucket_idx, bucket_receive_id);
                //set the vertex_not_moved to false, and update the vertex_just_moved and bucket_just_received
                vertex_not_moved = false;
                vertex_just_moved = vertex_move.vertex_id;
                bucket_just_received = bucket_receive_id;
            }

            else if (std::abs(bucket_receive_gain) == std::abs(vertex_move_gain)) //if both are equal move only if efficiency improves
            {
                std::cout << "Recieve and Move gain are equal" << bucket_receive_gain << " & " << vertex_move_gain << "\n";
                double current_efficiency = weight_map[move_bucket_idx]/avg_load;
                double recieve_efficiency = (weight_map[bucket_receive_id] + vertex_move.weight)/avg_load;
                if(recieve_efficiency <= current_efficiency)
                {
                    moveVertex(vertex_move, move_bucket_idx, bucket_receive_id);
                    //set the vertex_not_moved to false, and update the vertex_just_moved and bucket_just_received
                    vertex_not_moved = false;
                    vertex_just_moved = vertex_move.vertex_id;
                    bucket_just_received = bucket_receive_id;
                }
            }
            else 
            {
                std::cout << "No beneficial move found for vertex " << vertex_move.vertex_id << "\n";
                vertex_not_moved = true;
                lock_vertex.emplace_back(vertex_move);
            }
            count++;
        }
    }
}