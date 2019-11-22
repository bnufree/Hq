#include "qsinastkresultmergethread.h"
#include "qsinastkinfothread.h"
#include <QDebug>
#include "data_structure/sharedata.h"
#include <QRegularExpression>
#include "dbservices/dbservices.h"

QSinaStkResultMergeThread::QSinaStkResultMergeThread(QObject *parent) : QThread(parent)
{
    mStkCntPerTrd = 200;
    mTotalPage = -1;
    mPageSize = 50;
    mCurPage = 1;
    mMktType = MKT_ALL;
    mActive = true;
    QEastMoneyZjlxThread *zjt = new QEastMoneyZjlxThread();
}

QSinaStkResultMergeThread::~QSinaStkResultMergeThread()
{

}

void QSinaStkResultMergeThread::setActive(bool active)
{
    mActive = active;
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
}

void QSinaStkResultMergeThread::run()
{
    while (true) {
        ShareDataList wklist;
        mListMutex.lock();
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
        mListMutex.unlock();
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
                emit sendStkDataList(mid);
            }
        } else
        {
            emit sendStkDataList(ShareDataList());
        }

        sleep(2);
    }


}




void QSinaStkResultMergeThread::setMktType(int type)
{
    qDebug()<<"mkt:"<<type;
    mMktType = (MKT_TYPE)type;
    mCurPage = 1;
    qDebug()<<"set mkt:"<<mMktType;
    //updateStkCodes(mMktType);
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

}

void QSinaStkResultMergeThread::setDisplayChinaTop10()
{
    setMktType( MKT_LGT_TOP10);
}

