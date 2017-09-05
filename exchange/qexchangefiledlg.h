#ifndef QEXCHANGEFILEDLG_H
#define QEXCHANGEFILEDLG_H

#include <QDialog>

namespace Ui {
class QExchangeFileDlg;
}

class QExchangeFileDlg : public QDialog
{
    Q_OBJECT

public:
    explicit QExchangeFileDlg(QWidget *parent = 0);
    ~QExchangeFileDlg();

private slots:
    void on_importBtn_clicked();

private:
    Ui::QExchangeFileDlg *ui;
};

#endif // QEXCHANGEFILEDLG_H
