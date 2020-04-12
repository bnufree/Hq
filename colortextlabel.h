#ifndef COLORTEXTLABEL_H
#define COLORTEXTLABEL_H

#include <QLabel>

class ColorTextLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(int textColor READ textColor WRITE setTextColor)
public:
    explicit ColorTextLabel(QWidget *parent = 0);

    int textColor()const;
    void setTextColor(int sts);

signals:

public slots:
private:
    int     mTextColor;
};

#endif // COLORTEXTLABEL_H
