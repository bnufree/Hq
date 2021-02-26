#include "qsinastkresultmergethread.h"
#include "qsinastkinfothread.h"
#include <QDebug>
#include "data_structure/sharedata.h"
#include <QRegularExpression>
#include "dbservices/dbservices.h"

QSinaStkResultMergeThread::QSinaStkResultMergeThread(QObject *parent) : QThread(parent)
{
    mParamChange = false;
    mIsCodeChg = false;
    mStkCntPerTrd = 200;
    mMktType = MKT_ALL;
    mActive = true;
    QEastMoneyZjlxThread *zjt = new QEastMoneyZjlxThread();
}


void QSinaStkResultMergeThread::setShareCodes(const QStringList &list)
{
    QMutexLocker locker(&mCodeMutex);
    mIsCodeChg = true;
    mAllShareCodesList = list;
    //重新生成行情信息线程
    //先取消所有线程
    while (mSubThreadList.size() > 0)
    {
        QSinaStkInfoThread* thread = mSubThreadList.takeFirst();
        thread->cancel();
    }
    //生成新的线程
    int thread_code_size = qCeil(mAllShareCodesList.size() / 2.0);
    int nthread = (mAllShareCodesList.size() + thread_code_size - 1) / thread_code_size;
    int start  = 0;
    for(int i=0; i<nthread; i++)
    {
        QStringList wklist = mAllShareCodesList.mid(start, thread_code_size);
        QSinaStkInfoThread *wkthread = new QSinaStkInfoThread(wklist, false);
        connect(wkthread, SIGNAL(finished()), wkthread, SLOT(deleteLater()));
        mSubThreadList.append(wkthread);
        wkthread->start();
        start += thread_code_size;
    }
}

QSinaStkResultMergeThread::~QSinaStkResultMergeThread()
{

}

void QSinaStkResultMergeThread::setActive(bool active)
{
    mActive = active;
    mParamChange = true;
}

bool QSinaStkResultMergeThread::isActive()
{
    return mActive;
}

void QSinaStkResultMergeThread::setSortType(int type)
{
    if(ShareData::stk_sort_type == type)
    {
        ShareData::stk_sort_rule = (ShareData::stk_sort_rule == -1 ? 1: -1);

    } else
    {
        ShareData::stk_sort_type = type;
        ShareData::stk_sort_rule = 1;
    }
    //
    if(mMktType == MKT_ZXG)
    {
        if(type == STK_DISPLAY_SORT_TYPE_CODE)
        {
            ShareData::stk_sort_rule = 0;
        }
    }
    mParamChange = true;
}


void QSinaStkResultMergeThread::run()
{
    while (true) {
        QTime t;
        t.start();
        ShareDataList wklist;
        ShareDataList total_list = DATA_SERVICE->getShareDataList();
        if(mMktType == MKT_ALL)
        {
            //全部进行排序，不过滤
            wklist.append(total_list);
        } else
        {
            foreach (ShareData data, total_list) {
                bool sts = (mMktType == MKT_SH && (data.mType & SHARE_SH))||\
                        (mMktType == MKT_SZ && (data.mType & SHARE_SZ)) ||\
                        (mMktType == MKT_ZXB && (data.mType & SHARE_SZ_ZXB))||\
                        (mMktType == MKT_CYB && (data.mType & SHARE_SZ_CYB))||\
                        (mMktType == MKT_ZXG && data.mIsFav)||\
                        (mMktType == MKT_LGT_TOP10 && data.mForeignInfo.mIsTop10) ||\
                        (mMktType == MKT_JJ && (data.mType & SHARE_FUND))||\
                        (mMktType == MKT_KCB && (data.mType & SHARE_SH_KCB)) ||\
                        (mMktType == MKT_KZZ && (data.mType & SHARE_KZZ)) ||\
                        (mMktType == MKT_OTHER && mSelfCodesList.contains(data.mCode));

                if(sts && data.mRealInfo.mClose > 0.001)
                {
                    wklist.append(data);
                }
            }
        }

        if(wklist.length())
        {
            //开始排序
            if(ShareData::stk_sort_rule != 0)
            {
                qSort(wklist.begin(), wklist.end(), ShareData::ShareSort);
            }
        }
        {

            QMutexLocker locker(&mListMutex);
            emit sendStkDataList(wklist, QDateTime::currentMSecsSinceEpoch());
        }

        if(!mParamChange)
        {
            msleep(2000);
        } else
        {
            mParamChange = false;
        }
    }


}




void QSinaStkResultMergeThread::setMktType(int type)
{
    qDebug()<<"mkt:"<<type;
    mMktType = (MKT_TYPE)type;
    qDebug()<<"set mkt:"<<mMktType;
    //updateStkCodes(mMktType);
    mParamChange = true;
}



void QSinaStkResultMergeThread::setSelfCodesList(const QStringList &list)
{
    mSelfCodesList = list;
    if(list.length() > 0)
    {
        mMktType = MKT_OTHER;
    } else
    {
        mMktType = MKT_ALL;
    }
    mParamChange = true;

    //qDebug()<<"selfcodes:"<<mSelfCodesList;
}

MktType QSinaStkResultMergeThread::getMktType()
{
    return mMktType;
}

void QSinaStkResultMergeThread::displayFirst()
{
}

void QSinaStkResultMergeThread::displayLast()
{
}

void QSinaStkResultMergeThread::displayPrevious()
{
}

void QSinaStkResultMergeThread::displayNext()
{
}

void QSinaStkResultMergeThread::setCurPage(int page)
{
}

void QSinaStkResultMergeThread::setDisplayPage(int val)
{
    qDebug()<<"set page:"<<val<<QDateTime::currentDateTime().toString("hh:mm:ss");
    if(val == FIRST_PAGE)
    {
        displayFirst();
    } else if(val == NEXT_PAGE)
    {
        displayNext();
    } else if(val == PRE_PAGE)
    {
        displayPrevious();
    } else if(val == END_PAGE)
    {
        displayLast();
    }
    mParamChange = true;

}

void QSinaStkResultMergeThread::setDisplayChinaTop10()
{
    setMktType( MKT_LGT_TOP10);
}

