#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scan.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Setup UI
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0); // Set to homepage
    connect(ui->button_profiles, &QPushButton::clicked, this, &MainWindow::on_button_profiles_clicked);

    //setup device
    device = new Device();
    connect(device->getBattery(),&Battery::editBattery,this,&MainWindow::on_editBattery);
    connect(device->getBattery(),&Battery::editBattery,this,&MainWindow::on_editBattery);

    //setup app
    app = new App(device);
    app->activeUser = new User();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_editBattery(int value){
    if(ui->battery->value() != 0){
        ui->battery->setValue(value);
    }
    ui->label_batteryValue->setText(QString::number(value));
}

void MainWindow::showBatteryMsg()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("LOW BATTERY WARNING");
    msgBox.setText("Battery level is at 20%. Please charge soon.");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Close);

    msgBox.exec();
}

// **MENU SELECTION CODE**
void MainWindow::on_button_home_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_button_history_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_button_measure_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_button_profiles_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);

    /* Eric Test Code
    app->measure();
    app->calculateScan(0);
    Scan* scan = app->activeUser->getScan(0);
    app->calculateReadingGraph(scan->getPoints().at(1));
    */
}

