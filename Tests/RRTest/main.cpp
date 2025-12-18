
#include <AMReX.H>
#include <AMReX_Random.H>
#include <AMReX_MultiFab.H>
#include <AMReX_ParmParse.H>
#include <AMReX_ParallelContext.H>

#include <LB_Util.H>
#include <LB_RoundRobin.H>
//#include <Knapsack.H>
//#include <SFC.H>
//#include <SFC_knapsack.H>
//#include <painterPartition.H>
//#include <KK.h>

using namespace amrex;
void main_main();

// Ensures Finalize isn't ran before all 
// variables are cleaned up.
int main(int argc, char* argv[]) {
    amrex::Initialize(argc, argv);

    main_main();

    amrex::Finalize();
}

void main_main() {

    BL_PROFILE("main");

    // ==== Read Data from Input File ====

    int nranks = -1, ranks_per_node = 1, nnodes = -1;
    int nboxes = -1, nruns = 1;
    Real mean, stdev;
    bool verbose = false, print_wgts = false;
    bool print_swgts = true, print_dmap = true;
    int random_seed = -1;
    IntVect d_size(0), mgs(0);

    {
        ParmParse pp;
        pp.query("domain", d_size);
        pp.query("max_grid_size", mgs);
        pp.query("nboxes", nboxes);

        pp.query("nranks", nranks);
        pp.query("nnodes", nnodes);
        pp.query("ranks_per_node", ranks_per_node);

        pp.get("mean", mean);
        pp.get("stdev", stdev);

        pp.query("random_seed", random_seed);
        pp.query("nruns", nruns);
        pp.query("verbose", verbose);
        pp.query("print_weights", print_wgts);
        pp.query("print_scaled_weights", print_swgts);
        pp.query("print_distribution_map", print_dmap);
    }

    // Could add some assertions to input values, but this is a simple test.
    // Add here if desired.

    // ==== Setup ====

    // Generate a random seed or apply your own
    if (random_seed == -1) {
        std::srand(std::time({}));
        random_seed = rand();
    }

    amrex::InitRandom(random_seed, 1, 0);
    amrex::ResetRandomSeed(random_seed);

    // Build a BoxArray, if needed for SFC
    // BoxArray takes precedent in setup.
    BoxArray ba;

    if ((d_size != IntVect::TheZeroVector())
        && (mgs != IntVect::TheZeroVector())) {

        Box domain(IntVect{0}, (d_size -= 1));
        ba.define(domain);
        ba.maxSize(mgs);

        nboxes = ba.size();
    }
    else if (nboxes <= 0) {
        amrex::Abort("Neither BoxArray (domain & max_grid_size), nor nranks are provided.");
    }

    DistributionMapping dm(ba, nranks);

    int ncomp=1, ngrow=0;
    MultiFab mf(ba, dm, ncomp, ngrow);
    mf.setVal(3.14159);

    // Get resulting mem (box size, here) & print
    std::vector<amrex::Long> mem;
    mem = LB::FAB_mem();
    LB::print_vector("MEM: ", mem);

    std::vector<amrex::Real> wgts(nboxes);
    // ==== Random Weight Generation ====

    for (int i = 0; i < nboxes; ++i) {
        wgts[i] = amrex::RandomNormal(mean, stdev);
    }
    std::vector<Long> scaled_wgts = LB::scale_wgts(wgts);
    LB::print_vector(" Wgts: "       , wgts       , ", ");
    LB::print_vector(" Scaled Wgts: ", scaled_wgts, ", ");

    LB::LongWeightV indexed_wgts(scaled_wgts);

    double time_start = 0.0, time_finish = 0.0;

    // Try each of the RR results & print.
    // Round Robin - nboxes & nranks
    time_start = amrex::second();
    std::vector<int> rr_map = LB::RoundRobinProcessorMap (nboxes, nranks, false);
    time_finish = amrex::second();
    amrex::Print() << "RR(nb,nr) time: " << time_finish - time_start << std::endl;
    LB::print_vector(" Result: ", rr_map, " ");

    // Round Robin - Box Array & nranks
    time_start = amrex::second();
    std::vector<int> rrba_map = LB::RoundRobinProcessorMap (ba, nranks);
    time_finish = amrex::second();
    amrex::Print() << "RR(ba,nr) time: " << time_finish - time_start << std::endl;
    LB::print_vector(" Result: ", rrba_map, " ");

    // Round Robin - wgt & nranks
    time_start = amrex::second();
    std::vector<int> rrwgt_map = LB::RoundRobinProcessorMap (scaled_wgts, nranks, false);
    time_finish = amrex::second();
    amrex::Print() << "RR(wgt,nr) time: " << time_finish - time_start << std::endl;
    LB::print_vector(" Result: ", rrwgt_map, " ");

}
