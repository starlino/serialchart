#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "common.h"
#include "process.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);

private:
    Ui::MainWindow *ui;

    QString documentFilePath;

    bool saveDocument(const QString& filePath);
    bool loadDocument(const QString& filePath);
    bool checkDocument();
    bool documentIsDirty;
    Configuration* config;
    Process* process;

private slots:
    void on_actionConfiguration_toggled(bool );
    void on_actionChart_toggled(bool );
    void on_actionToolbar_toggled(bool );
    void on_actionStop_triggered();
    void on_actionRun_triggered();
    void on_actionAbout_triggered();
    void on_configurationText_textChanged();
    void on_actionSaveAs_triggered();
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_actionExit_triggered();
    void on_actionNew_triggered();

    void on_process_newDataLine(const QList<double>&);
    void on_process_stopped();
};

#endif // MAINWINDOW_H
