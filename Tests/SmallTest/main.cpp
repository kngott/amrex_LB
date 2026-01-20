#include <AMReX.H>
#include <AMReX_Random.H>
#include <AMReX_MultiFab.H>
#include <AMReX_ParmParse.H>
#include <AMReX_ParallelDescriptor.H>

#include <LB_Util.H>
#include <LB_Sort.H>
#include <LB_WeightedIndex.H>
#include <LB_WeightedSets.H>

//#include <Knapsack.H>
//#include <SFC.H>
//#include <SFC_knapsack.H>
//#include <painterPartition.H>
//#include <KK.h>

#if defined(AMREX_USE_MPI) || defined(AMREX_USE_GPU)
#error This is a serial test only.
#endif

using namespace amrex;
using namespace LB;

void main_main();

template<class T>
void print_vector(std::string label, std::vector<T> vec, std::string sep = ' ') {

    amrex::Print() << label;
    for (int i = 0; i<vec.size(); ++i)
    {
        amrex::Print() << vec[i] << sep;
    }
    amrex::Print() << std::endl;
}

// Ensures Finalize isn't ran before all 
// variables are cleaned up.
int main(int argc, char* argv[]) {
    amrex::Initialize(argc, argv);

    main_main();

    amrex::Finalize();
}

void main_main() {

    BL_PROFILE("main");

    std::srand(std::time({}));
    Long random_seed = rand();

    amrex::InitRandom(random_seed, 1, 0);
    amrex::ResetRandomSeed(random_seed);

    // WeightedBox
    // ========================
    // ========================
    LB::LongWeightV wbv;

    amrex::Print() << " Unsorted: " << std::endl;
    for (int i=0; i<20; ++i) {
        wbv.push_back(LongWeight(i, amrex::RandomNormal(1000, 100)));
        amrex::Print() << "(" << i << ": " << wbv[i].weight() << "), ";
    }
    amrex::Print() << std::endl;

    Sort(wbv, LB::SortOrder::Ascend);

    amrex::Print() << " Sorted ascending: " << std::endl;
    for (int i=0; i<20; ++i) {
        amrex::Print() << "(" << i << ": " << wbv[i].weight() << "), ";
    }
    amrex::Print() << std::endl;

    Sort(wbv, LB::SortOrder::Descend);

    amrex::Print() << " Sorted descending: " << std::endl;
    for (int i=0; i<20; ++i) {
        amrex::Print() << "(" << i << ": " << wbv[i].weight() << "), ";
    }
    amrex::Print() << std::endl;

    // WeightedSubsets
    // ========================
    // ========================
    LongWeightSets lws(3);

    for (int i=0; i<20; ++i) {
        int r = amrex::Random_int(3);
        LongWeight wb(i, amrex::RandomNormal(1000, 100));
        lws[r].add(wb);
    }

    amrex::Print() << " Unsorted: " << std::endl;
    for (int i=0; i<lws.size(); ++i) {
        amrex::Print() << "(" << i << ": " << lws[i].total() << "), ";
    }
    amrex::Print() << std::endl;

    Sort(lws, LB::SortOrder::Ascend);

    amrex::Print() << " Sorted ascending: " << std::endl;
    for (int i=0; i<lws.size(); ++i) {
        amrex::Print() << "(" << i << ": " << lws[i].total() << "), ";
    }
    amrex::Print() << std::endl;

    Sort(lws, LB::SortOrder::Descend);

    amrex::Print() << " Sorted descending: " << std::endl;
    for (int i=0; i<lws.size(); ++i) {
        amrex::Print() << "(" << i << ": " << lws[i].total() << "), ";
    }
    amrex::Print() << std::endl;

}
