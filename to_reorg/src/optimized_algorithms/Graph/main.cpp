#include <AMReX.H>
#include <AMReX_Random.H>

#include <Util.H>
#include <Knapsack.H>
#include <SFC.H>
#include <SFC_knapsack.H>
#include <painterPartition.H>
#include <KK.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>    
#include <algorithm>
#include <random>
#include <tuple>

#include "graph.H"
#include "graph_loadbalance.H"
#include <random>


void main_main() 
{


    // generate graph and test
    double mean = 100000, stdev = 25000, avg = 0, be = 0;
    int nruns = 10, rank = 4  , rows = 4, cols = 4;
    bool skew = false;
    //double average, max_weight, total;
    std::vector<double> sfc_eff;
    std::vector<double> knpsk_eff;
    std::vector<double> bfs_eff;

  
        std::vector<int> vertex_ids;
        std::vector<long> vertex_weights;
        

        int nboxes = rows * cols;
        vertex_ids.reserve(nboxes);
        vertex_weights.reserve(nboxes);

       
       //test distribution
       std::mt19937 seed(42);
      std::normal_distribution<double> wgt_dist(mean, stdev);

      //random distribution for 10 trials
      // std::random_device rd;
      // std::mt19937 gen(rd());
       //std::normal_distribution<double> wgt_dist(mean, stdev);

        // ==== Random Graph Generation ====
        
        std::cout<<"================Random Undirected Graph Generation==========" << "\n\n" ;
        std::cout<<"Normal Distribution" << "\n" ;
        std::cout<<"Mean: " << mean << "\n" ;    
        std::cout<<"Standard Deviation: " << stdev << "\n" ; 
        std::cout<<"Number of Boxes: " << nboxes << "\n" ;
        
        //get average, total weight

        long total = 0, temp = 0;
        long average = 0;
        
        for (int i = 0; i < nboxes; i++) 
        {  
            //std::cout << i << "\n";
            vertex_ids.push_back(i);
            temp = static_cast<long>(std::lround(wgt_dist(seed)));
            vertex_weights.push_back(temp);
            total += temp;
        }
        
        average = total/rank + (total % rank);
        std::cout<<"Total Weight: " << total << "\n" ;
        std::cout<<"Average Weight: " << average << "\n\n" ;


        Graph g(vertex_ids, vertex_weights);

        //2D boxes
        int box_id = 0;

        for (int r = 0; r < rows; r++)
        {

            for(int c = 0; c < cols; c++)
            {
                box_id = (rows * r) + c;
                
                if (r == c)
                {
                    
                }
                        
                //edge with boxes to the left
                if (c - 1 > -1 )
                {
                    g.addEdge(box_id, box_id - 1, 1.0); //left
                }

                

                //edge with boxes to the right
                if (c + 1 < cols)
                {
                    
                    g.addEdge(box_id, box_id + 1, 1.0);
                }

                
                //edge with box above it
                if (r - 1 > -1 )
                {
                    g.addEdge(box_id, box_id - rows, 1.0);
                }
                

                //edge with box below it
            if (r + 1 < rows)
                {   
                    g.addEdge(box_id, box_id + rows, 1.0);
                }


            }
            
        }

    //3D graph generation
    /*

        std::cout<<"Graph Generation Complete"<< "\n\n";

    std::cout << "=== Graph with Node and Edge Weights ===\n";

        g.print();
    */

        std::cout << "\n\n" <<"=== Start Node Only Load Balance ===== \n";

        Graph_LB graph_lb(g, rank, average);

        std::cout << "Enter 1 for bfs, any number for dfs \n";
        
        int lb_type = 1;

        be = graph_lb.bfs_LoadBalance();
        bfs_eff.push_back(be);
         std::cout << "inside main \n"; 
        std::cout <<"Efficiency: " << be << "\n";
 

        //Test with Knapsack 

        amrex::Real eff = 0.0;
        std::vector<int> k_dmap = KnapSackDoIt(vertex_weights, rank, eff);
        amrex::Print()<<" Knapsack efficiency: " << eff << std::endl;
        
        std::unordered_map<int,long> knp_result;
        knp_result.reserve(rank);

        std::unordered_map<int,int> knp_map;
        knp_map.reserve(rank);

        double k = 0;
        int id = 0;
        for (int i; i < k_dmap.size(); ++i)
        {
            id = k_dmap[i];
            std::cout << "(" << i << "," << id << "," << vertex_weights[i] << ")" << "\n";
            knp_result[id] += vertex_weights[i];
            knp_map[id] = i;
    
        }  

        for (int i; i < knp_result.size(); i++)
        {
            k = average/knp_result[i];

            std::cout << "Box " << i << ": " << knp_result[i] << "\n" ;
        }

        
        knpsk_eff.push_back(eff);
        
        //SFC
        amrex::IntVect d_size(amrex::Dim3{4,4,1});
        amrex::IntVect mgs(amrex::Dim3{1,1,1});

        //create box array
        // BoxArray takes precedent in setup.
        amrex::BoxArray ba;
        
        std::cout <<"dsize: " << d_size << "\n";
        amrex::Box domain(amrex::IntVect{0}, (d_size -= 1));
        ba.define(domain);
        ba.maxSize(mgs);

        std::cout <<"Invect: " << amrex::IntVect{0} << "\n";
        std::cout << "Domain" << domain << "\n";
        std::cout << ba << "\n";
        
        
        int sfc_boxes = ba.size();
        std::cout << "SFC Number of boxes: " << sfc_boxes << "\n";
        if (sfc_boxes != nboxes) {

        std::cout << "------>" << sfc_boxes << " != " << nboxes << std::endl;
    } 

        std::vector<int> s_dmap = SFCProcessorMapDoIt(ba, vertex_weights, rank, &eff);
        amrex::Print()<<" SFC efficiency: " << eff << std::endl; 

        sfc_eff.push_back(eff);

        std::unordered_map<int,long> sfc_result;
        sfc_result.reserve(rank);

        std::unordered_map<int,int> sfc_map;
        sfc_map.reserve(rank);

        double r = 0;
        int idx = 0;
        for (int i; i < s_dmap.size(); ++i)
        {
            idx = s_dmap[i];
            std::cout << "(" << i << "," << idx << "," << vertex_weights[i] << ")" << "\n";
            sfc_result[idx] += vertex_weights[i];
            sfc_map[idx] = i;
    
        }  

        for (int i; i < sfc_result.size(); i++)
        {
            r = average/sfc_result[i];

            std::cout << "Box " << i << ": " << sfc_result[i] << "\n" ;
        }

        
    }
     
 /*
    std::ofstream file("knapsack_std.csv");

    file << "Trial, Knapsack_Efficiency, SFC_Efficiency, BFS_Eficiency" << "\n";

    for (int j = 0; j < nruns; ++j)
    {
        file << j << "," << knpsk_eff[j] << "," << sfc_eff[j] << "," << bfs_eff[j] << "\n";
    }

    file.close();

*/


int main(int argc, char* argv[]) 
{
    amrex::Initialize(argc, argv);

    main_main();

    amrex::Finalize();
}