#ifndef LB_TYPETRAITS
#define LB_TYPETRAITS

#include <type_traits>
#include <AMReX_TypeTraits.H>
#include <vector>
#include <AMReX_Vector.H>
#include <LB_WeightedIndex.H>

namespace LB {
/*
template <typename T> constexpr bool IsStdVectorBase_v = amrex::IsStdVector<T>::value;

template <typename V> struct Is_stdvector : std::false_type {};
template <typename T, typename A> struct Is_stdvector< std::vector<T,A> > : std::true_type {};
template <typename V> inline constexpr bool Is_stdvector_v = Is_stdvector<V>::value;

template <typename V> struct Is_amrexVector : std::false_type {};
template <typename T, typename A> struct Is_amrexVector< amrex::Vector<T,A> > : std::true_type {};
template <typename V> inline constexpr bool Is_amrexVector_v = Is_amrexVector<V>::value;

template <typename V> struct Is_WeightedVector : std::false_type {};
template <typename T> struct Is_WeightedVector< LB::WeightedVector<T> > : std::true_type {};
template <typename V> inline constexpr bool Is_WeightedVector_v = Is_WeightedVector<V>::value;
*/
}

#endif
