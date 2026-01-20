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
        int m_index = -1;

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
        int m_index = -1;

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

        [[nodiscard]] int& index() {
            return m_index;
        }

        // Would've preferred, e.g. vec[n, LB::Index] or vec[n, LB::Wgt].
        // Seems like it requires user-side additions.
        // Change if possible.
        [[nodiscard]] int& index(int i) {
            return (*this)[i].index();
        }

        [[nodiscard]] T& weight(int i) {
            return (*this)[i].weight();
        }

        void set_weight (std::size_t n, const T& wgt) {
            this->resize(n);
            for (int i=0; i<n; ++i) {
                index(i) = i;
                weight(i) = wgt;
            }
        }

        void set_weight (const std::vector<T>& wgt) {
            this->resize(wgt.size());
            for (std::size_t i=0; i<wgt.size(); ++i) {
                index(i) = i;
                weight(i) = wgt[i];
            }
        }

        [[nodiscard]] std::vector<int> to_vec () const {
            std::vector<int> map (this->size());
            for(int i=0; i<this->size(); ++i) {
                map[i] = index(i);
            }
            return map;
        }

        // Check if full answer is here?
        // DM expects [0, ranks) all exist
        // Change to hand-placing each present item, otherwise -1?
        [[nodiscard]] amrex::DistributionMapping to_dm () const {
            std::vector<int> map = to_vec();
            amrex::DistributionMapping dm(map);
            return dm;
        }

    };

    // Put useful templates here:
    using LongWeightV = WeightedVector<amrex::Long>;
    using IntWeightV = WeightedVector<int>;
    using DoubleWeightV = WeightedVector<double>;

}

#endif
