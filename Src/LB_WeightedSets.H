#ifndef LB_WEIGHTEDSETS
#define LB_WEIGHTEDSETS

#include <AMReX_INT.H>
#include <AMReX_REAL.H>
#include <AMReX_Vector.H>
#include <LB_WeightedIndex.H>

// TODO: Add operator << for verbose printing?

namespace LB {

    // Possibly add:
    // Various other "sum" functions? Take a lambda?
    //    or
    // Template m_total uniquely? Allow more complex data collections?

    template<class T>
    struct WeightedSubset
        : public WeightedVector<T>
    {
        T m_total = 0L;

        [[nodiscard]] T total () const
        {
            return m_total;
        }

        void addWeight (const T& dw) { m_total += dw; }
        void removeWeight (const T& dw) { m_total -= dw; }

        void add (const WeightedIndex<T>& wi)
        {
            m_total += wi.weight();
            this->push_back(wi);
        }

        void remove (typename WeightedVector<T>::iterator& it)
        {
            m_total -= it->total();
            this->erase(it);
        }

        bool operator < (const WeightedSubset& rhs) const
        {
            return total() < rhs.total();
        }

        bool operator > (const WeightedSubset& rhs) const
        {
            return total() > rhs.total();
        }
    };

    // Currently, no safeguards for weight != std::is_arithmetic
    // Add if needed (e.g. more complex weights are added w/o += & -=).
    // Although, addition of that will likely alter this object anyway. :)
    using LongWeightSets = std::vector<WeightedSubset<amrex::Long>>;

}

#endif
