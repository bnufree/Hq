#include "qshareexchangedisplaywidget.h"
#include "ui_qshareexchangedisplaywidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <qandroidbutton.h>
#include "table/qshareexchangetable.h"

QShareExchangeDisplayWidget::QShareExchangeDisplayWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QShareExchangeDisplayWidget)
{
    ui->setupUi(this);
    QVBoxLayout* vlayout = new QVBoxLayout();
    this->setLayout(vlayout);
    QHBoxLayout* btnLayout = new QHBoxLayout;
    vlayout->addLayout(btnLayout);
    mContainter = new QStackedWidget(this);
    vlayout->addWidget(mContainter);

    addBtnAndTable(btnLayout, QStringLiteral("交易记录表"), new QShareExchangeTablewidget(this));
    addBtnAndTable(btnLayout, QStringLiteral("资金日报"), new QShareExchangeTablewidget(this));
    addBtnAndTable(btnLayout, QStringLiteral("资金月报"), new QShareExchangeTablewidget(this));
    addBtnAndTable(btnLayout, QStringLiteral("资金年报"), new QShareExchangeTablewidget(this));
    addBtnAndTable(btnLayout, QStringLiteral("证券记录表"), new QShareProfitTablewidget(this));

    QAndroidButton* item = qobject_cast<QAndroidButton*>(btnLayout->itemAt(0)->widget());
    if(item)
    {
        emit item->clicked();
    }

}

void QShareExchangeDisplayWidget::addBtnAndTable(QLayout* layout, const QString &title, QWidget *w)
{
    QAndroidButton* btn = new QAndroidButton(title, this);
    layout->addWidget(btn);
    btn->setRelatedWidget(w);
    mContainter->addWidget(btn->relatedWidget());
    connect(btn, &QAndroidButton::clicked, [=](){
        mContainter->setCurrentWidget(btn->relatedWidget());
        for(int i=0; i<layout->count(); i++)
        {
            QAndroidButton* item = qobject_cast<QAndroidButton*>(layout->itemAt(i)->widget());
            if(item)
            {
                item->setSelected(item == btn);
            }
        }
    });

}

QShareExchangeDisplayWidget::~QShareExchangeDisplayWidget()
{
    delete ui;
}
