#ifndef HASHTOOL_H
#define HASHTOOL_H

#include <QtWidgets/QWidget>
#include "ui_hashtool.h"

class HashTool : public QWidget
{
	Q_OBJECT

public:
    HashTool(QWidget *parent = nullptr);
    ~HashTool();

private slots:

    void on_rbtMD5_clicked(bool checked);

    void on_rbSHA1_clicked(bool checked);

    void on_rbSHA256_clicked(bool checked);

    void on_btnFile_clicked();

    void on_btnClear_clicked();

    void on_btnCheckUpdate_clicked();

    void on_edtText_textChanged();

    void on_btnCopy_clicked();

    void on_btnPaste_clicked();

    void on_btnCopyInfo_clicked();

    void on_btnSaveInfo_clicked();

    void on_btnCompare_clicked();

    void on_btnBigen_clicked();


private:
    bool bTextHash = true; //为true时是求文本内容的HASH值，为flase是求文件的HASH值
    int8_t hashtype = 0;   //哈希求值类型 ———— 0: MD5; 1:SHA1; 2:SHA256
    QString filename;      //打开文件的文件名，当他IsEmpty时，不进行文件Hash值的计算
    Ui::HashToolClass ui;

};

#endif // HASHTOOL_H
