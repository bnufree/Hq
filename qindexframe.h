#ifndef QINDEXFRAME_H
#define QINDEXFRAME_H

#include <QFrame>

namespace Ui {
class QIndexFrame;
}

struct IndexFrameData
{
    int         mType; //0:沪深港通; 1:指数
    QString     mCode;
    QString     mName;
    double      mCur;
    double      mChg;
    double      mChgPer;
    double      mTotal;

    bool operator ==( const IndexFrameData& other)
    {
        return this->mCode == other.mCode;
    }

};

class QIndexFrame : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(int status READ status WRITE setStatus)
public:
    explicit QIndexFrame(const QString& name, QWidget *parent = 0);
    ~QIndexFrame();
    QString getName() const {return mName;}
    int status()const;
    void setStatus(int sts);
    void updateData(const IndexFrameData& data);
    void updateVal(double cur, double chg, double chgper, double money);
    void updateBound(double buy, double sell, double pure, double total);
    void updateBound(double shVal, QString shName, double szVal, QString szName);
    void updateBound(double pure, const QString& name);
    void setName(const QString& name);
    void setLayoutLabel(QLayout* layout, int sts);
protected:
    void  adjustFrameFont(int width, int height) const;
    void  resizeEvent(QResizeEvent* e);

private:
    Ui::QIndexFrame *ui;
    int             mStatus;
    QString         mName;
};

#endif // QINDEXFRAME_H
