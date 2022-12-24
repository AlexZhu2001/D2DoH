#ifndef LOGGER_H
#define LOGGER_H

#include <QDataStream>
#include <QDateTime>
#include <QDebug>
#include <QFile>

#define ENABLE_DEBUG

class Logger : QObject
{
    Q_OBJECT

public:
    static void log(const QString& PREFIX, const QString& str)
    {
        const QDateTime& time = QDateTime::currentDateTime();
        static Logger logger;
        logger._log(QString("[%1]<%2>%3\n")
                        .arg(time.toString("MM-dd hh:mm:ss"), PREFIX, str));
    }

private:
    QFile* f;
    Logger()
        : f(new QFile("Log.log", this))
    {
        f->open(QFile::WriteOnly | QFile::Truncate);
    }
    ~Logger()
    {
        f->flush();
        f->close();
        delete f;
    }
    void _log(const QString& str)
    {
        f->write(str.toUtf8());
#ifdef ENABLE_DEBUG
        qInfo() << str.toUtf8().data();
#endif
    }
};

#endif // LOGGER_H
