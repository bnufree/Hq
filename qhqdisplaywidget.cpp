#include "qhqdisplaywidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "qindexwidget.h"
#include "table/qblocktablewidget.h"
#include "table/qsharetablewidget.h"

QHqDisplayWidget::QHqDisplayWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vlayour = new QVBoxLayout(this);
    setLayout(vlayour);

}
