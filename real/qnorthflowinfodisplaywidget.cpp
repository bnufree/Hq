#include "qnorthflowinfodisplaywidget.h"
#include "ui_qnorthflowinfodisplaywidget.h"
#include <QPainter>


NorthFlowCurveWidget::NorthFlowCurveWidget(QWidget *parent) :
    QWidget(parent)
{
    mSH.setNamedColor("#f47920");
    mSZ.setNamedColor("#4e72b8");
    mTotal.setNamedColor("#45b97c");
    mDataList.clear();
    mMax = 10;
}

void NorthFlowCurveWidget::setLineColor(const QColor &sh, const QColor &sz, const QColor &total)
{
    mSH = sh;
    mSZ = sz;
    mTotal = total;
    update();
}

void NorthFlowCurveWidget::setNorthRealInfo(const QList<NorthBoundData> &list, int max)
{
    mDataList = list;
    if(max > mMax) mMax = max;
    update();
}

void NorthFlowCurveWidget::paintEvent(QPaintEvent *e)
{
    //设定当前的区域的背景色
    QPainter painters(this);
    painters.fillRect(this->rect(), Qt::black);

    //刻度值宽度高度的设定
    int yTextWidth = painters.fontMetrics().width("10000");
    int xTextHeight = painters.fontMetrics().height();

    QRect draw_rect = this->rect().marginsAdded(QMargins(-yTextWidth, -yTextWidth, -yTextWidth, -yTextWidth));



    //画图区域设定，曲线区域和图例区域比例为9：1
    QRect curve_rect = draw_rect;
    curve_rect.setWidth(curve_rect.width() - yTextWidth);
    curve_rect.moveLeft(curve_rect.left() + yTextWidth);
//    int curve_rect_height = (int)(ceil(draw_rect.height() * 0.9));
//    curve_rect.setHeight(curve_rect_height);

//    QRect symbol_rect = draw_rect;
//    symbol_rect.moveTopLeft(QPoint(draw_rect.x(), draw_rect.y() + curve_rect_height + xTextHeight + 10));
//    symbol_rect.setHeight(draw_rect.height() - curve_rect_height - xTextHeight - 10);
    painters.save();
    QPen pen = painters.pen();
    pen.setColor(Qt::white);
    //开始画横坐标
    double x_pitch = curve_rect.width() / 4.0;
    double y = curve_rect.bottom();
    double x = curve_rect.left();
    for(int i=0; i<5; i++)
    {
        QString text = "";
        if(i < 3)
        {
            painters.drawText(QPointF(x+x_pitch*i, y + xTextHeight), QString("%1:30").arg(9+i));
        } else
        {
            painters.drawText(QPointF(x+x_pitch*i, y + xTextHeight), QString("%1:00").arg(11+i));
        }
    }
    painters.restore();

    //开始Y方向的刻度
    double val_per_pitch = mMax * 2.0 / 10;
    double y_pitch = curve_rect.height() * 1.0 / 10;
    for(int i=0; i<11; i++)
    {
        double left_x = curve_rect.left();
        double left_y = curve_rect.bottom() - i * y_pitch;
        int val = (-1) * mMax + i * val_per_pitch;
        //开始画坐标刻度，上下各5个，共11个刻度
        painters.save();
        pen.setColor(Qt::lightGray);
        pen.setStyle(Qt::DashLine);
        painters.setPen(pen);
        painters.drawLine(QPointF(left_x, left_y), QPointF(curve_rect.right(), left_y));
        painters.restore();

        painters.save();
        pen.setColor(Qt::white);
        painters.setPen(pen);
        //计算文本的举行大小
        QRect text_rect(0, 0, yTextWidth, xTextHeight);
        text_rect.moveLeft(curve_rect.left() - yTextWidth);
        text_rect.moveBottom(left_y + 0.5 * xTextHeight);
        painters.drawText(text_rect, Qt::AlignRight | Qt::AlignVCenter, QString::number(val));
        painters.restore();

    }

    //开始画实时的曲线
    QPolygonF sh, sz, total;
    QTime start_time(9, 30, 0);
    double x_px =  curve_rect.width() * 1.0 / (4 * 3600);
    double y_px =  curve_rect.height() * 1.0 / (2 * mMax);
    for(int i=0; i<mDataList.size(); i++)
    {
        NorthBoundData data = mDataList[i];
        //计算数据对应的X坐标
        QTime cur = data.time.time();
        int time_elapsed = start_time.secsTo(cur);
        if(time_elapsed < 0) continue;
        if(cur.hour() == 11 && cur.minute() > 30) continue;
        if(cur.hour() == 12 ) continue;
        if(cur.hour() == 15 && cur.minute() > 1) continue;
        if(cur.hour() > 12) time_elapsed -= (3600 + 1800); // form 11:30 - 13;00
        double x = curve_rect.left() + time_elapsed * x_px;
        double sh_y = curve_rect.bottom() - (data.sh_flow + mMax) * y_px;
        double sz_y = curve_rect.bottom() - (data.sz_flow + mMax) * y_px;
        double total_y = curve_rect.bottom() - (data.total_flow + mMax) * y_px;
        sh.append(QPointF(x, sh_y));
        sz.append(QPointF(x, sz_y));
        total.append(QPointF(x, total_y));
    }
    painters.save();
    painters.setPen(QPen(mSH, 2));
    painters.drawPolyline(sh);
    painters.setPen(QPen(mSZ, 2));
    painters.drawPolyline(sz);
    painters.setPen(QPen(mTotal, 2));
    painters.drawPolyline(total);
    painters.restore();
}



QNorthFlowInfoDisplayWidget::QNorthFlowInfoDisplayWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QNorthFlowInfoDisplayWidget),
    mDisplayWidget(0)
{
    ui->setupUi(this);
    mDisplayWidget = new NorthFlowCurveWidget(this);
    if(!ui->widget->layout())
    {
        ui->widget->setLayout(new QHBoxLayout);
    }
    ui->widget->layout()->setMargin(0);
    ui->widget->layout()->addWidget(mDisplayWidget);
    ui->sh->setLineWidth(2);
    ui->sz->setLineWidth(2);
    ui->north->setLineWidth(2);
    ui->sh->setStyleSheet(QString("background-color:%1").arg(mDisplayWidget->mSH.name()));
    ui->sz->setStyleSheet(QString("background-color:%1").arg(mDisplayWidget->mSZ.name()));
    ui->north->setStyleSheet(QString("background-color:%1").arg(mDisplayWidget->mTotal.name()));
    mRealInfoThread = new QSinaNorthRealInfoThread;
    connect(mRealInfoThread, SIGNAL(signalUpdateNorthBoundList(QList<NorthBoundData>, int)), mDisplayWidget, SLOT(setNorthRealInfo(QList<NorthBoundData>,int)));
    mRealInfoThread->start();
}

QNorthFlowInfoDisplayWidget::~QNorthFlowInfoDisplayWidget()
{
    delete ui;
}
