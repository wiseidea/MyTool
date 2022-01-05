
#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDateTime>
#include <QClipboard>
#include <QFileInfo>
#include <QTextStream>
#include "hashtool.h"
#include <QCryptographicHash>

HashTool::HashTool(QWidget *parent)
	: QWidget(parent)
{
    ui.setupUi(this);      
}

HashTool::~HashTool()
{

}

void HashTool::on_rbtMD5_clicked(bool checked)
{
    if (checked) hashtype = 0;    //"MD5"
    on_btnBigen_clicked();
}

void HashTool::on_rbSHA1_clicked(bool checked)
{
    if (checked) hashtype = 1;    //"SHA1"
    on_btnBigen_clicked();
}

void HashTool::on_rbSHA256_clicked(bool checked)
{
    if (checked) hashtype = 2;    //"SHA256"
    on_btnBigen_clicked();
}

void HashTool::on_btnFile_clicked()
{
    filename = QFileDialog::getOpenFileName(this,"打开","../");
    QFileInfo fileinfo(filename);

    if (!filename.isEmpty())
    {
        QString infoList(QString("文件名：%1").arg(filename));

        infoList.append(QString("\n文件大小：%1 Bytes").arg(fileinfo.size()));
        infoList.append(QString("\n创建时间：%1").arg(fileinfo.birthTime().toString("yyyy年MM月dd日 hh:mm:ss")));

        ui.edtText->setText(infoList);
        ui.edtHash->clear();
        ui.edtContrast->clear();
        ui.lblText->setText("文件");
        bTextHash = false;
        ui.edtText->setReadOnly(true);
    }
}

void HashTool::on_btnClear_clicked()
{
    filename.clear();
    ui.edtText->clear();
    ui.edtHash->clear();
    ui.edtContrast->clear();
    ui.edtText->setReadOnly(false);
    ui.lblText->setText("信息");
}

void HashTool::on_edtText_textChanged()
{
    if(!ui.edtText->toPlainText().isEmpty())
    {
        ui.lblText->setText("文本");
        bTextHash = true;
    }
    else
    {
        ui.lblText->setText("信息");
        ui.edtHash->clear();
    }
}

void HashTool::on_btnCopy_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();

    clipboard->setText(ui.edtHash->text());

}

void HashTool::on_btnPaste_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();

    ui.edtContrast->setText(clipboard->text());
}

void HashTool::on_btnCopyInfo_clicked()
{
    QVector<QString> hashMap(20);  //用来保存键值对：0=MD5，1=SHA1，2=SHA256;
    hashMap[0] = "MD5"; hashMap[1] = "SHA1"; hashMap[2] = "SHA256";

    QClipboard *clipboard = QApplication::clipboard();

    QString cinfo(ui.edtText->toPlainText());
    cinfo.append(QString::fromLatin1("\n哈希值(%1)：%2").arg(hashMap[hashtype]).arg(ui.edtHash->text()));

    clipboard->setText(cinfo);
}

void HashTool::on_btnSaveInfo_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            "保存",
            "",
            "文本文件 (*.txt);;所有文件 (*.*)");

    if (!fileName.isEmpty())
    {
        QVector<QString> hashMap(64);  //用来保存键值对：0=MD5，1=SHA1，2=SHA256;
        hashMap[0] = "MD5"; hashMap[1] = "SHA1"; hashMap[2] = "SHA256";

        QFile textfile(fileName);
        if (textfile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
           QTextStream outputfile(&textfile);
           outputfile << ui.edtText->toPlainText() << Qt::endl;
           outputfile << QString("\n哈希值(%1)：%2").arg(hashMap[hashtype]).arg(ui.edtHash->text()) << Qt::endl;
           textfile.close();
        }
    }
}

void HashTool::on_btnCompare_clicked()
{
    if ((!ui.edtHash->text().isEmpty()) & (!ui.edtContrast->text().isEmpty()) &
            (ui.edtHash->text().compare(ui.edtContrast->text()) == 0))

        QMessageBox::information(this, "比较结果", "恭喜：哈希值相同！");

    else

        QMessageBox::warning(this, "比较结果", "注意：哈希值不相同！");
}

