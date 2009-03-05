/****************************************************************************
**
** This file is part of the $PACKAGE_NAME$.
**
** Copyright (C) $THISYEAR$ $COMPANY_NAME$.
**
** $QT_EXTENDED_DUAL_LICENSE$
**
****************************************************************************/

#ifndef BIND_P_H
#define BIND_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt Extended API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//


// When using GCC 4.1.1 on ARM, TR1 functional cannot be included when RTTI
// is disabled, since it automatically instantiates some code using typeid().

// Provide the small parts of functional we use - binding only to member functions,
// with up to 6 function parameters, and with crefs only to value types.

namespace nonstd {
namespace tr1 {

namespace impl {

template<typename T>
struct ReferenceWrapper
{
    T* m_t;

    ReferenceWrapper(T& t) : m_t(&t) {}

    operator T&() const { return *m_t; }
};

template<typename R, typename F, typename A1>
struct FunctionWrapper1
{
    F m_f; A1 m_a1;

    FunctionWrapper1(F f, A1 a1) : m_f(f), m_a1(a1) {}

    R operator()() { return (m_a1->*m_f)(); }
};

template<typename R, typename F, typename A1, typename E1>
struct FunctionWrapper1e1
{
    F m_f; A1 m_a1;

    FunctionWrapper1e1(F f, A1 a1) : m_f(f), m_a1(a1) {}

    R operator()(E1 e1) { return (m_a1->*m_f)(e1); }
};

template<typename R, typename F, typename A1, typename A2>
struct FunctionWrapper2
{
    F m_f; A1 m_a1; A2 m_a2;

    FunctionWrapper2(F f, A1 a1, A2 a2) : m_f(f), m_a1(a1), m_a2(a2) {}

    R operator()() { return (m_a1->*m_f)(m_a2); }
};

template<typename R, typename F, typename A1, typename A2, typename E1>
struct FunctionWrapper2e1
{
    F m_f; A1 m_a1; A2 m_a2;

    FunctionWrapper2e1(F f, A1 a1, A2 a2) : m_f(f), m_a1(a1), m_a2(a2) {}

    R operator()(E1 e1) { return (m_a1->*m_f)(m_a2, e1); }
};

template<typename R, typename F, typename A1, typename A2, typename A3>
struct FunctionWrapper3
{
    F m_f; A1 m_a1; A2 m_a2; A3 m_a3;

    FunctionWrapper3(F f, A1 a1, A2 a2, A3 a3) : m_f(f), m_a1(a1), m_a2(a2), m_a3(a3) {}

    R operator()() { return (m_a1->*m_f)(m_a2, m_a3); }
};

template<typename R, typename F, typename A1, typename A2, typename A3, typename E1>
struct FunctionWrapper3e1
{
    F m_f; A1 m_a1; A2 m_a2; A3 m_a3;

    FunctionWrapper3e1(F f, A1 a1, A2 a2, A3 a3) : m_f(f), m_a1(a1), m_a2(a2), m_a3(a3) {}

    R operator()(E1 e1) { return (m_a1->*m_f)(m_a2, m_a3, e1); }
};

template<typename R, typename F, typename A1, typename A2, typename A3, typename A4>
struct FunctionWrapper4
{
    F m_f; A1 m_a1; A2 m_a2; A3 m_a3; A4 m_a4;

    FunctionWrapper4(F f, A1 a1, A2 a2, A3 a3, A4 a4) : m_f(f), m_a1(a1), m_a2(a2), m_a3(a3), m_a4(a4) {}

    R operator()() { return (m_a1->*m_f)(m_a2, m_a3, m_a4); }
};

template<typename R, typename F, typename A1, typename A2, typename A3, typename A4, typename E1>
struct FunctionWrapper4e1
{
    F m_f; A1 m_a1; A2 m_a2; A3 m_a3; A4 m_a4;

    FunctionWrapper4e1(F f, A1 a1, A2 a2, A3 a3, A4 a4) : m_f(f), m_a1(a1), m_a2(a2), m_a3(a3), m_a4(a4) {}

