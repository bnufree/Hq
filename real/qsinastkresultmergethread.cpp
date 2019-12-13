#include "qsinastkresultmergethread.h"
#include "qsinastkinfothread.h"
#include <QDebug>
#include "data_structure/sharedata.h"
#include <QRegularExpression>
#include "dbservices/dbservices.h"

QSinaStkResultMergeThread::QSinaStkResultMergeThread(int pagesize, QObject *parent) : QThread(parent)
{
    mParamChange = false;
    mIsCodeChg = false;
    mStkCntPerTrd = 200;
    mTotalPage = -1;
    mPageSize = pagesize;
    mCurPage = 1;
    mMktType = MKT_ALL;
    mActive = true;
    QEastMoneyZjlxThread *zjt = new QEastMoneyZjlxThread();
}


void QSinaStkResultMergeThread::setShareCodes(const QStringList &list)
{
    QMutexLocker locker(&mCodeMutex);
    mIsCodeChg = true;
    mAllShareCodesList = list;
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
    mCurPage = 1;
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
    QList<QSinaStkInfoThread*> hqInfoThreadList;
    while (true) {
        //检查当前的代码是否发生了变化
        if(mIsCodeChg)
        {
            QMutexLocker locker(&mCodeMutex);
            //重新生成行情信息线程
            //先取消所有线程
            while (hqInfoThreadList.size() > 0)
            {
                QSinaStkInfoThread* thread = hqInfoThreadList.takeFirst();
                thread->cancel();
            }
            //生成新的线程
            int nthread = 10;
            int thread_code = (mAllShareCodesList.length() + nthread-1 ) / nthread;
            for(int i=0; i<nthread; i++)
            {
                QStringList wklist = mAllShareCodesList.mid(i*thread_code, thread_code);
                QSinaStkInfoThread *wkthread = new QSinaStkInfoThread(wklist, false);
                connect(wkthread, SIGNAL(finished()), wkthread, SLOT(deleteLater()));
                hqInfoThreadList.append(wkthread);
                wkthread->start();
            }
            mIsCodeChg = false;
        }

//        qDebug()<<"start update list"<<QDateTime::currentDateTime();
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
                bool sts = (mMktType == MKT_SH && (data.mShareType & SHARE_SH))||\
                        (mMktType == MKT_SZ && (data.mShareType & SHARE_SZ)) ||\
                        (mMktType == MKT_ZXB && (data.mShareType & SHARE_SZ_ZXB))||\
                        (mMktType == MKT_CYB && (data.mShareType & SHARE_SZ_CYB))||\
                        (mMktType == MKT_ZXG && data.mIsFav)||\
                        (mMktType == MKT_LGT_TOP10 && data.mHsgtData.mIsTop10) ||\
                        (mMktType == MKT_JJ && (data.mShareType & SHARE_FUND))||\
                        (mMktType == MKT_KCB && data.mCode.contains(QRegularExpression("68[0-9]{4}"))) ||
                        (mMktType == MKT_OTHER && mSelfCodesList.contains(data.mCode));

                if(sts && data.mCur > 0.001)
                {
                    wklist.append(data);
                }
            }
        }
        mTotalPage = (wklist.length() + mPageSize -1) / mPageSize;

        if(wklist.length())
        {
            //开始排序
            if(ShareData::stk_sort_rule != 0)
            {
                qSort(wklist.begin(), wklist.end(), ShareData::ShareSort);
            }
            if(mActive)
            {
                ShareDataList mid = wklist.mid((mCurPage - 1) * mPageSize, mPageSize);
                //qDebug()<<"mid:"<<mid.length();
                emit sendStkDataList(mCurPage, mPageSize, mid, QDateTime::currentMSecsSinceEpoch());
            }
        } else
        {
            emit sendStkDataList(mCurPage, mPageSize, ShareDataList(), QDateTime::currentMSecsSinceEpoch());
        }
        //qDebug()<<"update list info end:"<<t.elapsed();

        if(!mParamChange)
        {
            msleep(500);
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
    mCurPage = 1;
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
    mCurPage = 1;
}

void QSinaStkResultMergeThread::displayLast()
{
    mCurPage = mTotalPage;
}

void QSinaStkResultMergeThread::displayPrevious()
{
    mCurPage--;
    if(mCurPage <= 0) mCurPage = 1;
}

void QSinaStkResultMergeThread::displayNext()
{
    mCurPage++;
    if(mCurPage > mTotalPage) mCurPage = mTotalPage;
}

void QSinaStkResultMergeThread::setCurPage(int page)
{
    mCurPage = page;
    if(mCurPage >= mTotalPage)
    {
        mCurPage = mTotalPage;
    }
    if(mCurPage <= 0)
        mCurPage = 1;
}

void QSinaStkResultMergeThread::setDisplayPage(int val)
{
    qDebug()<<"set page:"<<val<<QDateTime::currentDateTime();
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

