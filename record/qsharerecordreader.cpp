#include "qsharerecordreader.h"
#include <QFile>
#include <QDebug>
#include <QTextCodec>

QShareRecordReader::QShareRecordReader(const QString& fileName, QObject *parent) :          QThread(parent),
    mFileName(fileName)
{

}

void QShareRecordReader::run()
{
    slotReadRecordFromFile(mFileName);
}

void QShareRecordReader::slotReadRecordFromFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) return;
    int line_no = 0;
    QTextCodec *code = QTextCodec::codecForName("gbk");
    while (!file.atEnd()) {
        QTextCodec::ConverterState state;
        QByteArray all = file.readAll();
        code->toUnicode(all.constData(), all.length(), &state);
        qDebug()<<"state:"<<state.invalidChars;
        break;

        QString line = QString::fromUtf8(file.readLine());
        if(line.isEmpty()) continue;
        QStringList line_list = line.split(",");
        if(line_list.isEmpty()) continue;
        line_no++;
        if(line_no == 1)
        {
            //read header now
            qDebug()<<"headers:"<<line_list;
            break;
        }
    }
    file.close();
}

