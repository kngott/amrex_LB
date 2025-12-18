
#include <LB_Mapping.H>

namespace LB {

    std::vector<int> Mapping (const int& idxs, const int& keys,
                              const MappingOpts& opts)
    {
        std::vector<int> map(idxs);
        for (int i=0; i<idxs; ++i) {
           map[i] = (i%keys);
        }
        return map;
    }

}
