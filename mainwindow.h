#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAbout_triggered();

    void on_actionFind_triggered();

    void on_actionReplace_triggered();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_textEditor_textChanged();

    void on_actionUndo_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionRedo_triggered();

    void on_textEditor_copyAvailable(bool b);

    void on_textEditor_redoAvailable(bool b);

    void on_textEditor_undoAvailable(bool b);

    void on_actionFontColor_triggered();

    void on_actionFontBackgroundColor_triggered();

    void on_actionBackgroundColor_triggered();

    void on_actionLineWrap_triggered();

    void on_actionFont_triggered();

    void on_actiontoolsbar_triggered();

    void on_actionStatebar_triggered();

    void on_actionSelectAll_triggered();

    void on_actionExit_triggered();

    void on_textEditor_cursorPositionChanged();

    void on_actionShowLineNumber_triggered();

private:
    Ui::MainWindow *ui;
    QLabel statusCursorLabel;
    QLabel statusLabel;

    QString filePath;
    bool textChanged;

    bool userEdiitConfirmed();
};
#endif // MAINWINDOW_H
