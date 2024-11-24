#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QTimer>

QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0); // Set to homepage
    connect(ui->button_profiles, &QPushButton::clicked, this, &MainWindow::on_button_profiles_clicked);
    //setup device
    device = new Device();
    connect(device->charge,&Battery::editBattery,this,&MainWindow::on_editBattery);
    //setup app
    app = new App(device);
    app->activeUser = new User();
    QLineSeries *series = new QLineSeries();

    // Create a chart and add the series
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Metering Body Point");
    chart->legend()->hide();

    // display chartView
    ui->meteringGraph->setChart(chart); // `graphicsView` is now a QChartView
    ui->meteringGraph->setRenderHint(QPainter::Antialiasing); // Enable anti-aliasing for smooth rendering

    connect(app, &App::plotPoint, this, &MainWindow::updateChart);
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
    ui->stackedWidget->setCurrentIndex(2);

    app->measure();
    app->calculateScan(0);
}

void MainWindow::updateChart(int y)
{
    series->append(pointCounter, y); // Append (x, y) to the chart series
    pointCounter++;                 // Increment the x value
    ui->meteringGraph->repaint();
}
