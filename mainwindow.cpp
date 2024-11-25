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
    connect(device->getBattery(),&Battery::lowBatteryWarning,this,&MainWindow::showBatteryMsg);

    //setup app
    app = new App(device);
    app->activeUser = new User();


    userModel = new QStringListModel(this);
    ui->profileList->setModel(userModel);
    connect(ui->button_add, &QPushButton::clicked, this, &MainWindow::on_Add_User_clicked);
    connect(ui->button_delete, &QPushButton::clicked, this, &MainWindow::on_Delete_User_clicked);
    connect(ui->button_update, &QPushButton::clicked, this, &MainWindow::on_Update_User_clicked);
    connect(ui->userSelect, &QComboBox::currentTextChanged, this, &MainWindow::update_Active_User);

    // Create a chart and add the series
    series = new QLineSeries(this);
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Metering Body Point");
    chart->legend()->hide();
    QValueAxis *xAxis = new QValueAxis;
    QValueAxis *yAxis = new QValueAxis;
    xAxis->setRange(0, 20);
    yAxis->setRange(0, 100);

    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addAxis(yAxis, Qt::AlignLeft);
    series->attachAxis(xAxis);
    series->attachAxis(yAxis);


    // display chartView
    ui->meteringGraph->setChart(chart); // `graphicsView` is now a QChartView
    ui->meteringGraph->setRenderHint(QPainter::Antialiasing); // Enable anti-aliasing for smooth rendering

    connect(app, &App::plotPoint, this, &MainWindow::updateChart);
    connect(app,&App::clearMeteringGraph,this,&MainWindow::clearChart);

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

//Creates new user from input data
void MainWindow::on_Add_User_clicked(){
    //get the users info from the UI
    QString name = ui->text_name->toPlainText();
    QString heightStr = ui->text_height->toPlainText();
    QString weightStr = ui->text_weight->toPlainText();

    // Validate the input
    if (name.isEmpty() || heightStr.isEmpty() || weightStr.isEmpty()) {
        qInfo() << "Please fill in all fields.";
        return;
    }

    //Convert QString to int
    int numHeight = heightStr.toInt();
    int numWeight = weightStr.toInt();


    User* newUser = new User(this, name, numWeight, numHeight);
    app->addUser(newUser);

    //adjust UI accordingly
    ui->text_name->clear();
    ui->text_height->clear();
    ui->text_weight->clear();

    QStringList users = userModel->stringList();
    users.append(name);
    userModel->setStringList(users);


    ui->userSelect->addItem(newUser->getName(), QVariant::fromValue(newUser));
    ui->profileLog->setText("Profile Created!");

    //connecting user signals (Eric added this for shutdown...)
    //I am not too sure if this will delete the most recent scan from all users...
    //if not then just need to add a check to make sure we are doing it to active user
    connect(app,&App::deleteCurrentScan,newUser,&User::deleteScan);
}

// deletes all traces of a user
void MainWindow::on_Delete_User_clicked(){

    //get the users name from the list view and remove that row from the list view
    QModelIndex selectedIndex = ui->profileList->currentIndex();
    QStringListModel* model = qobject_cast<QStringListModel*>(ui->profileList->model());
    QString userName = model->data(selectedIndex, Qt::DisplayRole).toString();
    model->removeRow(selectedIndex.row());

    //remove user from the combo box
    int comboIndex = ui->userSelect->findText(userName);
    ui->userSelect->removeItem(comboIndex);

    //get the user to delete from its name, and delete the user from the user list
    User* userToDelete = app->getUserFromName(userName);
    app->deleteUser(userToDelete);

    ui->profileLog->setText("Profile Deleted");
}

//update the users info and the UI accordingly
void MainWindow::on_Update_User_clicked(){

    User* userToUpdate = app->getActiveUser();

    if(!ui->text_name->toPlainText().isEmpty()){

        QString oldName = userToUpdate->getName();
        QString newName = ui->text_name->toPlainText();
        userToUpdate->setName(newName);

        //display updated name in profile list
        QStringListModel* listModel = qobject_cast<QStringListModel*>(ui->profileList->model());
        int row = listModel->stringList().indexOf(oldName);
        listModel->setData(listModel->index(row), newName);

        //display updated name in combo box
        int comboIndex = ui->userSelect->findText(oldName);
        ui->userSelect->setItemText(comboIndex, newName);
    }

    if(!ui->text_weight->toPlainText().isEmpty()){
        QString weightStr = ui->text_weight->toPlainText();
        int numWeight = weightStr.toInt();
        userToUpdate->setWeight(numWeight);
    }

    if(!ui->text_height->toPlainText().isEmpty()){
        QString heightStr = ui->text_height->toPlainText();
        int numHeight = heightStr.toInt();
        userToUpdate->setHeight(numHeight);
    }

    ui->profileLog->setText("Profile Updated!");
    ui->text_name->clear();
    ui->text_height->clear();
    ui->text_weight->clear();


}


//updates active user
void MainWindow::update_Active_User(const QString &text) {
    ui->activeUser->setText(text);

    User* activeUser = app->getUserFromName(text);
    app->setActiveUser(activeUser);

}


void MainWindow::updateChart(int y)
{
    series->append(pointCounter, y); // Append (x, y) to the chart series
    // Update axis ranges dynamically
    pointCounter++; // Increment the x value
    ui->meteringGraph->repaint();
}

void MainWindow::clearChart(int max_y,int max_x)
{

    QChart *chart = ui->meteringGraph->chart();
    chart->removeAllSeries(); // Remove existing series
    // Create a new empty series
    series = new QLineSeries();
    chart->addSeries(series);
    chart->removeAxis(chart->axisX());
    chart->removeAxis(chart->axisY());

    // Reset axes
    QValueAxis *xAxis = new QValueAxis();
    QValueAxis *yAxis = new QValueAxis();
    xAxis->setRange(1, max_x); // Start with a basic range
    yAxis->setRange(0, max_y + 5); // Adjust as needed for initial view
    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addAxis(yAxis, Qt::AlignLeft);
    series->attachAxis(xAxis);
    series->attachAxis(yAxis);

    chart->update();
    pointCounter = 1; // Reset the x-axis counter

}


void MainWindow::on_button_on_clicked()
{
    device->turnOn();
}


void MainWindow::on_button_off_clicked()
{
    device->turnOff();
}


void MainWindow::on_button_charge_clicked()
{
    device->plugIn();
}


void MainWindow::on_button_startMeasure_clicked()
{
    app->MeasureFunctionTemplate();
}

