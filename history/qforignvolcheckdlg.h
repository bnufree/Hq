#ifndef QFORIGNVOLCHECKDLG_H
#define QFORIGNVOLCHECKDLG_H

#include <QWidget>
#include <QThreadPool>
#include "data_structure/sharedata.h"

namespace Ui {
class QForignVolCheckDlg;
}


class QForignVolCheckDlg : public QWidget
{
    Q_OBJECT

public:
    explicit QForignVolCheckDlg(QWidget *parent = 0);
    ~QForignVolCheckDlg();
public slots:
    void slotFetchForeignData(const ShareForignVolFileDataList& list, const QDate& date);
    void slotFetchBtnClicked();
signals:
    void signalReadFileData(const QDate& date);
private:
    void initTable();

private:
    Ui::QForignVolCheckDlg *ui;
    QThreadPool         mPool;

};

#endif // QFORIGNVOLCHECKDLG_H
