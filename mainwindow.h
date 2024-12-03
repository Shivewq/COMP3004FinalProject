#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "device.h"
#include "app.h"
#include "user.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSeries>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QListView>
#include <QListWidgetItem>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //profile page buttons
    void on_Add_User_clicked();
    void on_Delete_User_clicked();
    void on_Update_User_clicked();

    void update_Active_User(const QString &text);
    void onStart();
    void loadUserHistory();
    void onScanSelected(QListWidgetItem* item);

private slots:
    void showBatteryMsg();
    //Menu Selection Options
    void on_button_home_clicked();
    void on_button_history_clicked();
    void on_button_profiles_clicked();
    void on_button_measure_clicked();
    void on_details_clicked();

    //Metering chart updating
    void updateChart(int y);
    void clearChart(int max_y,int max_x);
    void initializeHistoryBar(Scan*);
    void initializePolarGraph(const QVector<int>& processedData);
    void initializeHomeGraph(Scan*);
    void populateOrganList();

    //Device Buttons
    void on_button_on_clicked();
    void on_button_off_clicked();
    void on_button_charge_clicked();
    void on_button_startMeasure_clicked();

    //UI dynamic changing
    void changeMeasurePointUI(int const*);
    void changeBodyImageUI(int const*);
    void changeSkinContact(bool);
    void deviceStateUI(bool);
    void batteryOutMessage(QString);
    void updateMeasureButtonUI();
    void batteryOutOff();

    void on_button_Interrupt_clicked();

private:
    Ui::MainWindow *ui;
    QtCharts::QLineSeries *series;
    QtCharts::QBarSeries *historyBar;
    Device * device;
    App* app;
    QStringListModel* userModel;
    int pointCounter =1;

public slots:
    void on_editBattery(int value);

};
#endif // MAINWINDOW_H
