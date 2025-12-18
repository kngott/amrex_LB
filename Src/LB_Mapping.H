#ifndef LB_MAPPING
#define LB_MAPPING

#include <LB_TypeTraits.H>

namespace LB {

    // TODO: Add lambdas as idxs / keys?
    // Add Sorting to options?
    //   Would need idxs & keys, forward & reverse. Pair of 3-item enums.

    struct MappingOpts{
        bool verbose = false;
    };

    // Expected internal functions.
    // ============================

    std::vector<int> Mapping (const int& idxs, const int& keys,
                              const MappingOpts& opts = MappingOpts());

    // --------------

    template <class T>
    std::vector<int> Mapping (const WeightedVector<T>& idxs, const int& keys,
                              const MappingOpts& opts = MappingOpts())
    {
        size_t nidxs = idxs.size();
        std::vector<int> map(nidxs);
        for (int i=0; i<nidxs; ++i) {
           map[idxs[i].index()] = (i%keys); 
        }
        return map;
    }

    template <class T>
    std::vector<int> Mapping (const std::vector<T>& idxs, const int& keys,
                              const MappingOpts& opts = MappingOpts())
    {
        size_t nidxs = idxs.size();
        std::vector<int> map(nidxs);
        for (int i=0; i<nidxs; ++i) {
           map[idxs[i]] = (i%keys); 
        }
        return map;
    }

    template <class U>
    std::vector<int> Mapping (const int& idxs, const WeightedVector<U>& keys,
                              const MappingOpts& opts = MappingOpts())
    {
        size_t nkeys = keys.size();
        std::vector<int> map(idxs);
        for (int i=0; i<idxs; ++i) {
           map[i] = keys[i%nkeys].index();
        }
        return map;
    }

    template <class U>
    std::vector<int> Mapping (const int& idxs, const std::vector<U>& keys,
                              const MappingOpts& opts = MappingOpts())
    {
        size_t nkeys = keys.size();
        std::vector<int> map(idxs);
        for (int i=0; i<idxs; ++i) {
           map[i] = keys[i%nkeys];
        }
        return map;
    }

    // --------------

    template <class T, class U>
    std::vector<int> Mapping (const WeightedVector<T>& idxs, const WeightedVector<U>& keys,
                              const MappingOpts& opts = MappingOpts())
    {
        size_t nidxs = idxs.size();
        size_t nkeys = keys.size();
        std::vector<int> map(nidxs);
        for (int i=0; i<nidxs; ++i) {
           map[idxs[i].index()] = keys[i%nkeys].index();
        }
        return map;
    }

    template <class T, class U>
    std::vector<int> Mapping (const WeightedVector<T>& idxs, const std::vector<U>& keys,
                              const MappingOpts& opts = MappingOpts())
    {
        size_t nidxs = idxs.size();
        size_t nkeys = keys.size();
        std::vector<int> map(nidxs);
        for (int i=0; i<nidxs; ++i) {
           map[idxs[i].index()] = keys[i%nkeys];
        }
        return map;
    }

    template <class T, class U>
    std::vector<int> Mapping (const std::vector<T>& idxs, const WeightedVector<U>& keys,
                              const MappingOpts& opts = MappingOpts())
    {
        size_t nidxs = idxs.size();
        size_t nkeys = keys.size();
        std::vector<int> map(nidxs);
        for (int i=0; i<nidxs; ++i) {
           map[idxs[i]] = keys[i%nkeys].index();
        }
        return map;
    }


    template <class T, class U>
    std::vector<int> Mapping (const std::vector<T>& idxs, const std::vector<U>& keys,
                              const MappingOpts& opts = MappingOpts())
    {
        size_t nidxs = idxs.size();
        size_t nkeys = keys.size();
        std::vector<int> map(nidxs);
        for (int i=0; i<nidxs; ++i) {
           map[idxs[i]] = keys[i%nkeys];
        }
        return map;
    }

    // Expected user-facing.
    // ============================
   
    template <class T, class U>
    std::vector<int> Map (const T& idxs, const U& keys,
                          const MappingOpts& opts = MappingOpts())
    {
        return Mapping(idxs, keys, opts);
    }

    template <class T, class U>
    amrex::DistributionMapping MapToDM (const T& idxs, const U& keys,
                                        const MappingOpts& opts = MappingOpts())
    {
        std::vector<int> map = Mapping(idxs, keys, opts);
        amrex::DistributionMapping dm(map);
        return dm;
    }
}

#endif
