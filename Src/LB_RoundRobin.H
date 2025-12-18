#ifndef LB_ROUNDROBIN
#define LB_ROUNDROBIN

#include <AMReX_BoxArray.H>
#include <AMReX_DistributionMapping.H>
#include <LB_WeightedIndex.H>
#include <LB_Util.H>
#include <LB_WeightedIndex.H>
#include <LB_Sort.H>
#include <LB_Mapping.H>

namespace LB {

    // Possibilities. Not yet used.
    struct RoundRobinOpts {
        int nprocs = -1;
        bool sort_wgt = false;
        bool sort_rank = false;
        bool verbose = false;
    };

    // Remake w/ Opts?
    // e.g. Sort the wgts?
    // (Most likely, leave until use case found.)
    template<class T>
    std::vector<int> RoundRobinProcessorMap (WeightedVector<T>& wgts, int nranks, bool sort=true)
    {
        BL_ASSERT(!wgts.empty());

        std::vector<int> map;
        int nboxes = wgts.size();

        Sort(wgts, SortOrder::Descend);

        if (sort) {
            LongWeightV mem(FAB_mem());
            Sort(mem, SortOrder::Ascend);
            map = Map(wgts, mem);
        }
        else {
            map = Map(wgts, nranks);
        }

        return map;
    }

    // AMReX wrappers.
    std::vector<int> RoundRobinProcessorMap (const amrex::BoxArray& boxes, int nranks);
    std::vector<int> RoundRobinProcessorMap (int nboxes, int nranks, bool sort=true);

    // Expanded from AMReX wrapper. 
    template<class T>
    std::vector<int> RoundRobinProcessorMap (const std::vector<T>& wgts, int nranks, bool sort=true)
    {
        BL_ASSERT(!wgts.empty());
        WeightedVector<T> wv(wgts);

        return RoundRobinProcessorMap(wv, nranks, sort);
    }

    // RoundRobin = std::vector<int>?
    // RoundRobinProcessorMap = return DM?

    // TODO: Make a more generic implementation? 
}

#endif
