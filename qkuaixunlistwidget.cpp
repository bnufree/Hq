#include "qkuaixunlistwidget.h"
#include "ui_qkuaixunlistwidget.h"
#include <QLabel>
#include <QDebug>
#include <QResizeEvent>
#include <QTextBrowser>
#include <QScroller>
#include <QScrollBar>
#include <QDesktopWidget>
#ifdef Q_OS_WIN
//#include <QtWebKitWidgets/QWebView>
#endif

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
    Qt::GestureType type = QScroller::grabGesture(ui->listWidget);
    qDebug()<<"gesture type:"<<type;
#endif
    this->setStyleSheet("font-size:16pt;");
    ui->listWidget->setFocusPolicy(Qt::NoFocus);
    ui->listWidget->setWrapping(false);
    ui->listWidget->setWordWrap(true);
    ui->listWidget->verticalScrollBar()->setVisible(false);
    ui->listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    ui->listWidget->setSpacing(12);
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(itemDoubleClicked(QListWidgetItem*)));
}

void QKuaixunListWidget::itemDoubleClicked(QListWidgetItem *item)
{
//#ifdef Q_OS_WIN
//    if(!item) return;
//    KuaixunData data = item->data(Qt::UserRole).value<KuaixunData>();
//    QWebView *view = new QWebView;
//    view->setUrl(QUrl(data.url));
//    view->setWindowFlags(Qt::WindowStaysOnTopHint);
//    view->setWindowTitle(data.title);
//    view->setAttribute(Qt::WA_DeleteOnClose);
//    QRect rect = QApplication::desktop()->availableGeometry();
//    QPoint center = rect.center();
//    rect.setWidth(rect.width() * 0.5);
//    rect.setHeight(rect.height() * 0.5);
//    rect.moveCenter(center);
//    view->setGeometry(rect);
//    view->show();
//#endif
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
        QStringList content;
        content.append(data.src_time + "  " + data.sourceString());
        content.first().append("\n");
        content.first().append(data.digest);
        ui->listWidget->insertItems(0, content);
        QListWidgetItem *item = ui->listWidget->item(0);
        item->setTextColor(QColor(255, 100,100));
        item->setFlags(item->flags()& ~Qt::ItemIsSelectable);
        item->setData(Qt::UserRole, QVariant::fromValue(data));
    }
    int rowCount = ui->listWidget->count();
    if(rowCount)
    {
        ui->listWidget->scrollToTop();
    }
    int sub_row = rowCount - 100;
    while (sub_row > 0) {
        ui->listWidget->takeItem(100);
        sub_row--;
    }
    if(!isVisible())
    {
        emit sendRecvInfoList(list);
    }
}