void HashTool::on_btnCheckUpdate_clicked()
{
    /*经过查看QmessageBox的帮助文档，里面的Static Public Members，查看里面的about函数的使用方法
     * 从Static Public Members得知其是静态的成员函数，这里就涉及到调用类里面的静态的成员函数。即通过类名调用；
     * 最后的调用方式如下：
     * QMessageBox::about(this,"about","关于QT");
    */
    QMessageBox::about(this, "关于", "一个计算文本或文件哈希值的工具！");
}

QByteArray getFileHash(QString filePath, int8_t ihashtype)
{
    QFile localFile(filePath);

    if (localFile.open(QFile::ReadOnly))
    {
        quint64 totalBytes = 0;
        quint64 bytesToWrite = 0;
        quint64 loadSize = 1024 * 4;
        QByteArray buf, baHash;

        totalBytes = localFile.size();
        //bytesToWrite = totalBytes;

        switch(ihashtype)
        {
            case 0:              //MD5
            {
                QCryptographicHash ch(QCryptographicHash::Md5);
                for (bytesToWrite = totalBytes; bytesToWrite > 0;)
                {
                    buf = localFile.read(qMin(bytesToWrite, loadSize));
                    ch.addData(buf);
                    bytesToWrite -= buf.length();
                    buf.resize(0);
                }
                baHash = ch.result();
            }
            break;
            case 1:              //SHA1
            {
                QCryptographicHash ch(QCryptographicHash::Sha1);
                for (bytesToWrite = totalBytes; bytesToWrite > 0;)
                {
                    buf = localFile.read(qMin(bytesToWrite, loadSize));
                    ch.addData(buf);
                    bytesToWrite -= buf.length();
                    buf.resize(0);
                }
                baHash = ch.result();
            }
            break;
            case 2:               //SHA256
            {
                QCryptographicHash ch(QCryptographicHash::Sha512);
                for (bytesToWrite = totalBytes; bytesToWrite > 0;)
                {
                    buf = localFile.read(qMin(bytesToWrite, loadSize));
                    ch.addData(buf);
                    bytesToWrite -= buf.length();
                    buf.resize(0);
                }
                baHash = ch.result();
            }

        }

        localFile.close();
        return baHash;
    }
    else {
        return nullptr;
    }

}

void HashTool::on_btnBigen_clicked()
{
    if (!(ui.edtText->toPlainText().isEmpty()))
    {
        switch(hashtype)
        {
            case 0:              //MD5
                if (bTextHash)
                {
                    char * buf;

                    //将输入框的文本转化为char *

                    QByteArray ba = ui.edtText->toPlainText().toLatin1();
                    buf = ba.data();

                    int len = strlen(buf);

                    QCryptographicHash ch(QCryptographicHash::Md5);
                    ch.addData(buf, len);
                    QString md5 = QString::fromLatin1(ch.result().toHex());
                    ui.edtHash->setText(md5.toUpper());
                }
                else{
                    QString val = QString::fromStdString(getFileHash(filename, hashtype).toHex().toStdString());
                    if(!filename.isEmpty()) ui.edtHash->setText(val.toUpper());
                }
                break;
            case 1:                   //SHA1
               if (bTextHash)
               {
                   char * buf;
                   QByteArray ba = ui.edtText->toPlainText().toLatin1();
                   buf = ba.data();
                   int len = strlen(buf);
                   QCryptographicHash ch(QCryptographicHash::Sha1);
                   ch.addData(buf, len);
                   QString md5 = QString::fromLatin1(ch.result().toHex());
                   ui.edtHash->setText(md5.toUpper());
               }
              else{
                   QString val = QString::fromStdString(getFileHash(filename, hashtype).toHex().toStdString());
                   if(!filename.isEmpty()) ui.edtHash->setText(val.toUpper());
               }
               break;
            case 2:                 //SHA256
               if (bTextHash)
               {
                   char * buf;
                   QByteArray ba = ui.edtText->toPlainText().toLatin1();
                   buf = ba.data();
                   int len = strlen(buf);
                   QCryptographicHash ch(QCryptographicHash::Sha256);
                   ch.addData(buf, len);
                   QString md5 = QString::fromLatin1(ch.result().toHex());
                   ui.edtHash->setText(md5.toUpper());
               }
               else{
                   QString val = QString::fromStdString(getFileHash(filename, hashtype).toHex().toStdString());
                   if(!filename.isEmpty()) ui.edtHash->setText(val.toUpper());
               }
               break;
          }
    }
}

