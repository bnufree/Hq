#ifndef QSHARECODENAMEWIDGET_H
#define QSHARECODENAMEWIDGET_H

#include <QWidget>
class QLabel;

class QShareCodeNameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QShareCodeNameWidget(const QString& code, const QString& name, QWidget *parent = 0);
    void setCode(const QString& str);
    void setName(const QString& str);

signals:

public slots:

private:
    QLabel      *mCodeLbl, *mNameLbl;
};

#endif // QSHARECODENAMEWIDGET_H
