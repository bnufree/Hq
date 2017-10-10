#ifndef HQTABLEWIDGET_H
#define HQTABLEWIDGET_H

#include <QTableWidget>
#include <QMenu>
#include "utils/comdatadefines.h"

class HqTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit HqTableWidget(QWidget *parent = 0);
    void setHeaders(const TableColDataList& list);
    void appendRow();
    void setItemText(int row, int column, const QString& text, Qt::AlignmentFlag flg = Qt::AlignCenter);
    void setFavShareList(const QString& list);
    void appendFavShare(const QString& code);
    void removeFavShare(const QString& code);
    void updateFavShareIconOfRow(int row);
private slots:
    void prepareUpdateTable();
    void afterUpdateTable(int newRowCount);
signals:

public slots:
    void slotSetColDisplay(bool isDisplay);
    void slotCustomContextMenuRequested(const QPoint& point);

private:
    TableColDataList        mColDataList;
    int                     mColWidth;
    QMenu                   *mHeaderMenu;
    QStringList             mFavShareList;
    int                     mOldRowCount;
};

#endif // HqTableWidget_H
