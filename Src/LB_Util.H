#ifndef LB_UTIL
#define LB_UTIL

#include <type_traits>
#include <AMReX_INT.H>
#include <AMReX_REAL.H>
#include <AMReX_ParallelContext.H>
#include <AMReX_BaseFab.H>

namespace LB {

    std::vector<amrex::Long> FAB_mem();

    template <class T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    std::vector<amrex::Long>
    scale_wgts(std::vector<T> wgts)
    {
    
        double wmax = *std::max_element(wgts.begin(), wgts.end());
        double scale = (wmax == 0) ? double(1e9) : double(1.e9)/wmax;
        std::vector<amrex::Long> swgts(wgts.size());
        for (long unsigned int i = 0; i<wgts.size(); ++i) {
            swgts[i] = amrex::Long(wgts[i]*scale) + 1L;
        }
    
        return swgts;
    }

    template<class T>
    void print_vector(std::string label, std::vector<T> vec, std::string sep = " ") {

        amrex::Print() << label;
        for (int i = 0; i<vec.size(); ++i)
        {
            if (i != 0) { amrex::Print() << sep; }
                amrex::Print() << vec[i];
        }
        amrex::Print() << std::endl;
    }

/*
    std::vector<int> & std::vector<int>
    WeightedVector< > & WeightedVector< >
    + other combinations.

    if std::vector<int>, func returns vec[]
    if WeightedVector, func returns vec[].index() or vec.weight() 
*/

    template<class W>
    double efficiency(const std::vector<int>& map, const std::vector<W>& wgt, int nranks)
    {
        double max = 0, total = 0;
        std::vector<double> sum(nranks, 0);
        for (int i=0; i<map.size(); ++i) {
            sum[map[i]] += wgt[i];
            total += wgt[i];
            if (sum[map[i]] > max) {
                max = sum[map[i]];
            }
        }

        double eff = (total) / (double(nranks) * max);
        return eff;
    }
}
#endif
