#ifndef QEXCHANGEDATEMANGAGEDIALOG_H
#define QEXCHANGEDATEMANGAGEDIALOG_H

#include <QDialog>

namespace Ui {
class QExchangeDateMangageDialog;
}

class QExchangeDateMangageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QExchangeDateMangageDialog(QWidget *parent = 0);
    ~QExchangeDateMangageDialog();

private slots:
    void on_DelBtn_clicked();

    void on_AddBtn_clicked();

    void on_calendarWidget_currentPageChanged(int year, int month);

    void on_CloseBtn_clicked();

private:
    Ui::QExchangeDateMangageDialog *ui;
    QStringList         mMgrDates;
};

#endif // QEXCHANGEDATEMANGAGEDIALOG_H
