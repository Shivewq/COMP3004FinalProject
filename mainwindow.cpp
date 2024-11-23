#include "mainwindow.h"
#include "ui_mainwindow.h"
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
    ui->Battery->setText(QString::number(value));
}


void MainWindow::on_button_home_clicked()
{
    QLabel* label = new QLabel("battery low",this);
    label->setGeometry(100,100,300,500);
    label->setVisible(true);
}



void MainWindow::on_button_history_clicked()
{
    device->turnOn();
}


void MainWindow::on_button_profiles_clicked()
{
    app->measure();
    app->calculateScan(0);
}

