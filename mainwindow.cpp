#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QColorDialog>
#include <QFontDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    textChanged=false;
    on_actionNew_triggered();
    statusLabel.setMaximumWidth(200);
    statusLabel.setText("length："+QString::number(0)+"    lines："+QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(150);
    statusCursorLabel.setText("Ln："+QString::number(0)+"    Col："+QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author=new QLabel(ui->statusbar);
    author->setText("沈荣杰");
    ui->statusbar->addPermanentWidget(author);

    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionPaste->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actiontoolsbar->setChecked(true);
    ui->actiontoolsbar->setChecked(true);
    ui->actionShowLineNumber->setChecked(true);

    QPlainTextEdit::LineWrapMode mode = ui->textEditor->lineWrapMode();

    if(mode == QTextEdit::NoWrap){
        ui->textEditor->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->actionLineWrap->setChecked(false);
    }else{
        ui->textEditor->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionLineWrap->setChecked(true);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dig;
    dig.exec();
}


void MainWindow::on_actionFind_triggered()
{
    SearchDialog dig(this,ui->textEditor);
    dig.exec();
}


void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dig(this,ui->textEditor);
    dig.exec();

}

//新建文本
void MainWindow::on_actionNew_triggered()
{
    if(!userEdiitConfirmed())
        return;

    filePath="";

    ui->textEditor->clear();
    this->setWindowTitle(tr("新建文本文件 - 编辑器"));

    textChanged=false;
}

//打开文本
void MainWindow::on_actionOpen_triggered()
{
    if(!userEdiitConfirmed())
        return;

    QString filename =QFileDialog::getOpenFileName(this,"打开文件",".",tr("Text files (*.txt) ;; All (*.*)"));
    QFile file(filename);

    if(!file.open(QFile::ReadOnly| QFile::Text))
    {
        return;
    }

    filePath = filename;

    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEditor->insertPlainText(text);
    file.close();

    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());

    textChanged=false;
}

//保存文本
void MainWindow::on_actionSave_triggered()
{
    if(filePath==""){
        QString filename =QFileDialog::getSaveFileName(this,"保存文件",".",tr("Text files (*.txt) "));
        QFile file(filename);
        if(!file.open(QFile::WriteOnly| QFile::Text)){
            return;
        }
        filePath = filename;
        file.close();
    }

    QFile file(filePath);
    if(!file.open(QFile::WriteOnly| QFile::Text)){
        return;
    }

    QTextStream out(&file);
    QString text =ui->textEditor->toPlainText();
    out<<text;
    file.flush();
    file.close();
    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());
    textChanged = false;
}

//另存为
void MainWindow::on_actionSaveAs_triggered()
{
    QString filename =QFileDialog::getSaveFileName(this,"保存文件",".",tr("Text files (*.txt) "));

    QFile file(filename);
    if(!file.open(QFile::WriteOnly| QFile::Text)){
        return;
    }

    filePath = filename;
    QTextStream out(&file);
    QString text =ui->textEditor->toPlainText();
    out<<text;
    file.flush();
    file.close();
    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());
    textChanged = false;
}


void MainWindow::on_textEditor_textChanged()
{
    if(!textChanged)
    {
        this->setWindowTitle("*"+this->windowTitle());
        textChanged=true;
    }
    statusLabel.setText("length："+QString::number(ui->textEditor->toPlainText().length())+
                        "    lines："+QString::number(ui->textEditor->document()->lineCount()));
}

bool MainWindow::userEdiitConfirmed()
{
    if(textChanged){

        QString path = (filePath != "") ? filePath : "无标题.txt";

        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Question);
        msg.setWindowTitle("...");
        msg.setWindowFlag(Qt::Drawer);
        msg.setText(QString("是否将更改保存到\n") + "\"" + path + "\" ?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        int r = msg.exec();
        switch (r) {
        case QMessageBox::Yes:
            on_actionSave_triggered();
            break;
        case QMessageBox::No:
            textChanged=false;
            break;
        case QMessageBox::Cancel:
            return false;
        }
    }

    return true;
}

//撤销
void MainWindow::on_actionUndo_triggered()
{
    ui->textEditor->undo();
}

//恢复
void MainWindow::on_actionRedo_triggered()
{
    ui->textEditor->redo();
}

//剪切
void MainWindow::on_actionCut_triggered()
{
    ui->textEditor->cut();
    ui->actionPaste->setEnabled(true);
}

//复制
void MainWindow::on_actionCopy_triggered()
{
    ui->textEditor->copy();
    ui->actionPaste->setEnabled(true);
}

//粘贴
void MainWindow::on_actionPaste_triggered()
{
    ui->textEditor->paste();
}

//全选
void MainWindow::on_actionSelectAll_triggered()
{
    ui->textEditor->selectAll();
}

//可否复制剪切
void MainWindow::on_textEditor_copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);
}

//可否恢复
void MainWindow::on_textEditor_redoAvailable(bool b)
{
    ui->actionRedo->setEnabled(b);
}

//可否撤销
void MainWindow::on_textEditor_undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);
}

//设置字体
void MainWindow::on_actionFont_triggered()
{
    bool ok=false;
    QFont font = QFontDialog::getFont(&ok,this);

    if(ok){
        ui->textEditor->setFont(font);
    }
}

//设置字体颜色
void MainWindow::on_actionFontColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(color.isValid()){
        ui->textEditor->setStyleSheet(QString("QPlainTextEdit {color: %1}").arg(color.name()));
    }
}

//设置编辑器背景色
void MainWindow::on_actionBackgroundColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(color.isValid()){
        ui->textEditor->setStyleSheet(QString("QPlainTextEdit {background-color: %1}").arg(color.name()));
    }
}

//自动换行
void MainWindow::on_actionLineWrap_triggered()
{
    QPlainTextEdit::LineWrapMode mode = ui->textEditor->lineWrapMode();

    if(mode == QTextEdit::NoWrap){
        ui->textEditor->setLineWrapMode(QPlainTextEdit::WidgetWidth);

        ui->actionLineWrap->setChecked(true);
    }else{
        ui->textEditor->setLineWrapMode(QPlainTextEdit::NoWrap);

        ui->actionLineWrap->setChecked(false);
    }
}

void MainWindow::on_actionFontBackgroundColor_triggered()
{

}

//显示工具栏
void MainWindow::on_actiontoolsbar_triggered()
{
    bool visable = ui->toolBar->isVisible();
    ui->toolBar->setVisible(!visable);
    ui->actiontoolsbar->setChecked(!visable);
}

//显示状态栏
void MainWindow::on_actionStatebar_triggered()
{
    bool visable = ui->statusbar->isVisible();
    ui->statusbar->setVisible(!visable);
    ui->actionStatebar->setChecked(!visable);
}


void MainWindow::on_actionExit_triggered()
{
    if(userEdiitConfirmed())
        exit(0);
}

//状态栏更新
void MainWindow::on_textEditor_cursorPositionChanged()
{
    int col=0;
    int ln=0;
    int flg=-1;
    int pos = ui->textEditor->textCursor().position();

    QString text = ui->textEditor->toPlainText();

    for(int i=0;i<pos;i++)
    {
        if(text[i]=='\n'){
            ln++;
            flg= i;
        }
    }

    flg++;
    col=pos-flg;

    statusCursorLabel.setText("Ln："+QString::number(ln+1)+"    Col："+QString::number(col+1));
}


void MainWindow::on_actionShowLineNumber_triggered()
{
    ui->textEditor->hideLineNumberArea();
}

