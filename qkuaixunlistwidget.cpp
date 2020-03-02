#include "qkuaixunlistwidget.h"
#include "ui_qkuaixunlistwidget.h"
#include <QLabel>
#include <QDebug>
#include <QResizeEvent>
#include <QTextBrowser>
#include <QScroller>
#include <QScrollBar>

ConfortableLabel::ConfortableLabel(const QString &text, QWidget *parent) : QLabel(text, parent)
{

}

void ConfortableLabel::updateWidth(int width)
{
    int total_width = fontMetrics().width(text());
    int row = (total_width + width - 1) / width;
    setContentsMargins(1,1,1,1);
    setMinimumHeight(fontMetrics().height() * row + 5 * (row-1) + 2 );

}

QInfoWidget::QInfoWidget(const KuaixunData &data, QWidget *parent) : QWidget(parent)
{
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);
    this->setLayout(new QVBoxLayout);
    mTitle = new ConfortableLabel(QString("%1 %2").arg(data.src_time).arg(data.sourceString()), this);
    mContent = new QTextBrowser(this);
    mContent->insertPlainText(data.digest);
    mContent->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    layout()->setSpacing(1);
    layout()->setContentsMargins(1, 1,1,1);
    layout()->addWidget(mTitle);
    layout()->addWidget(mContent);
//    this->setMinimumWidth(title->width());
}

void QInfoWidget::setReferWidth(int width)
{
    setFixedWidth(width);
//    mContent->updateWidth(width);
    mContent->adjustSize();
    setWindowFlags(Qt::SubWindow);

}

QKuaixunListWidget::QKuaixunListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QKuaixunListWidget)
{
    ui->setupUi(this);
//    ui->mTextBrowser->setDocumentTitle(QStringLiteral("7*24快讯"));
#ifndef Q_OS_WIN
    QScroller::grabGesture(ui->listWidget);
#endif
    this->setStyleSheet("font-size:12pt;");
    ui->listWidget->setWrapping(false);
    ui->listWidget->setWordWrap(true);
    ui->listWidget->verticalScrollBar()->setVisible(false);
    ui->listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->listWidget->setSpacing(10);
}

QKuaixunListWidget::~QKuaixunListWidget()
{
    delete ui;
}



void QKuaixunListWidget::appendData(const KuaiXunList &list)
{
    for(int i=list.size()-1; i>=0; i--)
    {
        KuaixunData data = list.at(i);
//        if(data.strid == mLastDataID){
//            break;
//        }
        QStringList content;
        content.append(data.src_time + "  " + data.sourceString());
        content.first().append("\n");
        content.first().append(data.digest);
//        content.append("\n");
        ui->listWidget->insertItems(0, content);
//        ui->mTextBrowser->append(data.digest);
//        ui->mTextBrowser->append("\n");
//        mLastDataID = data.strid;
//        ui->tableWidget->insertRow(0);
//        QInfoWidget *text = new QInfoWidget(data, this);
//        ui->tableWidget->setCellWidget(0, 0, text);
//        text->setReferWidth(ui->tableWidget->columnWidth(0)-10);
////        qDebug()<<"width:"<<text->width();
//        ui->tableWidget->setRowHeight(0, text->height() + 20);
//        mLastDataID = data.strid;
//        if(ui->tableWidget->rowCount() == 100)
//        {
//            ui->tableWidget->removeRow(99);
//        }
    }
//    int rowCount = ui->tableWidget->rowCount();
//    if(rowCount)
//    {
//        ui->tableWidget->scrollToTop();
//    }
}
