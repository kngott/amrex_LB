
#include <LB_Util.H>

namespace LB {

    // TODO: Maybe add a variable to change units.
    // TODO: Add sort? Is this ever not sorted?
    // TODO: AllGather vs. send to rank doing calc?

    std::vector<amrex::Long> FAB_mem()
    {
        std::vector<amrex::Long> bytes(amrex::ParallelContext::NProcsSub());
        amrex::Long thisbyte = amrex::TotalBytesAllocatedInFabs()/1024;
        amrex::ParallelAllGather::AllGather(thisbyte, bytes.data(),
                                            amrex::ParallelContext::CommunicatorSub());
        return bytes;
    }

}
