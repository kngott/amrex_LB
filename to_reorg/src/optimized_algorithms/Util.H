#include <AMReX_INT.H>
#include <AMReX_REAL.H>
#include <type_traits>

std::vector<std::size_t>
get_memory(std::vector<amrex::Real> wgts)
{
    std::vector<std::size_t> memory(wgts.size());
    for (long unsigned int i = 0; i<wgts.size(); ++i) {
        memory[i] = static_cast<std::size_t>(wgts[i]) * sizeof(double);
    }
    return memory;
}
