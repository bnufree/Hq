#include "qindexframe.h"
#include "ui_qindexframe.h"
#include <QDebug>
#include "data_structure/hqutils.h"

QIndexFrame::QIndexFrame(const QString& name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QIndexFrame)
{
    ui->setupUi(this);
    ui->name->setText(name.trimmed());

    setFixedSize(calSize());
}

QIndexFrame::~QIndexFrame()
{
    delete ui;
}


QSize QIndexFrame::calSize() const
{
    int half_height = HqUtils::convertMM2Pixel(6);
    QFont font = ui->name->font();
    HqUtils::setFontPixelSize(&font, half_height);
    font.setBold(false);
    ui->name->setFont(font);
    ui->cur->setFont(font);

    int total_height = 2* half_height + 4;
    int width = 18;
    //测试frame的宽度
    width += QFontMetrics(font).width(tr("上证指数"));
    HqUtils::setFontPixelSize(&font,total_height);
    ui->chg->setFont(font);
    ui->chgper->setFont(font);
    ui->money->setFont(font);
    width += QFontMetrics(font).width(tr("+300.00"));
    width += QFontMetrics(font).width(tr("+10.12%"));
    width += QFontMetrics(font).width(tr("10000亿"));
    width += 18;
    return QSize(width, total_height);
}


void QIndexFrame::setName(const QString &name)
{
    ui->name->setText(name);
}

void QIndexFrame::updateVal(double cur, double chg, double chgper, double money)
{
    ui->chg->setText(QString("").sprintf("%.2f", chg));
    ui->cur->setText(QString("").sprintf("%.2f", cur));
    ui->chgper->setText(QString("").sprintf("%.2f%", chgper));
    ui->money->setText(QStringLiteral("%1亿").arg(QString::number(money / 100000000.0, 'f', 0)));
    int chgint = (int)(chg*100);
    QString newColor = QString("color:%1").arg(chgint == 0? "black" : chgint > 0? "red":"green");
    QString oldStyleSheet = this->styleSheet();
    oldStyleSheet.replace(QRegularExpression("color:[a-z0-9\\(\\),]{1,}"), newColor);
    this->setStyleSheet(oldStyleSheet);
}

void QIndexFrame::updateBound(double shVal, QString shName, double szVal, QString szName)
{
    ui->cur->setText(shName);
    ui->chg->setText(QStringLiteral("%1亿").arg(QString::number(shVal / 10000.0, 'f', 2)));
    ui->chgper->setText(szName);
    ui->money->setText(QStringLiteral("%1亿").arg(QString::number(szVal / 10000.0, 'f', 2)));
    int chgint = (int)(shVal+szVal);
    this->setStyleSheet(QString("QLabel{"
                            "font-weight:bold;"
                                "font-size:18pt;"
                            "color:%1;"
                            "alignment:center;"
                                "}"
                                "#name,#cur{"
                                "font-size:10pt;"
                                "}")
                        .arg(chgint == 0? "black" : chgint > 0? "red":"green"));
}

void QIndexFrame::updateBound(double pure, const QString &name)
{
    ui->name->setText(name);
    ui->name->setVisible(false);
    ui->cur->setText(name);
    ui->cur->setVisible(false);
    ui->chg->setText(name);
    ui->chgper->setText(QStringLiteral("%1亿").arg(QString::number(pure / 10000.0, 'f', 2)));
    ui->money->setVisible(false);
    int chgint = (int)(pure);
    QString newColor = QString("color:%1").arg(chgint == 0? "black" : chgint > 0? "red":"green");
    QString oldStyleSheet = this->styleSheet();
    oldStyleSheet.replace(QRegularExpression("color:[a-z0-9\\(\\),]{1,}"), newColor);
    this->setStyleSheet(oldStyleSheet);
}

void QIndexFrame::updateBound(double buy, double sell, double pure, double total)
{
    ui->cur->setText(QStringLiteral("买：%1亿").arg(QString::number(buy / 10000.0, 'f', 1)));
    ui->chg->setText(QStringLiteral("卖：%1亿").arg(QString::number(sell / 10000.0, 'f', 1)));
    ui->chgper->setText(QStringLiteral("净：%1亿").arg(QString::number(pure / 10000.0, 'f', 1)));
    ui->money->setText(QStringLiteral("总：%1亿").arg(QString::number(total / 10000.0, 'f', 0)));
    int chgint = (int)(pure);
    this->setStyleSheet(QString("QLabel{"
                            "font-weight:bold;"
                            "color:%1;"
                            "alignment:center;"
                                "}")
                        .arg(chgint == 0? "black" : chgint > 0? "red":"green"));
}
