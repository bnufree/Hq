/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MainDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *topframe;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *shlbl;
    QLabel *label_2;
    QLabel *szlbl;
    QLabel *label_3;
    QLabel *hslbl;
    QLabel *label_4;
    QLabel *cyblbl;
    QLabel *label_5;
    QLabel *dpglbl;
    QSpacerItem *horizontalSpacer;
    QPushButton *minBtn;
    QPushButton *closeBtn;
    QFrame *midframe;
    QVBoxLayout *verticalLayout_2;
    QFrame *btnframe;
    QHBoxLayout *horizontalLayout_2;
    QTableWidget *blocktbl;
    QTableWidget *hqtbl;
    QFrame *hqframe;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *zxgBtn;
    QPushButton *hqcenterBtn;
    QPushButton *blkbtn;
    QPushButton *zjlxBtn;
    QPushButton *lhbBtn;
    QPushButton *DateMgrBtn;
    QLineEdit *searchTxt;
    QLabel *updatelbl;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *MainDialog)
    {
        if (MainDialog->objectName().isEmpty())
            MainDialog->setObjectName(QStringLiteral("MainDialog"));
        MainDialog->resize(850, 300);
        MainDialog->setMinimumSize(QSize(350, 300));
        MainDialog->setMaximumSize(QSize(16777215, 16777215));
        MainDialog->setStyleSheet(QString::fromUtf8("QTableView{\n"
"color:black;\n"
"}\n"
"\n"
"/*QMenu {\n"
"/*    background-color: white; /* sets background of the menu \350\256\276\347\275\256\346\225\264\344\270\252\350\217\234\345\215\225\345\214\272\345\237\237\347\232\204\350\203\214\346\231\257\350\211\262\357\274\214\346\210\221\347\224\250\347\232\204\346\230\257\347\231\275\350\211\262\357\274\232white*/\n"
"/*     border: 1px solid white;/*\346\225\264\344\270\252\350\217\234\345\215\225\345\214\272\345\237\237\347\232\204\350\276\271\346\241\206\347\262\227\347\273\206\343\200\201\346\240\267\345\274\217\343\200\201\351\242\234\350\211\262*/\n"
"/* }\n"
" \n"
" QMenu::item {\n"
"     /* sets background of menu item. set this to something non-transparent\n"
"         if you want menu color and menu item color to be different */\n"
"/*     background-color: transparent;\n"
"     padding:8px 8px;/*\350\256\276\347\275\256\350\217\234\345\215\225\351\241\271\346\226\207\345\255\227\344\270\212\344\270\213\345\222\214\345\267\246\345\217\263\347\232\204"
                        "\345\206\205\350\276\271\350\267\235\357\274\214\346\225\210\346\236\234\345\260\261\346\230\257\350\217\234\345\215\225\344\270\255\347\232\204\346\235\241\347\233\256\345\267\246\345\217\263\344\270\212\344\270\213\346\234\211\344\272\206\351\227\264\351\232\224*/\n"
"/*     margin:0px 1px;/*\350\256\276\347\275\256\350\217\234\345\215\225\351\241\271\347\232\204\345\244\226\350\276\271\350\267\235*/\n"
"/*     border-bottom:1px solid #DBDBDB;/*\344\270\272\350\217\234\345\215\225\351\241\271\344\271\213\351\227\264\346\267\273\345\212\240\346\250\252\347\272\277\351\227\264\351\232\224*/\n"
" /*}\n"
" \n"
"/* QMenu::item:selected { /* when user selects item using mouse or keyboard */\n"
" /*    background-color: #2dabf9;/*\350\277\231\344\270\200\345\217\245\346\230\257\350\256\276\347\275\256\350\217\234\345\215\225\351\241\271\351\274\240\346\240\207\347\273\217\350\277\207\351\200\211\344\270\255\347\232\204\346\240\267\345\274\217*/\n"
" /*}*/\n"
"\n"
"QMenu { \n"
"background-color: #1c5388; \n"
"border"
                        ": 1px solid #afb2b5; \n"
"color: #FFFFFF; \n"
"font: 75 10pt \\345\\276\\256\\350\\275\\257\\351\\233\\205\\351\\273\\221; \n"
" }\n"
"QMenu::item:selected { \n"
"border-color: darkblue; \n"
"background: #ff8400;   \n"
"} \n"
"QMenu::item { height:24px }"));
        verticalLayout = new QVBoxLayout(MainDialog);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        topframe = new QFrame(MainDialog);
        topframe->setObjectName(QStringLiteral("topframe"));
        topframe->setFrameShape(QFrame::StyledPanel);
        topframe->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(topframe);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, -1, 0);
        label = new QLabel(topframe);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        shlbl = new QLabel(topframe);
        shlbl->setObjectName(QStringLiteral("shlbl"));

        horizontalLayout->addWidget(shlbl);

        label_2 = new QLabel(topframe);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        szlbl = new QLabel(topframe);
        szlbl->setObjectName(QStringLiteral("szlbl"));

        horizontalLayout->addWidget(szlbl);

        label_3 = new QLabel(topframe);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout->addWidget(label_3);

        hslbl = new QLabel(topframe);
        hslbl->setObjectName(QStringLiteral("hslbl"));

        horizontalLayout->addWidget(hslbl);

        label_4 = new QLabel(topframe);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout->addWidget(label_4);

        cyblbl = new QLabel(topframe);
        cyblbl->setObjectName(QStringLiteral("cyblbl"));

        horizontalLayout->addWidget(cyblbl);

        label_5 = new QLabel(topframe);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout->addWidget(label_5);

        dpglbl = new QLabel(topframe);
        dpglbl->setObjectName(QStringLiteral("dpglbl"));

        horizontalLayout->addWidget(dpglbl);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        minBtn = new QPushButton(topframe);
        minBtn->setObjectName(QStringLiteral("minBtn"));

        horizontalLayout->addWidget(minBtn);

        closeBtn = new QPushButton(topframe);
        closeBtn->setObjectName(QStringLiteral("closeBtn"));

        horizontalLayout->addWidget(closeBtn);


        verticalLayout->addWidget(topframe);

        midframe = new QFrame(MainDialog);
        midframe->setObjectName(QStringLiteral("midframe"));
        midframe->setFrameShape(QFrame::StyledPanel);
        midframe->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(midframe);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(9, 0, 9, 0);
        btnframe = new QFrame(midframe);
        btnframe->setObjectName(QStringLiteral("btnframe"));
        btnframe->setFrameShape(QFrame::StyledPanel);
        btnframe->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(btnframe);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        blocktbl = new QTableWidget(btnframe);
        if (blocktbl->columnCount() < 2)
            blocktbl->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        blocktbl->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        blocktbl->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        blocktbl->setObjectName(QStringLiteral("blocktbl"));
        blocktbl->setMaximumSize(QSize(115, 16777215));
        blocktbl->setContextMenuPolicy(Qt::CustomContextMenu);
        blocktbl->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        blocktbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
        blocktbl->setAlternatingRowColors(true);
        blocktbl->setSelectionMode(QAbstractItemView::SingleSelection);
        blocktbl->setSelectionBehavior(QAbstractItemView::SelectRows);

        horizontalLayout_2->addWidget(blocktbl);

        hqtbl = new QTableWidget(btnframe);
        if (hqtbl->columnCount() < 14)
            hqtbl->setColumnCount(14);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        hqtbl->setHorizontalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        hqtbl->setHorizontalHeaderItem(1, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        hqtbl->setHorizontalHeaderItem(2, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        hqtbl->setHorizontalHeaderItem(3, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        hqtbl->setHorizontalHeaderItem(4, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        hqtbl->setHorizontalHeaderItem(5, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        hqtbl->setHorizontalHeaderItem(6, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        hqtbl->setHorizontalHeaderItem(7, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        hqtbl->setHorizontalHeaderItem(8, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        hqtbl->setHorizontalHeaderItem(9, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        hqtbl->setHorizontalHeaderItem(10, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        hqtbl->setHorizontalHeaderItem(11, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        hqtbl->setHorizontalHeaderItem(12, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        hqtbl->setHorizontalHeaderItem(13, __qtablewidgetitem15);
        hqtbl->setObjectName(QStringLiteral("hqtbl"));
        hqtbl->setMouseTracking(true);
        hqtbl->setContextMenuPolicy(Qt::CustomContextMenu);
        hqtbl->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        hqtbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
        hqtbl->setAlternatingRowColors(true);
        hqtbl->setSelectionMode(QAbstractItemView::SingleSelection);
        hqtbl->setSelectionBehavior(QAbstractItemView::SelectRows);
        hqtbl->horizontalHeader()->setCascadingSectionResizes(true);
        hqtbl->horizontalHeader()->setMinimumSectionSize(20);
        hqtbl->verticalHeader()->setVisible(false);

        horizontalLayout_2->addWidget(hqtbl);

        horizontalLayout_2->setStretch(0, 3);
        horizontalLayout_2->setStretch(1, 16);

        verticalLayout_2->addWidget(btnframe);

        hqframe = new QFrame(midframe);
        hqframe->setObjectName(QStringLiteral("hqframe"));
        hqframe->setMinimumSize(QSize(0, 30));
        hqframe->setMaximumSize(QSize(16777215, 30));
        hqframe->setFrameShape(QFrame::StyledPanel);
        hqframe->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(hqframe);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        zxgBtn = new QPushButton(hqframe);
        zxgBtn->setObjectName(QStringLiteral("zxgBtn"));
        zxgBtn->setMinimumSize(QSize(0, 30));
        zxgBtn->setFlat(true);

        horizontalLayout_3->addWidget(zxgBtn);

        hqcenterBtn = new QPushButton(hqframe);
        hqcenterBtn->setObjectName(QStringLiteral("hqcenterBtn"));
        hqcenterBtn->setMinimumSize(QSize(0, 30));
        hqcenterBtn->setFlat(true);

        horizontalLayout_3->addWidget(hqcenterBtn);

        blkbtn = new QPushButton(hqframe);
        blkbtn->setObjectName(QStringLiteral("blkbtn"));
        blkbtn->setMinimumSize(QSize(0, 30));
        blkbtn->setFlat(true);

        horizontalLayout_3->addWidget(blkbtn);

        zjlxBtn = new QPushButton(hqframe);
        zjlxBtn->setObjectName(QStringLiteral("zjlxBtn"));
        zjlxBtn->setMinimumSize(QSize(0, 30));
        zjlxBtn->setFlat(true);

        horizontalLayout_3->addWidget(zjlxBtn);

        lhbBtn = new QPushButton(hqframe);
        lhbBtn->setObjectName(QStringLiteral("lhbBtn"));
        lhbBtn->setMinimumSize(QSize(0, 30));
        lhbBtn->setFlat(true);

        horizontalLayout_3->addWidget(lhbBtn);

        DateMgrBtn = new QPushButton(hqframe);
        DateMgrBtn->setObjectName(QStringLiteral("DateMgrBtn"));
        DateMgrBtn->setFlat(true);

        horizontalLayout_3->addWidget(DateMgrBtn);

        searchTxt = new QLineEdit(hqframe);
        searchTxt->setObjectName(QStringLiteral("searchTxt"));
        searchTxt->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_3->addWidget(searchTxt);

        updatelbl = new QLabel(hqframe);
        updatelbl->setObjectName(QStringLiteral("updatelbl"));
        updatelbl->setMinimumSize(QSize(120, 0));

        horizontalLayout_3->addWidget(updatelbl);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout_2->addWidget(hqframe);


        verticalLayout->addWidget(midframe);


        retranslateUi(MainDialog);

        QMetaObject::connectSlotsByName(MainDialog);
    } // setupUi

    void retranslateUi(QDialog *MainDialog)
    {
        MainDialog->setWindowTitle(QApplication::translate("MainDialog", "Dialog", 0));
        label->setText(QApplication::translate("MainDialog", "\344\270\212\350\257\201\357\274\232", 0));
        shlbl->setText(QApplication::translate("MainDialog", "10000.00 +10.00%", 0));
        label_2->setText(QApplication::translate("MainDialog", "   \346\267\261\350\257\201\357\274\232", 0));
        szlbl->setText(QApplication::translate("MainDialog", "10000.00 +10.00%", 0));
        label_3->setText(QApplication::translate("MainDialog", "   \346\262\252\346\267\261300\357\274\232", 0));
        hslbl->setText(QApplication::translate("MainDialog", "10000.00 +10.00%", 0));
        label_4->setText(QApplication::translate("MainDialog", "  \345\210\233\344\270\232\346\235\277\357\274\232", 0));
        cyblbl->setText(QApplication::translate("MainDialog", "10000.00 +10.00%", 0));
        label_5->setText(QApplication::translate("MainDialog", "   \345\244\247\347\233\230\350\202\241\357\274\232", 0));
        dpglbl->setText(QApplication::translate("MainDialog", "10000.00 +10.00%", 0));
        minBtn->setText(QString());
        closeBtn->setText(QString());
        QTableWidgetItem *___qtablewidgetitem = blocktbl->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainDialog", "\345\220\215\347\247\260", 0));
        QTableWidgetItem *___qtablewidgetitem1 = blocktbl->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainDialog", "\346\266\250\350\267\214\345\271\205", 0));
        QTableWidgetItem *___qtablewidgetitem2 = hqtbl->horizontalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("MainDialog", "\344\273\243\347\240\201", 0));
        QTableWidgetItem *___qtablewidgetitem3 = hqtbl->horizontalHeaderItem(1);
        ___qtablewidgetitem3->setText(QApplication::translate("MainDialog", "\345\220\215\347\247\260", 0));
        QTableWidgetItem *___qtablewidgetitem4 = hqtbl->horizontalHeaderItem(2);
        ___qtablewidgetitem4->setText(QApplication::translate("MainDialog", "\347\216\260\344\273\267", 0));
        QTableWidgetItem *___qtablewidgetitem5 = hqtbl->horizontalHeaderItem(3);
        ___qtablewidgetitem5->setText(QApplication::translate("MainDialog", "\346\266\250\350\267\214\345\271\205", 0));
        QTableWidgetItem *___qtablewidgetitem6 = hqtbl->horizontalHeaderItem(4);
        ___qtablewidgetitem6->setText(QApplication::translate("MainDialog", "\346\210\220\344\272\244", 0));
        QTableWidgetItem *___qtablewidgetitem7 = hqtbl->horizontalHeaderItem(5);
        ___qtablewidgetitem7->setText(QApplication::translate("MainDialog", "\350\265\204\351\207\221\346\257\224", 0));
        QTableWidgetItem *___qtablewidgetitem8 = hqtbl->horizontalHeaderItem(6);
        ___qtablewidgetitem8->setText(QApplication::translate("MainDialog", "3\346\227\245", 0));
        QTableWidgetItem *___qtablewidgetitem9 = hqtbl->horizontalHeaderItem(7);
        ___qtablewidgetitem9->setText(QApplication::translate("MainDialog", "\350\265\204\351\207\221\346\265\201", 0));
        QTableWidgetItem *___qtablewidgetitem10 = hqtbl->horizontalHeaderItem(8);
        ___qtablewidgetitem10->setText(QApplication::translate("MainDialog", "\350\202\241\346\201\257\347\216\207", 0));
        QTableWidgetItem *___qtablewidgetitem11 = hqtbl->horizontalHeaderItem(9);
        ___qtablewidgetitem11->setText(QApplication::translate("MainDialog", "\351\200\201\350\275\254", 0));
        QTableWidgetItem *___qtablewidgetitem12 = hqtbl->horizontalHeaderItem(10);
        ___qtablewidgetitem12->setText(QApplication::translate("MainDialog", "\347\231\273\350\256\260\346\227\245", 0));
        QTableWidgetItem *___qtablewidgetitem13 = hqtbl->horizontalHeaderItem(11);
        ___qtablewidgetitem13->setText(QApplication::translate("MainDialog", "\346\234\200\351\253\230", 0));
        QTableWidgetItem *___qtablewidgetitem14 = hqtbl->horizontalHeaderItem(12);
        ___qtablewidgetitem14->setText(QApplication::translate("MainDialog", "\346\234\200\344\275\216", 0));
        QTableWidgetItem *___qtablewidgetitem15 = hqtbl->horizontalHeaderItem(13);
        ___qtablewidgetitem15->setText(QApplication::translate("MainDialog", "\345\205\254\345\221\212\346\227\245", 0));
        zxgBtn->setText(QApplication::translate("MainDialog", "\350\207\252\351\200\211\350\202\241", 0));
        hqcenterBtn->setText(QApplication::translate("MainDialog", "\350\241\214\346\203\205\344\270\255\345\277\203", 0));
        blkbtn->setText(QApplication::translate("MainDialog", "\346\235\277\345\235\227", 0));
        zjlxBtn->setText(QApplication::translate("MainDialog", "\350\265\204\351\207\221\346\265\201\345\220\221", 0));
        lhbBtn->setText(QApplication::translate("MainDialog", "\351\276\231\350\231\216\346\246\234", 0));
        DateMgrBtn->setText(QApplication::translate("MainDialog", "\346\227\245\346\234\237\347\256\241\347\220\206", 0));
        searchTxt->setPlaceholderText(QApplication::translate("MainDialog", "\344\273\243\347\240\201\346\213\274\351\237\263", 0));
        updatelbl->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainDialog: public Ui_MainDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