    R operator()(E1 e1) { return (m_a1->*m_f)(m_a2, m_a3, m_a4, e1); }
};

template<typename R, typename F, typename A1, typename A2, typename A3, typename A4, typename A5>
struct FunctionWrapper5
{
    F m_f; A1 m_a1; A2 m_a2; A3 m_a3; A4 m_a4; A5 m_a5;

    FunctionWrapper5(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) : m_f(f), m_a1(a1), m_a2(a2), m_a3(a3), m_a4(a4), m_a5(a5) {}

    R operator()() { return (m_a1->*m_f)(m_a2, m_a3, m_a4, m_a5); }
};

template<typename R, typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename E1>
struct FunctionWrapper5e1
{
    F m_f; A1 m_a1; A2 m_a2; A3 m_a3; A4 m_a4; A5 m_a5;

    FunctionWrapper5e1(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) : m_f(f), m_a1(a1), m_a2(a2), m_a3(a3), m_a4(a4), m_a5(a5) {}

    R operator()(E1 e1) { return (m_a1->*m_f)(m_a2, m_a3, m_a4, m_a5, e1); }
};

template<typename R, typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
struct FunctionWrapper6
{
    F m_f; A1 m_a1; A2 m_a2; A3 m_a3; A4 m_a4; A5 m_a5; A6 m_a6;

    FunctionWrapper6(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) : m_f(f), m_a1(a1), m_a2(a2), m_a3(a3), m_a4(a4), m_a5(a5), m_a6(a6) {}

    R operator()() { return (m_a1->*m_f)(m_a2, m_a3, m_a4, m_a5, m_a6); }
};

template<typename R, typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename E1>
struct FunctionWrapper6e1
{
    F m_f; A1 m_a1; A2 m_a2; A3 m_a3; A4 m_a4; A5 m_a5; A6 m_a6;

    FunctionWrapper6e1(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) : m_f(f), m_a1(a1), m_a2(a2), m_a3(a3), m_a4(a4), m_a5(a5), m_a6(a6) {}

    R operator()(E1 e1) { return (m_a1->*m_f)(m_a2, m_a3, m_a4, m_a5, m_a6, e1); }
};

template<typename R, typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
struct FunctionWrapper7
{
    F m_f; A1 m_a1; A2 m_a2; A3 m_a3; A4 m_a4; A5 m_a5; A6 m_a6; A7 m_a7;

    FunctionWrapper7(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) : m_f(f), m_a1(a1), m_a2(a2), m_a3(a3), m_a4(a4), m_a5(a5), m_a6(a6), m_a7(a7) {}

    R operator()() { return (m_a1->*m_f)(m_a2, m_a3, m_a4, m_a5, m_a6, m_a7); }
};

template<typename R, typename F, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename E1>
struct FunctionWrapper7e1
{
    F m_f; A1 m_a1; A2 m_a2; A3 m_a3; A4 m_a4; A5 m_a5; A6 m_a6; A7 m_a7;

    FunctionWrapper7e1(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) : m_f(f), m_a1(a1), m_a2(a2), m_a3(a3), m_a4(a4), m_a5(a5), m_a6(a6), m_a7(a7) {}

