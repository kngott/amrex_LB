#ifndef LB_SORT
#define LB_SORT

namespace LB {

    // Generic Sort function for a vector of items
    // Should work for any types with operators defined.
    // =============================================

    enum class SortOrder : char {Ascend, Descend};

    template <class T>
    void Sort (std::vector<T>& vec, const SortOrder& order = SortOrder::Descend)
    {
        if (vec.size() > 1)
        {
            if (order == SortOrder::Ascend) {
                std::stable_sort(vec.begin(), vec.end());
            }
            else {
                std::stable_sort(vec.begin(), vec.end(),
                                 [](const T& a, const T& b)
                                    { return a > b; });
            }
        }
    }

    // If operators (< and >) are set for weight, should be equivalent of Sort.
    // Can be used when implementing more complex types.
    template <class T>
    void SortbyWeight (std::vector<T>& vec, const SortOrder& order = SortOrder::Descend)
    {
        if (vec.size() > 1)
        {
            if (order == SortOrder::Ascend) {
                std::stable_sort(vec.begin(), vec.end(),
                                 [](const T& a, const T& b)
                                    { return a.weight() < b.weight(); });
            }
            else {
                std::stable_sort(vec.begin(), vec.end(),
                                 [](const T& a, const T& b)
                                    { return a.weight() > b.weight(); });
            }
        }
    }

    template <class T>
    void SortbyIndex (std::vector<T>& vec, const SortOrder& order = SortOrder::Descend)
    {
        if (vec.size() > 1)
        {
            if (order == SortOrder::Ascend) {
                std::stable_sort(vec.begin(), vec.end(),
                                 [](const T& a, const T& b)
                                    { return a.index() < b.index(); });
            }
            else {
                std::stable_sort(vec.begin(), vec.end(),
                                 [](const T& a, const T& b)
                                    { return a.index() > b.index(); });
            }
        }
    }

}
#endif
