#include "replacedialog.h"
#include "ui_replacedialog.h"
#include <QMessageBox>

ReplaceDialog::ReplaceDialog(QWidget *parent,QPlainTextEdit *textEdit)
    : QDialog(parent)
    , ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);

    pTextEdit=textEdit;
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

//下一个查找
void ReplaceDialog::on_btnNext_clicked()
{
    QString target = ui->lineEditFind->text();
    if(target==""||pTextEdit==nullptr)
        return;

    QString text = pTextEdit->toPlainText();
    QTextCursor c = pTextEdit->textCursor();
    int index=-1;

    if(ui->rbDown->isChecked()){
        index = text.indexOf(target,c.position(),
                             ui->cbCase->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

        if(index>=0){
            c.setPosition(index);
            c.setPosition(index + target.length(),QTextCursor::KeepAnchor);

            pTextEdit->setTextCursor(c);
        }
    }else{
        index = text.lastIndexOf(target,c.position()-target.length()-1,
                                 ui->cbCase->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

        if(index>=0){
            c.setPosition(index);
            c.setPosition(index + target.length(),QTextCursor::KeepAnchor);

            pTextEdit->setTextCursor(c);
        }
    }

    if(index<0){
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Question);
        msg.setWindowTitle("提示");
        msg.setText("找不到 "+target);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
}

//替换
void ReplaceDialog::on_btnReplace_clicked()
{
    QString target = ui->lineEditFind->text();
    QString to = ui->lineEditReplace->text();
    QString text = pTextEdit->toPlainText();

    QString selText = pTextEdit->textCursor().selectedText();

    if(selText==target)
        pTextEdit->insertPlainText(to);
}

//全部替换
void ReplaceDialog::on_btnReplaceAll_clicked()
{
    QString target = ui->lineEditFind->text();
    QString to = ui->lineEditReplace->text();

    if(pTextEdit!=nullptr &&  (target!="") && (to!="")){
        QString text = pTextEdit->toPlainText();
        text.replace(target,to,ui->cbCase->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

        pTextEdit->clear();

        pTextEdit->insertPlainText(text);
    }

}


void ReplaceDialog::on_btnCancel_clicked()
{
    accept();
}

