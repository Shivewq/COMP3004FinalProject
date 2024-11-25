#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include "device.h"
#include "app.h"
#include "user.h"

#include <QStringListModel>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void on_Add_User_clicked();
    void on_Delete_User_clicked();
    void on_Update_User_clicked();
    void update_Active_User(const QString &text);


private slots:
    void on_button_home_clicked();

    void on_button_history_clicked();

    void on_button_profiles_clicked();




private:
    Ui::MainWindow *ui;
    Device * device;
    App* app;
    QStringListModel* userModel;

public slots:
    void on_editBattery(int value);
};



#endif // MAINWINDOW_H
