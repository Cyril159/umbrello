/*
    Copyright 2015, 2019  Ralf Habacker  <ralf.habacker@freenet.de>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License or (at your option) version 3 or any later version
    accepted by the membership of KDE e.V. (or its successor approved
    by the membership of KDE e.V.), which shall act as a proxy
    defined in Section 14 of version 3 of the license.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TESTBASE_H
#define TESTBASE_H

// qt includes
#include <QObject>
#include <QtTest>

#ifdef RUN_ALL
#undef QCOMPARE
#define QCOMPARE(actual, expected) \
    QTest::qCompare(actual, expected, #actual, #expected, __FILE__, __LINE__)
#undef QVERIFY
#define QVERIFY(statement) \
    QTest::qVerify((statement), #statement, "", __FILE__, __LINE__)
#endif

#define IS_NOT_IMPL() QSKIP("not implemented yet", SkipSingle)

/**
 * The TestBase class is intended as base class for umbrello unit tests.
 *
 * Currently it provides an instance of class UMLApp, which is required
 * to run mostly unit tests.
 *
 * @author Ralf Habacker <ralf.habacker@freenet.de>
 */
class TestBase : public QObject
{
    Q_OBJECT
public:
    explicit TestBase(QObject *parent = 0);

protected slots:
    virtual void initTestCase();
    virtual void cleanupTestCase();
    virtual void cleanupOnExit(QObject *p);

protected:
    QList<QPointer<QObject>> m_objectsToDelete;
};

/**
 * The TestCodeGeneratorBase class is intended as base class for code generator unit tests
 *
 * Currently it provides a path to a temporary directory, where generated code could be
 * placed into. The temporary path is set as default output path for any code generating.
 *
 * @author Ralf Habacker <ralf.habacker@freenet.de>
 */
class TestCodeGeneratorBase : public TestBase
{
    Q_OBJECT
private slots:
    virtual void initTestCase();

protected:
    QString m_tempPath;  ///< holds path to temporary directory
    QString temporaryPath();
};

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
/**
 * Automatically block signals of QObject base class
 */
class SignalBlocker
{
public:
    SignalBlocker(QObject *o)
      : _o(o)
    {
        _state = _o->blockSignals(true);
    }

    SignalBlocker(QObject &o)
      : _o(&o)
    {
        _state = _o->blockSignals(true);
    }

    ~SignalBlocker()
    {
        _o->blockSignals(_state);
    }

protected:
    QObject *_o;
    bool _state;
};
#else
#include <QSignalBlocker>
typedef QSignalBlocker SignalBlocker;
#endif

/**
 * Set loading state to avoid signaling to tree view etc.
 */
class SetLoading
{
public:
    SetLoading();
    ~SetLoading();
protected:
    bool _state;
};

#endif // TESTBASE_H
