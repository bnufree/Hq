#include "qexchangefiledlg.h"
#include "ui_qexchangefiledlg.h"
#include <QFileDialog>

QExchangeFileDlg::QExchangeFileDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QExchangeFileDlg)
{
    ui->setupUi(this);
}

QExchangeFileDlg::~QExchangeFileDlg()
{
    delete ui;
}

void QExchangeFileDlg::on_importBtn_clicked()
{
//    QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("选择交易记录文件"), "", tr("Files (*.xls *.xlsx *.csv)"));
//    if (fileName.isEmpty()) return;
//    QFile file;
//    file.setFileName(sOpenFileName);
//    if(!file.open(QIODevice::ReadOnly))
//    {
//        QMessageBox::warning(this, QStringLiteral("错误"),QStringLiteral("打开文件失败！"));
//        return;
//    }
//    if (sOpenFileName == m_sFilePathName)
//    {
//        return;
//    }
//    m_sFilePathName = sOpenFileName;
//    ui->fileLineEdit->setText(sOpenFileName);
//    m_bSendFileOver = false;

//    QString sFileSuffix = sOpenFileName.right(3);
//    int nFileFlag = 0;
//    if (sFileSuffix == "dwg" || sFileSuffix == "DWG")
//    {
//        nFileFlag = 0;//dwg 文件
//    }
//    else
//    {
//        nFileFlag = 1;//dxf 文件
//    }

//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));//解决中文有乱码
//    QByteArray ba = sOpenFileName.toLocal8Bit();
//    char *pFileName=ba.data();
//   // const char *pFileName = sOpenFileName.toStdString().c_str();
//    if (!m_pDWGFileShowView->analysisDwgFile(pFileName, nFileFlag))
//    {
//        QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("解析CAD数据失败！"));

//        return;
//    }
//    layerSet();
}
