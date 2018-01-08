#ifndef QSHAREHISTORYDIALOG_H
#define QSHAREHISTORYDIALOG_H

#include <QDialog>

namespace Ui {
class QShareHistoryDialog;
}

class QShareHistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QShareHistoryDialog(QWidget *parent = 0);
    ~QShareHistoryDialog();

private:
    Ui::QShareHistoryDialog *ui;
};

#endif // QSHAREHISTORYDIALOG_H
