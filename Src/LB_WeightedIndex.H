#ifndef LB_WEIGHTEDTYPES
#define LB_WEIGHTEDTYPES

#include <AMReX_INT.H>
#include <AMReX_REAL.H>
#include <AMReX_DistributionMapping.H>

// TODO: Add operator << for verbose printing?
//       Add + & -, etc. for summing?

namespace LB {

    // Templated WeightedIndex
    // Allows user-defined complex
    // types + operators for sorting.
    // ==============================

    template<class T>
    struct WeightedIndex {
        T m_weight;
        int m_index;

        WeightedIndex<T> () = default;    
        WeightedIndex<T> (int i, T w) : m_index(i), m_weight(w) {}
        [[nodiscard]] T weight () const { return m_weight; }
        [[nodiscard]] int index () const { return m_index; }
        [[nodiscard]] T& weight ()  { return m_weight; }
        [[nodiscard]] int& index () { return m_index;  }
 
        bool operator< (const WeightedIndex<T>& rhs) const
        {
            return weight() < rhs.weight();
        }
    
        bool operator> (const WeightedIndex<T>& rhs) const
        {
            return weight() > rhs.weight();
        }
    };

    // Put useful templates here:
    using LongWeight = WeightedIndex<amrex::Long>;
    using IntWeight = WeightedIndex<int>;
    using DoubleWeight = WeightedIndex<double>;



    template<class T>
    struct WeightedVector
        : public std::vector<WeightedIndex<T>>
    {
        WeightedVector<T> () = default;

        WeightedVector<T> (std::size_t n) {
            this->resize(n); 
        }

        WeightedVector<T> (std::size_t n, const T& wgt) {
            set_weight(n, wgt);
        }

        WeightedVector<T> (const std::vector<T>& wgt) {
            set_weight(wgt);
        }

        void set_weight (std::size_t n, const T& wgt) {
            this->resize(n);
            for (int i=0; i<n; ++i) {
                (*this)[i].index() = i;
                (*this)[i].weight() = wgt;
            }
        }

        void set_weight (const std::vector<T>& wgt) {
            this->resize(wgt.size());
            for (std::size_t i=0; i<wgt.size(); ++i) {
                (*this)[i].index() = i;
                (*this)[i].weight() = wgt[i];
            }
        }

        std::vector<int> to_vec () const {
            std::vector<int> map (this->size());
            for(int i=0; i<this->size(); ++i) {
                map[i] = (*this)[i].index();
            }
            return map;
        }

        amrex::DistributionMapping to_dm () const {
            std::vector<int> map = to_vec();
            amrex::DistributionMapping dm(map);
            return dm;
        }

    };

    // Put useful templates here:
    using LongWeightV = WeightedVector<amrex::Long>;
    using IntWeightV = WeightedVector<int>;
    using DoubleWeightV = WeightedVector<double>;

/*
//  Taken from amrex_TypeTraits -- not fully tested.
    template <class D>
    struct IsWeightedIndex<D, std::enable_if_t<std::is_base_of_v<WeightedIndex,D>>>
        : std::true_type {};

    template <class A>
    inline constexpr bool IsWeightedIndex_v = IsWeightedIndex<A>::value;
*/
}

#endif
