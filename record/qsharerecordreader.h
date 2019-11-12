#ifndef QSHARERECORDREADER_H
#define QSHARERECORDREADER_H

#include <QThread>


class QShareRecordReader : public QThread
{
    Q_OBJECT
public:
    explicit QShareRecordReader(const QString& fileName, QObject *parent = 0);
protected:
    void    run();

signals:

public slots:
    void    slotReadRecordFromFile(const QString& file);
private:
    QString     mFileName;
};

#endif // QSHARERECORDREADER_H
