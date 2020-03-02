#ifndef QANDROIDBUTTON_H
#define QANDROIDBUTTON_H

#include <QLabel>

class QAndroidButton : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(bool selected READ selected WRITE setSelected)
public:
    explicit QAndroidButton(const QString& label, QWidget *parent = 0);
    explicit QAndroidButton(QWidget *parent = 0) : QLabel(parent) {}
    bool     selected() const;
    void     setSelected(bool);
    QWidget*    relatedWidget() {return mRelatedWidget;}
    void        setRelatedWidget(QWidget* w) {mRelatedWidget = w;}

signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent *ev);
    void enterEvent(QEvent *ev);
    void leaveEvent(QEvent *ev);
public slots:

private:
    QWidget*            mRelatedWidget;
    bool                mIsSelected;
};

#endif // QANDROIDBUTTON_H
