
#include <LB_RoundRobin.H>

namespace LB {

    // AMReX wrappers 
    // ==============
    std::vector<int>
    RoundRobinProcessorMap (const amrex::BoxArray& boxes, int nranks)
    {
        BL_ASSERT(!boxes.empty());

        LongWeightV pts;
        for (int i=0; i < boxes.size(); ++i) {
            pts.emplace_back(i, boxes[i].numPts());
        }
        Sort(pts, SortOrder::Descend);

        std::vector<int> map;
        map = Map(pts, nranks);

        return map;
    }

    std::vector<int> 
    RoundRobinProcessorMap (int nboxes, int nranks, bool sort)
    {
        BL_ASSERT(nboxes > 0);

        std::vector<int> map; 

        if (sort) {
            LongWeightV mem(FAB_mem());
            Sort(mem, SortOrder::Ascend);
            map = Map(nboxes, mem);
        }
        else {
            map = Map(nboxes, nranks);
        }

        return map;
    }

}
