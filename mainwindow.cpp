#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scan.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //setup device
    device = new Device();
    connect(device->charge,&Battery::editBattery,this,&MainWindow::on_editBattery);
    //setup app
    app = new App(device);
    app->activeUser = new User();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_editBattery(int value){
    ui->label_batteryValue->setText(QString::number(value));
}


void MainWindow::on_button_home_clicked()
{
}



void MainWindow::on_button_history_clicked()
{
}


void MainWindow::on_button_profiles_clicked()
{
    //app->measure();
    app->calculateScan(0);
    Scan* scan = app->activeUser->getScan(0);
    app->calculateReadingGraph(scan->getPoints().at(1));
}

