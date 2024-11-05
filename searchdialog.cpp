#include "searchdialog.h"
#include "ui_searchdialog.h"
#include <QMessageBox>

SearchDialog::SearchDialog(QWidget *parent,QPlainTextEdit *textEdit)
    : QDialog(parent)
    , ui(new Ui::SearchDialog)
{
    ui->setupUi(this);

    pTextEdit=textEdit;
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

//下一个查找
void SearchDialog::on_btnNext_clicked()
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


void SearchDialog::on_btnCancel_clicked()
{
    accept();
}

