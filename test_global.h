#ifndef TEST_GLOBAL_H
#define TEST_GLOBAL_H

#include "gtest.h"
#include <QList>
#include <QStringList>
#include <QString>
#include <QByteArray>

class QObject;
class QThread;
class FakeServer;
class QSignalSpy;

//Methods for pretty printing Qt class'es values
GTEST_API_ void PrintTo(const QString& str, ::std::ostream* os);
GTEST_API_ void PrintTo(const QByteArray& str, ::std::ostream* os);

GTEST_API_ QStringList findWrongChangedSignalsInQmlWrapper(QObject *obj, const QStringList &excludes = {});
GTEST_API_ QList<QSignalSpy *> spiesForObject(QObject *obj, const QStringList &excludes = {});
GTEST_API_ QByteArray dataFromFile(const QString &fileName);
GTEST_API_ QByteArray binaryDataFromFile(const QString &fileName);

//Fake Server Runner class
class GTEST_API_ FakeServerRunner  // clazy:exclude=rule-of-three
{
public:
    FakeServerRunner(int port = 9091);
    ~FakeServerRunner();

    void runServer();
    void addAnswerHeader(const QString &header, const QString &value);
    void clearAnswerHeaders();
    void setServerAnswer(const QByteArray &answer);
    bool serverIsRunning() const;
    void setResultCode(int code, const QByteArray &reasonPhrase);
    QByteArray lastQuery() const;

private:
    QThread *m_serverThread;
    FakeServer *m_server;
};

#endif // TEST_GLOBAL_H
