#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    documentIsDirty = false;

    config = new Configuration(this);
    portValid = false;


    loadDocument(settings.value("Last Configuration").toString());


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
    if(portValid) port->requestToStop();
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
        documentIsDirty  = false;
        updateDocumentFilePath(filePath);
        success = true;
    }

    if(!success)
        QMessageBox::critical(0,"","Could not save file: " + filePath);
    else
        ui->statusBar->showMessage("File Saved",2000);

    return success;
}

bool MainWindow::loadDocument(const QString& filePath)
{
    bool success = true;
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly)){
        QTextStream textStream(&file);
        ui->configurationText->setPlainText(textStream.readAll());
        updateDocumentFilePath(filePath);
        success = true;
    }

    if(!success) QMessageBox::critical(0,"","Could not load file: " + filePath);

    return success;
}

void MainWindow::updateDocumentFilePath(const QString& filePath){
    documentFilePath = filePath;
    settings.setValue("Last Configuration",filePath);
    this->setWindowTitle("SerialChart - " + QFileInfo(filePath).fileName() );
    documentIsDirty  = false;
};


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
    ui->sendButton->setEnabled(true);
    ui->dataText->clear();

    port = createPort(this,config);
    decoder = createDecoder(this,config);
    display = createDisplay(this,config);

    portValid = true;
    //port signals
    connect(port,SIGNAL(newData(const QByteArray&)),decoder,SLOT(newData(const QByteArray&)));
    connect(port,SIGNAL(packetSeparator()),decoder,SLOT(packetSeparator()));
    connect(port,SIGNAL(stopped()),this,SLOT(portStopped()));
    //decoder signals
    connect(decoder,SIGNAL(newPacket(DecoderBase*)),ui->chart,SLOT(newPacket(DecoderBase*)));
    connect(decoder,SIGNAL(newPacket(DecoderBase*)),display,SLOT(newPacket(DecoderBase*)));
    //display signals
    connect(display,SIGNAL(newDisplay(const QString&)),ui->dataText,SLOT(appendPlainText(const QString&)));

    port->start();
}

void MainWindow::on_actionStop_triggered()
{
    ui->actionStop->setEnabled(false);
    ui->sendButton->setEnabled(false);
    port->requestToStop();
}

void MainWindow::portStopped(){
    ui->actionStop->setEnabled(false);
    ui->sendButton->setEnabled(false);
    ui->actionRun->setEnabled(true);
    port->deleteLater();
    portValid = false;
    delete decoder;
    delete display;
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

void MainWindow::on_sendButton_clicked()
{
    if(portValid) port->send(ui->sendText->text());
}
