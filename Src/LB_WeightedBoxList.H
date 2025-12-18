#ifndef LB_WEIGHTEDBOXLIST
#define LB_WEIGHTEDBOXLIST

#include <AMReX_INT.H>
#include <AMReX_REAL.H>
#include <AMReX_Vector.H>

// TODO: Add operator << for verbose printing?

namespace LB {

    // WeightedBoxList
    // Possibly template this in the future?
    // e.g. T = long mem & long wgt
    //      Vector<T> & T w/ the sum of multiple items? 
    //      Work with T <, >, + and - defined?
    // ==============================

    struct WeightedBoxList
    {
        amrex::Vector<LongWeight> m_lb;
        amrex::Long               m_weight = 0L;
        int                       m_rank   = -1;

        [[nodiscard]] amrex::Long weight () const
        {
            return m_weight;
        }

        [[nodiscard]] int rank () const { return m_rank; }
        void addWeight (amrex::Long dw) { m_weight += dw; }

        void erase (amrex::Vector<LongWeight>::iterator& it)
        {
            m_weight -= it->weight();
            m_lb.erase(it);
        }

        void push_back (const LongWeight& bx)
        {
            m_weight += bx.weight();
            m_lb.push_back(bx);
        }

        [[nodiscard]] int size () const { return static_cast<int>(m_lb.size()); }
        [[nodiscard]] amrex::Vector<LongWeight>::const_iterator begin () const { return m_lb.begin(); }
        [[nodiscard]] amrex::Vector<LongWeight>::iterator       begin ()       { return m_lb.begin(); } // NOLINT(readability-make-member-function-const)
        [[nodiscard]] amrex::Vector<LongWeight>::const_iterator end   () const { return m_lb.end();   }
        [[nodiscard]] amrex::Vector<LongWeight>::iterator       end   ()       { return m_lb.end();   } // NOLINT(readability-make-member-function-const)
    
        bool operator < (const WeightedBoxList& rhs) const
        {
            return weight() < rhs.weight();
        }

        bool operator > (const WeightedBoxList& rhs) const
        {
            return weight() > rhs.weight();
        }
  
    };
}
#endif
