#ifndef QSHARECODENAMEWIDGET_H
#define QSHARECODENAMEWIDGET_H

#include <QWidget>
#include <QLabel>;

class QFavLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(bool favourite READ favourite WRITE setFavourite)
public:
    explicit QFavLabel(const QString& label, QWidget *parent = 0);
    void setFavourite(bool sts);
    bool favourite() const {return mIsFav;}
private:
    bool         mIsFav;
};

class QShareCodeNameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QShareCodeNameWidget(const QString& code, const QString& name, QWidget *parent = 0);
    void setCode(const QString& str);
    void setName(const QString& str);
    void setFavourite(bool sts);
private:
    void updateDisplay();
protected:
    void resizeEvent(QResizeEvent* e);

signals:

public slots:

private:
    QFavLabel      *mCodeLbl, *mNameLbl;
};

#endif // QSHARECODENAMEWIDGET_H
