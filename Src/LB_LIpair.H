#ifndef LB_LIPAIR
#define LB_LIPAIR

#include <AMReX_INT.H>
#include <AMReX_REAL.H>
#include <AMReX_Vector.H>

// TODO: Add operator << for verbose printing?

namespace LB {

    // LIpair
    // Simple indexed Long for sorting
    // Use locally for simple sortings
    // ==============================

    using LIpair = std::pair<amrex::Long,int>;

    bool operator< (const LIpair& lhs,
                    const LIpair& rhs)
    {
        return lhs.first < rhs.first; 
    };

    bool operator> (const LIpair& lhs,
                    const LIpair& rhs)
    {
        return lhs.first > rhs.first; 
    };

}
#endif
