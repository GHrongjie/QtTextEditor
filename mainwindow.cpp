#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    textChanged=false;
    on_actionNew_triggered();
    statusLabel.setMaximumWidth(150);
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
    SearchDialog dig;
    dig.exec();
}


void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dig;
    dig.exec();

}


void MainWindow::on_actionNew_triggered()
{
    if(!userEdiitConfirmed())
        return;

    filePath="";

    ui->textEditor->clear();
    this->setWindowTitle(tr("新建文本文件 - 编辑器"));

    textChanged=false;
}


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

    QTextStream out(&file);
    QString text =ui->textEditor->toPlainText();
    out<<text;
    file.flush();
    file.close();
    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());
    textChanged = false;
}


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


void MainWindow::on_actionUndo_triggered()
{
    ui->textEditor->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEditor->redo();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEditor->cut();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEditor->copy();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEditor->paste();
}


void MainWindow::on_textEditor_copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);
}


void MainWindow::on_textEditor_redoAvailable(bool b)
{
    ui->actionRedo->setEnabled(b);
}


void MainWindow::on_textEditor_undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);
}

