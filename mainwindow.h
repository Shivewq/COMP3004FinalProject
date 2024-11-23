#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "device.h"
#include "app.h"
#include "user.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_button_home_clicked();

    void on_button_history_clicked();

    void on_button_profiles_clicked();

private:
    Ui::MainWindow *ui;
    Device * device;
    App* app;
public slots:
    void on_editBattery(int value);
};
#endif // MAINWINDOW_H
