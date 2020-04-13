#include "qpopwindow.h"
#include "ui_qpopwindow.h"
#include <QDebug>
#include <QTextBlock>
#include <QTimer>
#include <QDesktopWidget>
#include <QDateTime>

QPopWindow::QPopWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QPopWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(windowFlags() | Qt::SubWindow);
    ui->textEdit->clear();
    QTimer* timer = new QTimer;
    timer->setInterval(1000 * 20);
    connect(timer, &QTimer::timeout, this, [=](){
        QStringList list;
        list.append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
//        list.append(QStringLiteral("迪士尼为本片在中国进行了内部试映，尤其包括迪士尼的中国高管参加了试映。在早期版本里，木兰和陈洪辉有一场桥上的吻别戏，导演Niki Caro表示尽管这场戏非常美，但中国这边给出的意见是\"不能这么拍，对于中国观众来说不合适。\"所以就删掉了。"));
        appendText(list);
    });

    timer->start();
}

QPopWindow::~QPopWindow()
{
    delete ui;
}

void QPopWindow::appendText(const QStringList &list)
{
    show();
    ui->textEdit->clear();
    foreach (QString str, list) {
        ui->textEdit->append(str);
    }
    calc();
    QTimer::singleShot(10000, this, SLOT(hide()));

}

void QPopWindow::calc()
{
    QTextDocument *doc = ui->textEdit->document();
    int wid = ui->textEdit->width();
    int line_num = 0;
    int text_height = fontMetrics().height();
    for(int i=0; i<doc->blockCount(); i++)
    {
        QTextBlock block = doc->findBlockByNumber(i);
        QString text = block.text();
        int text_width = this->fontMetrics().width(text);
        int lines = (text_width + wid - 1) / wid;
        line_num += lines;

    }

    int est_height = line_num * text_height + (line_num-1)*10;
    this->setFixedHeight(est_height);

    //重新校正位置
    int real_width = this->width();
    int real_height = this->height();
    QRect rect = QApplication::desktop()->availableGeometry();
    move(QPoint(rect.width() - real_width - 10, rect.height() - real_height - 5));
}
