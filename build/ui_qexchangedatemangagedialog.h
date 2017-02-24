/********************************************************************************
** Form generated from reading UI file 'qexchangedatemangagedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QEXCHANGEDATEMANGAGEDIALOG_H
#define UI_QEXCHANGEDATEMANGAGEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_QExchangeDateMangageDialog
{
public:
    QCalendarWidget *calendarWidget;
    QPushButton *AddBtn;
    QListWidget *listWidget;
    QPushButton *DelBtn;
    QPushButton *CloseBtn;

    void setupUi(QDialog *QExchangeDateMangageDialog)
    {
        if (QExchangeDateMangageDialog->objectName().isEmpty())
            QExchangeDateMangageDialog->setObjectName(QStringLiteral("QExchangeDateMangageDialog"));
        QExchangeDateMangageDialog->resize(641, 300);
        calendarWidget = new QCalendarWidget(QExchangeDateMangageDialog);
        calendarWidget->setObjectName(QStringLiteral("calendarWidget"));
        calendarWidget->setGeometry(QRect(10, 40, 248, 169));
        AddBtn = new QPushButton(QExchangeDateMangageDialog);
        AddBtn->setObjectName(QStringLiteral("AddBtn"));
        AddBtn->setGeometry(QRect(260, 60, 75, 23));
        listWidget = new QListWidget(QExchangeDateMangageDialog);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(350, 50, 256, 192));
        DelBtn = new QPushButton(QExchangeDateMangageDialog);
        DelBtn->setObjectName(QStringLiteral("DelBtn"));
        DelBtn->setGeometry(QRect(260, 150, 75, 23));
        CloseBtn = new QPushButton(QExchangeDateMangageDialog);
        CloseBtn->setObjectName(QStringLiteral("CloseBtn"));
        CloseBtn->setGeometry(QRect(510, 260, 75, 23));

        retranslateUi(QExchangeDateMangageDialog);

        QMetaObject::connectSlotsByName(QExchangeDateMangageDialog);
    } // setupUi

    void retranslateUi(QDialog *QExchangeDateMangageDialog)
    {
        QExchangeDateMangageDialog->setWindowTitle(QApplication::translate("QExchangeDateMangageDialog", "Dialog", 0));
        AddBtn->setText(QApplication::translate("QExchangeDateMangageDialog", ">>>", 0));
        DelBtn->setText(QApplication::translate("QExchangeDateMangageDialog", "<<<", 0));
        CloseBtn->setText(QApplication::translate("QExchangeDateMangageDialog", "\345\205\263\351\227\255", 0));
    } // retranslateUi

};

namespace Ui {
    class QExchangeDateMangageDialog: public Ui_QExchangeDateMangageDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QEXCHANGEDATEMANGAGEDIALOG_H