    R operator()(E1 e1) { return (m_a1->*m_f)(m_a2, m_a3, m_a4, m_a5, m_a6, m_a7, e1); }
};

} // namespace impl

template<typename T>
impl::ReferenceWrapper<const T> cref(const T& t)
{
    return impl::ReferenceWrapper<const T>(t);
}

template<typename R, typename T, typename A1>
impl::FunctionWrapper1<R, R (T::*)(), A1> bind(R (T::*f)(), A1 a1)
{
    return impl::FunctionWrapper1<R, R (T::*)(), A1>(f, a1);
}

template<typename R, typename T, typename A1>
impl::FunctionWrapper1<R, R (T::*)() const, A1> bind(R (T::*f)() const, A1 a1)
{
    return impl::FunctionWrapper1<R, R (T::*)() const, A1>(f, a1);
}

template<typename R, typename T, typename E1, typename A1>
impl::FunctionWrapper1e1<R, R (T::*)(E1), A1, E1> bind(R (T::*f)(E1), A1 a1)
{
    return impl::FunctionWrapper1e1<R, R (T::*)(E1), A1, E1>(f, a1);
}

template<typename R, typename T, typename E1, typename A1>
impl::FunctionWrapper1e1<R, R (T::*)(E1) const, A1, E1> bind(R (T::*f)(E1) const, A1 a1)
{
    return impl::FunctionWrapper1e1<R, R (T::*)(E1) const, A1, E1>(f, a1);
}

template<typename R, typename T, typename B1, typename A1, typename A2>
impl::FunctionWrapper2<R, R (T::*)(B1), A1, A2> bind(R (T::*f)(B1), A1 a1, A2 a2)
{
    return impl::FunctionWrapper2<R, R (T::*)(B1), A1, A2>(f, a1, a2);
}

template<typename R, typename T, typename B1, typename A1, typename A2>
impl::FunctionWrapper2<R, R (T::*)(B1) const, A1, A2> bind(R (T::*f)(B1) const, A1 a1, A2 a2)
{
    return impl::FunctionWrapper2<R, R (T::*)(B1) const, A1, A2>(f, a1, a2);
}

template<typename R, typename T, typename B1, typename E1, typename A1, typename A2>
impl::FunctionWrapper2e1<R, R (T::*)(B1, E1), A1, A2, E1> bind(R (T::*f)(B1, E1), A1 a1, A2 a2)
{
    return impl::FunctionWrapper2e1<R, R (T::*)(B1, E1), A1, A2, E1>(f, a1, a2);
}

template<typename R, typename T, typename B1, typename E1, typename A1, typename A2>
impl::FunctionWrapper2e1<R, R (T::*)(B1, E1) const, A1, A2, E1> bind(R (T::*f)(B1, E1) const, A1 a1, A2 a2)
{
    return impl::FunctionWrapper2e1<R, R (T::*)(B1, E1) const, A1, A2, E1>(f, a1, a2);
}

template<typename R, typename T, typename B1, typename B2, typename A1, typename A2, typename A3>
impl::FunctionWrapper3<R, R (T::*)(B1, B2), A1, A2, A3> bind(R (T::*f)(B1, B2), A1 a1, A2 a2, A3 a3)
{
    return impl::FunctionWrapper3<R, R (T::*)(B1, B2), A1, A2, A3>(f, a1, a2, a3);
}

template<typename R, typename T, typename B1, typename B2, typename A1, typename A2, typename A3>
impl::FunctionWrapper3<R, R (T::*)(B1, B2) const, A1, A2, A3> bind(R (T::*f)(B1, B2) const, A1 a1, A2 a2, A3 a3)
{
    return impl::FunctionWrapper3<R, R (T::*)(B1, B2) const, A1, A2, A3>(f, a1, a2, a3);
}

template<typename R, typename T, typename B1, typename B2, typename E1, typename A1, typename A2, typename A3>
impl::FunctionWrapper3e1<R, R (T::*)(B1, B2, E1), A1, A2, A3, E1> bind(R (T::*f)(B1, B2, E1), A1 a1, A2 a2, A3 a3)
{
    return impl::FunctionWrapper3e1<R, R (T::*)(B1, B2, E1), A1, A2, A3, E1>(f, a1, a2, a3);
}

template<typename R, typename T, typename B1, typename B2, typename E1, typename A1, typename A2, typename A3>
impl::FunctionWrapper3e1<R, R (T::*)(B1, B2, E1) const, A1, A2, A3, E1> bind(R (T::*f)(B1, B2, E1) const, A1 a1, A2 a2, A3 a3)
{
    return impl::FunctionWrapper3e1<R, R (T::*)(B1, B2, E1) const, A1, A2, A3, E1>(f, a1, a2, a3);
}

template<typename R, typename T, typename B1, typename B2, typename B3, typename A1, typename A2, typename A3, typename A4>
impl::FunctionWrapper4<R, R (T::*)(B1, B2, B3), A1, A2, A3, A4> bind(R (T::*f)(B1, B2, B3), A1 a1, A2 a2, A3 a3, A4 a4)
{
    return impl::FunctionWrapper4<R, R (T::*)(B1, B2, B3), A1, A2, A3, A4>(f, a1, a2, a3, a4);
}

template<typename R, typename T, typename B1, typename B2, typename B3, typename A1, typename A2, typename A3, typename A4>
impl::FunctionWrapper4<R, R (T::*)(B1, B2, B3) const, A1, A2, A3, A4> bind(R (T::*f)(B1, B2, B3) const, A1 a1, A2 a2, A3 a3, A4 a4)
{
    return impl::FunctionWrapper4<R, R (T::*)(B1, B2, B3) const, A1, A2, A3, A4>(f, a1, a2, a3, a4);
}

template<typename R, typename T, typename B1, typename B2, typename B3, typename E1, typename A1, typename A2, typename A3, typename A4>
impl::FunctionWrapper4e1<R, R (T::*)(B1, B2, B3, E1), A1, A2, A3, A4, E1> bind(R (T::*f)(B1, B2, B3, E1), A1 a1, A2 a2, A3 a3, A4 a4)
{
    return impl::FunctionWrapper4e1<R, R (T::*)(B1, B2, B3, E1), A1, A2, A3, A4, E1>(f, a1, a2, a3, a4);
}

template<typename R, typename T, typename B1, typename B2, typename B3, typename E1, typename A1, typename A2, typename A3, typename A4>
impl::FunctionWrapper4e1<R, R (T::*)(B1, B2, B3, E1) const, A1, A2, A3, A4, E1> bind(R (T::*f)(B1, B2, B3, E1) const, A1 a1, A2 a2, A3 a3, A4 a4)
{
    return impl::FunctionWrapper4e1<R, R (T::*)(B1, B2, B3, E1) const, A1, A2, A3, A4, E1>(f, a1, a2, a3, a4);
}

template<typename R, typename T, typename B1, typename B2, typename B3, typename B4, typename A1, typename A2, typename A3, typename A4, typename A5>
impl::FunctionWrapper5<R, R (T::*)(B1, B2, B3, B4), A1, A2, A3, A4, A5> bind(R (T::*f)(B1, B2, B3, B4), A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{
    return impl::FunctionWrapper5<R, R (T::*)(B1, B2, B3, B4), A1, A2, A3, A4, A5>(f, a1, a2, a3, a4, a5);
}

template<typename R, typename T, typename B1, typename B2, typename B3, typename B4, typename A1, typename A2, typename A3, typename A4, typename A5>
impl::FunctionWrapper5<R, R (T::*)(B1, B2, B3, B4) const, A1, A2, A3, A4, A5> bind(R (T::*f)(B1, B2, B3, B4) const, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{
    return impl::FunctionWrapper5<R, R (T::*)(B1, B2, B3, B4) const, A1, A2, A3, A4, A5>(f, a1, a2, a3, a4, a5);
}

template<typename R, typename T, typename B1, typename B2, typename B3, typename B4, typename E1, typename A1, typename A2, typename A3, typename A4, typename A5>
impl::FunctionWrapper5e1<R, R (T::*)(B1, B2, B3, B4, E1), A1, A2, A3, A4, A5, E1> bind(R (T::*f)(B1, B2, B3, B4, E1), A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{
    return impl::FunctionWrapper5e1<R, R (T::*)(B1, B2, B3, B4, E1), A1, A2, A3, A4, A5, E1>(f, a1, a2, a3, a4, a5);
}

template<typename R, typename T, typename B1, typename B2, typename B3, typename B4, typename E1, typename A1, typename A2, typename A3, typename A4, typename A5>
impl::FunctionWrapper5e1<R, R (T::*)(B1, B2, B3, B4, E1) const, A1, A2, A3, A4, A5, E1> bind(R (T::*f)(B1, B2, B3, B4, E1) const, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{
    return impl::FunctionWrapper5e1<R, R (T::*)(B1, B2, B3, B4, E1) const, A1, A2, A3, A4, A5, E1>(f, a1, a2, a3, a4, a5);
}

template<typename R, typename T, typename B1, typename B2, typename B3, typename B4, typename B5, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
impl::FunctionWrapper6<R, R (T::*)(B1, B2, B3, B4, B5), A1, A2, A3, A4, A5, A6> bind(R (T::*f)(B1, B2, B3, B4, B5), A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
{
    return impl::FunctionWrapper6<R, R (T::*)(B1, B2, B3, B4, B5), A1, A2, A3, A4, A5, A6>(f, a1, a2, a3, a4, a5, a6);
}

template<typename R, typename T, typename B1, typename B2, typename B3, typename B4, typename B5, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
impl::FunctionWrapper6<R, R (T::*)(B1, B2, B3, B4, B5) const, A1, A2, A3, A4, A5, A6> bind(R (T::*f)(B1, B2, B3, B4, B5) const, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
{
    return impl::FunctionWrapper6<R, R (T::*)(B1, B2, B3, B4, B5) const, A1, A2, A3, A4, A5, A6>(f, a1, a2, a3, a4, a5, a6);
}

template<typename R, typename T, typename B1, typename B2, typename B3, typename B4, typename B5, typename E1, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
impl::FunctionWrapper6e1<R, R (T::*)(B1, B2, B3, B4, B5, E1), A1, A2, A3, A4, A5, A6, E1> bind(R (T::*f)(B1, B2, B3, B4, B5, E1), A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
{
    return impl::FunctionWrapper6e1<R, R (T::*)(B1, B2, B3, B4, B5, E1), A1, A2, A3, A4, A5, A6, E1>(f, a1, a2, a3, a4, a5, a6);
}

template<typename R, typename T, typename B1, typename B2, typename B3, typename B4, typename B5, typename E1, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
impl::FunctionWrapper6e1<R, R (T::*)(B1, B2, B3, B4, B5, E1) const, A1, A2, A3, A4, A5, A6, E1> bind(R (T::*f)(B1, B2, B3, B4, B5, E1) const, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
{
    return impl::FunctionWrapper6e1<R, R (T::*)(B1, B2, B3, B4, B5, E1) const, A1, A2, A3, A4, A5, A6, E1>(f, a1, a2, a3, a4, a5, a6);
}

template<typename R, typename T, typename B1, typename B2, typename B3, typename B4, typename B5, typename B6, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
impl::FunctionWrapper7<R, R (T::*)(B1, B2, B3, B4, B5, B6), A1, A2, A3, A4, A5, A6, A7> bind(R (T::*f)(B1, B2, B3, B4, B5, B6), A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
{
    return impl::FunctionWrapper7<R, R (T::*)(B1, B2, B3, B4, B5, B6), A1, A2, A3, A4, A5, A6, A7>(f, a1, a2, a3, a4, a5, a6, a7);
}

template<typename R, typename T, typename B1, typename B2, typename B3, typename B4, typename B5, typename B6, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
impl::FunctionWrapper7<R, R (T::*)(B1, B2, B3, B4, B5, B6) const, A1, A2, A3, A4, A5, A6, A7> bind(R (T::*f)(B1, B2, B3, B4, B5, B6) const, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
{
    return impl::FunctionWrapper7<R, R (T::*)(B1, B2, B3, B4, B5, B6) const, A1, A2, A3, A4, A5, A6, A7>(f, a1, a2, a3, a4, a5, a6, a7);
}

template<typename R, typename T, typename B1, typename B2, typename B3, typename B4, typename B5, typename B6, typename E1, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
impl::FunctionWrapper7e1<R, R (T::*)(B1, B2, B3, B4, B5, B6, E1), A1, A2, A3, A4, A5, A6, A7, E1> bind(R (T::*f)(B1, B2, B3, B4, B5, B6, E1), A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
{
    return impl::FunctionWrapper7e1<R, R (T::*)(B1, B2, B3, B4, B5, B6, E1), A1, A2, A3, A4, A5, A6, A7, E1>(f, a1, a2, a3, a4, a5, a6, a7);
}

template<typename R, typename T, typename B1, typename B2, typename B3, typename B4, typename B5, typename B6, typename E1, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
impl::FunctionWrapper7e1<R, R (T::*)(B1, B2, B3, B4, B5, B6, E1) const, A1, A2, A3, A4, A5, A6, A7, E1> bind(R (T::*f)(B1, B2, B3, B4, B5, B6, E1) const, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
{
    return impl::FunctionWrapper7e1<R, R (T::*)(B1, B2, B3, B4, B5, B6, E1) const, A1, A2, A3, A4, A5, A6, A7, E1>(f, a1, a2, a3, a4, a5, a6, a7);
}

}  // namespace tr1
}  // namespace nonstd

#endif

