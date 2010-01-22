#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    documentIsDirty = false;

    config = new Configuration(this);
    process = new Process(this,config);

    connect(process,SIGNAL(stopped()),this,SLOT(on_process_stopped()));
    connect(process,SIGNAL(newDataLine(const QList<double>&)),ui->chart,SLOT(plotData(const QList<double>&)));
    connect(process,SIGNAL(newDataLine(const QList<double>&)),this,SLOT(on_process_newDataLine(const QList<double>&)));


    loadDocument("../scripts/imu_arduino.scc");
    config->parse( ui->configurationText->toPlainText());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void MainWindow::closeEvent(QCloseEvent *e)
{
    process->requestToStop();
    if(!checkDocument()) e->ignore();
}

void MainWindow::on_actionNew_triggered()
{
    if(checkDocument()){
        ui->configurationText->clear();
        documentFilePath = "";
        ui->statusBar->showMessage(documentFilePath);
        documentIsDirty  = false;
    }
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}


void MainWindow::on_actionOpen_triggered()
{
    if(!checkDocument()) return;

    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Open File"), documentFilePath, tr("SerialChart Configuration (*.scc);;All Files (*.*)"));

    if(!filePath.isEmpty()) loadDocument(filePath);

}

void MainWindow::on_actionSave_triggered()
{
    if(documentFilePath.isEmpty())
        on_actionSaveAs_triggered();
    else
        saveDocument(documentFilePath);
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"),
                               documentFilePath,
                               tr("SerialChart Configuration (*.scc);;All Files (*.*)"));
    if(!filePath.isEmpty()) saveDocument(filePath);
}

bool MainWindow::checkDocument(){
    if(documentIsDirty)
        if(QMessageBox::No == QMessageBox::warning(0,"","Current configuration was changed but not saved.\nAre you sure you want to proceed ?",QMessageBox::Yes,QMessageBox::No|QMessageBox::Default))
            return false;
    return true;
}

bool MainWindow::saveDocument(const QString& filePath)
{
    bool success = false;
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly)){
        QTextStream textStream(&file);
        textStream<<ui->configurationText->toPlainText();
        textStream.flush();
        documentFilePath = filePath;
        ui->statusBar->showMessage(documentFilePath);
        documentIsDirty  = false;
        success = true;
    }

    if(!success) QMessageBox::critical(0,"","Could not save file: " + filePath);

    return success;
}

bool MainWindow::loadDocument(const QString& filePath)
{
    bool success = true;
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly)){
        QTextStream textStream(&file);
        ui->configurationText->setPlainText(textStream.readAll());
        documentFilePath = filePath;
        ui->statusBar->showMessage(documentFilePath);
        documentIsDirty  = false;
        success = true;
    }

    if(!success) QMessageBox::critical(0,"","Could not load file: " + filePath);

    return success;
}


void MainWindow::on_configurationText_textChanged()
{
    documentIsDirty = true;
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(0,"","Developed by Starlino\nwww.Starlino.com");
}

void MainWindow::on_actionRun_triggered()
{
    config->parse( ui->configurationText->toPlainText());
    ui->dataText->setMaximumBlockCount(MAX(1,config->get("_setup_","width").toInt()));


    ui->chart->init(config);
    ui->actionRun->setEnabled(false);
    ui->actionStop->setEnabled(true);
    ui->dataText->clear();
    process->start();
}

void MainWindow::on_actionStop_triggered()
{
    ui->actionStop->setEnabled(false);
    process->requestToStop();
}

void MainWindow::on_process_stopped(){
    ui->actionStop->setEnabled(false);
    ui->actionRun->setEnabled(true);
}

void MainWindow::on_process_newDataLine(const QList<double>& data){
    ui->dataText->appendPlainText(process->dataLine);
}

void MainWindow::on_actionToolbar_toggled(bool b)
{
    ui->mainToolBar->setVisible(b);
}

void MainWindow::on_actionChart_toggled(bool b)
{
    ui->dockWidgetChart->setVisible(b);
}

void MainWindow::on_actionConfiguration_toggled(bool b)
{
    ui->dockWidgetConfiguration->setVisible(b);
}
