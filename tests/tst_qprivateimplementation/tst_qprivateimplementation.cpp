/****************************************************************************
**
** This file is part of the $PACKAGE_NAME$.
**
** Copyright (C) $THISYEAR$ $COMPANY_NAME$.
**
** $QT_EXTENDED_DUAL_LICENSE$
**
****************************************************************************/

#include "qprivateimplementationdef.h"
#include <QObject>
#include <QString>
#include <QTest>


// Class A provides the property 'name'
class A_Impl;
class A : public QPrivatelyImplemented<A_Impl>
{
public:
    // Typedef to name our implementation class
    typedef A_Impl ImplementationType;

    // Constructor for client use
    A(const QString& name = QString());

    QString name() const;
    void setName(const QString& name);

    virtual QString toString() const;

protected:
    // Constructor for subclasses' use
    template<typename Subclass>
    A(Subclass* p, const QString& name = QString());
};


// Class B provides the interface of A, plus the property 'address'
class B_Impl;
class B : public A
{
public:
    // Typedef to name our implementation class
    typedef B_Impl ImplementationType;

    // Constructor for client use
    B(const QString& name = QString(), const QString& address = QString());

    QString address() const;
    void setAddress(const QString& address);

    virtual QString toString() const;
};


// Class A_Impl implements A's interface
class A_Impl : public QPrivateImplementationBase
{
    QString _name;

public:
    A_Impl();

    QString name() const;
    void setName(const QString& name);

protected:
    template<typename Subclass>
    A_Impl(Subclass* p);
};

// Create an A_Impl directly
A_Impl::A_Impl()
    : QPrivateImplementationBase(this)
{
}

// Create an A_Impl object with the space allocated by a subtype class
template<typename Subclass>
A_Impl::A_Impl(Subclass* p)
    : QPrivateImplementationBase(p)
{
}

QString A_Impl::name() const
{
    return _name;
}

void A_Impl::setName(const QString& name)
{
    _name = name;
}

// Instantiate the implementation class for A
template class QPrivatelyImplemented<A_Impl>;


// Class B_Impl implements B's interface
class B_Impl : public A_Impl
{
    QString _address;

public:
    B_Impl();

    QString address() const;
    void setAddress(const QString& address);
};

// Create a B_Impl directly
B_Impl::B_Impl()
    : A_Impl(this)
{
}

QString B_Impl::address() const
{
    return _address;
}

void B_Impl::setAddress(const QString& address)
{
    _address = address;
}

// Instantiate the implementation class for B
template class QPrivatelyImplemented<B_Impl>;


// Implement class A by delegation, now that the definition of A_Impl is visible
A::A(const QString& name)
    : QPrivatelyImplemented<A_Impl>(new A_Impl)
{
    impl(this)->setName(name);
}

template<typename Subclass>
A::A(Subclass* p, const QString& name)
    : QPrivatelyImplemented<A_Impl>(p)
{
    setName(name);
}

QString A::name() const
{
    return impl(this)->name();
}

void A::setName(const QString& name)
{
    impl(this)->setName(name);
}

QString A::toString() const
{
    return name();
}

// Implement class B by delegation, now that the definition of B_Impl is visible
B::B(const QString& name, const QString& address)
    : A(new B_Impl)
{
    setName(name);
    setAddress(address);
}

QString B::address() const
{
    return impl(this)->address();
}

void B::setAddress(const QString& address)
{
    impl(this)->setAddress(address);
}

QString B::toString() const
{
    return name() + ':' + address();
}


class tst_QPrivateImplementation : public QObject
{
    Q_OBJECT

public:
    tst_QPrivateImplementation();
    virtual ~tst_QPrivateImplementation();

private slots:
    void basicTest();
};

QTEST_MAIN(tst_QPrivateImplementation)

#include "tst_qprivateimplementation.moc"


tst_QPrivateImplementation::tst_QPrivateImplementation()
{
}

tst_QPrivateImplementation::~tst_QPrivateImplementation()
{
}

void tst_QPrivateImplementation::basicTest()
{
    QString homer("Homer J. Simpson");
    QString monty("C. Montgomery Burns");
    QString bart("Bart Simpson");
    QString marge("Marge Simpson");

    QString evergreen("Evergreen Tce.");
    QString mammon("Mammon St.");

    A a1(homer);
    A a2(a1);
    QCOMPARE(a1.toString(), homer);
    QCOMPARE(a2.toString(), homer);

    a1.setName(monty);
    QCOMPARE(a1.toString(), monty);
    QCOMPARE(a2.toString(), homer);

    a2 = a1;
    QCOMPARE(a1.toString(), monty);
    QCOMPARE(a2.toString(), monty);

    B b1(homer, evergreen);
    B b2(b1);
    QCOMPARE(b1.toString(), homer + ':' + evergreen);
    QCOMPARE(b2.toString(), homer + ':' + evergreen);

    b1.setName(monty);
    b1.setAddress(mammon);
    QCOMPARE(b1.toString(), monty + ':' + mammon);
    QCOMPARE(b2.toString(), homer + ':' + evergreen);

    b2 = b1;
    QCOMPARE(b1.toString(), monty + ':' + mammon);
    QCOMPARE(b2.toString(), monty + ':' + mammon);

    b1.setName(bart);
    a1 = b1;
    QCOMPARE(a1.toString(), bart);
    QCOMPARE(b1.toString(), bart + ':' + mammon);

    a1.setName(marge);
    QCOMPARE(a1.toString(), marge);
    QCOMPARE(b1.toString(), bart + ':' + mammon);

    static_cast<A&>(b1) = a1;
    QCOMPARE(a1.toString(), marge);
    QCOMPARE(b1.toString(), marge + ':' + mammon);

    b1.setAddress(evergreen);
    QCOMPARE(a1.toString(), marge);
    QCOMPARE(b1.toString(), marge + ':' + evergreen);
}
