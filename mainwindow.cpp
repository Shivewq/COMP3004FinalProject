#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scan.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Setup UI
    ui->setupUi(this);
    this->setStyleSheet("background-color: white;");
    ui->stackedWidget->setCurrentIndex(2); // Set to homepage
    connect(ui->button_profiles, &QPushButton::clicked, this, &MainWindow::on_button_profiles_clicked);
    connect(ui->User_History, &QListWidget::itemClicked, this, &MainWindow::onScanSelected);


    //setup device
    device = new Device();
    connect(device->getBattery(),&Battery::editBattery,this,&MainWindow::on_editBattery);
    connect(device->getBattery(),&Battery::editBattery,this,&MainWindow::on_editBattery);
    connect(device->getBattery(),&Battery::lowBatteryWarning,this,&MainWindow::showBatteryMsg);
    connect(device,&Device::statusChange,this,&MainWindow::deviceStateUI);

    //setup app
    app = new App(device);
    app->activeUser = new User();

    populateOrganList();
    userModel = new QStringListModel(this);
    ui->profileList->setModel(userModel);
    connect(ui->button_add, &QPushButton::clicked, this, &MainWindow::on_Add_User_clicked);
    connect(ui->button_delete, &QPushButton::clicked, this, &MainWindow::on_Delete_User_clicked);
    connect(ui->button_update, &QPushButton::clicked, this, &MainWindow::on_Update_User_clicked);
    connect(ui->mesNow_button, &QPushButton::clicked, this, &MainWindow::on_button_measure_clicked);
    connect(ui->details_button, &QPushButton::clicked, this, &MainWindow::on_details_clicked);
    connect(ui->userSelect, &QComboBox::currentTextChanged, this, &MainWindow::update_Active_User);
    connect(app, &App::bodyPointNumber, this, &MainWindow::changeMeasurePointUI);
    connect(app, &App::bodyImageNum, this, &MainWindow::changeBodyImageUI);
    connect(app, &App::skinContact, this, &MainWindow::changeSkinContact);
    connect(app, &App::outOfBattery, this, &MainWindow::batteryOutMessage);
    connect(app, &App::doneScan, this, &MainWindow::updateMeasureButtonUI);

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

    onStart();

}

void MainWindow::onStart(){
    //Get History ready and make sure it cant be access until there is a user
    ui->button_history->setEnabled(false);
    ui->button_measure->setEnabled(false);
    ui->button_charge->setEnabled(false);
    ui->button_on->setEnabled(false);
    ui->button_off->setEnabled(false);
    ui->mesNow_button->setEnabled(false);
    ui->button_startMeasure->setEnabled(false);
    ui->button_on->setEnabled(false);
    ui->button_off->setEnabled(false);
}


void MainWindow::initializeHomeGraph(Scan* selectedScan)
{
   // Prepare the bar sets for "Right" and "Left"
   QtCharts::QBarSet *leftSet = new QtCharts::QBarSet("Left");
   QtCharts::QBarSet *rightSet = new QtCharts::QBarSet("Right");

   // Assign colors
   leftSet->setColor(Qt::blue);
   rightSet->setColor(Qt::green);

   // Get processed data
   QVector<int> processedData = selectedScan->getProccesedPoints();
   if (processedData.isEmpty() || processedData.size() < 24) {
       qWarning() << "Invalid or insufficient processed scan data.";
       return;
   }

   // Add values to bar sets
   for (int i = 0; i < 12; ++i) {
       *leftSet << processedData[i];          // Left side of organ
       *rightSet << processedData[i + 12];   // Corresponding right side of organ
   }

   // Create the bar series and add the sets
   historyBar = new QtCharts::QBarSeries();
   historyBar->append(leftSet);
   historyBar->append(rightSet);

   // Create the chart and add the series
   QtCharts::QChart *chart = new QtCharts::QChart();
   chart->addSeries(historyBar);
   chart->setTitle("Organ Functionality Comparison");
   chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

   // Define categories for the x-axis
   QStringList categories = app->categories;
   // Set up x-axis
   QtCharts::QBarCategoryAxis *xAxis = new QtCharts::QBarCategoryAxis();
   xAxis->append(categories);
   chart->addAxis(xAxis, Qt::AlignBottom);
   historyBar->attachAxis(xAxis);

   // Set up y-axis
   QtCharts::QValueAxis *yAxis = new QtCharts::QValueAxis();
   yAxis->setRange(0, 200); // Assuming percentages range from 0 to 200%
   yAxis->setTitleText("Functionality (%)");
   chart->addAxis(yAxis, Qt::AlignLeft);
   historyBar->attachAxis(yAxis);

   // Add the chart to a QChartView in your UI
   ui->home_graph->setChart(chart); // Assuming `historyGraph` is a QChartView in your UI
   ui->home_graph->setRenderHint(QPainter::Antialiasing);


}

void MainWindow::on_details_clicked(){
    ui->stackedWidget->setCurrentIndex(3);

    if(!(app->getActiveUser()->getScanList().empty())){
        ui->User_History->clear();
        loadUserHistory();
        initializeHistoryBar(app->getActiveUser()->getMostRecentScan());
        ui->scan_title_label->setText(app->getActiveUser()->getMostRecentScan()->toString());
    }

}


void MainWindow::initializeHistoryBar(Scan* selectedScan)
{
    // Prepare the bar sets for "Right" and "Left"
    QtCharts::QBarSet *leftSet = new QtCharts::QBarSet("Left");
    QtCharts::QBarSet *rightSet = new QtCharts::QBarSet("Right");

    // Assign colors
    leftSet->setColor(Qt::blue);
    rightSet->setColor(Qt::green);

    // Get processed data
    QVector<int> processedData = selectedScan->getProccesedPoints();
    if (processedData.isEmpty() || processedData.size() < 24) {
        qWarning() << "Invalid or insufficient processed scan data.";
        return;
    }

    // Add values to bar sets
    for (int i = 0; i < 12; ++i) {
        *leftSet << processedData[i];          // Left side of organ
        *rightSet << processedData[i + 12];   // Corresponding right side of organ
    }

    // Create the bar series and add the sets
    historyBar = new QtCharts::QBarSeries();
    historyBar->append(leftSet);
    historyBar->append(rightSet);

    // Create the chart and add the series
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(historyBar);
    chart->setTitle("Organ Functionality Comparison");
    chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

    // Define categories for the x-axis
    QStringList categories = app->categories;
    // Set up x-axis
    QtCharts::QBarCategoryAxis *xAxis = new QtCharts::QBarCategoryAxis();
    xAxis->append(categories);
    chart->addAxis(xAxis, Qt::AlignBottom);
    historyBar->attachAxis(xAxis);

    // Set up y-axis
    QtCharts::QValueAxis *yAxis = new QtCharts::QValueAxis();
    yAxis->setRange(0, 200); // Assuming percentages range from 0 to 200%
    yAxis->setTitleText("Functionality (%)");
    chart->addAxis(yAxis, Qt::AlignLeft);
    historyBar->attachAxis(yAxis);

    // Add the chart to a QChartView in your UI
    ui->barGraph->setChart(chart); // Assuming `historyGraph` is a QChartView in your UI
    ui->barGraph->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::initializePolarGraph(const QVector<int>& processedData)
{
    // Ensure processedData has the expected 24 points (12 left + 12 right)
    if (processedData.size() != 24) {
        qWarning() << "Processed data must have exactly 24 points.";
        return;
    }

    // Split processedData into left and right sides
    QVector<int> leftData, rightData;
    for (int i = 0; i < 12; ++i) {
        leftData << processedData[i];          // Left side of organ
        rightData << processedData[i + 12];   // Corresponding right side of organ
    }

    // Create a polar chart
    QPolarChart *polarChart = new QPolarChart();
    polarChart->setTitle("Organ Functionality - Polar Chart");

    // Create axes
    QCategoryAxis *angleAxis = new QCategoryAxis();
    QStringList pointNames = {"H1", "H2", "H3", "H4", "H5", "H6",
                              "F1", "F2", "F3", "F4", "F5", "F6"};

    for (int i = 0; i < pointNames.size(); ++i) {
        angleAxis->append(pointNames[i], i * 30); // Map point names to angular positions
    }
    angleAxis->setRange(0, 360); // Full circular range
    angleAxis->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    polarChart->addAxis(angleAxis, QPolarChart::PolarOrientationAngular);

    QValueAxis *radiusAxis = new QValueAxis();
    radiusAxis->setRange(0, 200); // Adjust as necessary
    radiusAxis->setTickCount(5);
    polarChart->addAxis(radiusAxis, QPolarChart::PolarOrientationRadial);

    // Add indicator lines for functionality
    QLineSeries *lowLine = new QLineSeries();
    QLineSeries *normalLine = new QLineSeries();
    QLineSeries *highLine = new QLineSeries();

    for (int i = 0; i <= 360; i += 30) {
        lowLine->append(i, 90);    // Low functionality threshold
        normalLine->append(i, 100); // Normal functionality threshold
        highLine->append(i, 108);  // High functionality threshold
    }

    polarChart->legend()->setAlignment(Qt::AlignLeft); // Position legend on the left
    polarChart->legend()->setMarkerShape(QLegend::MarkerShapeRectangle);

    lowLine->setName("Low (90%)");
    normalLine->setName("Normal (100%)");
    highLine->setName("High (108%)");

    polarChart->addSeries(lowLine);
    polarChart->addSeries(normalLine);
    polarChart->addSeries(highLine);

    lowLine->attachAxis(angleAxis);
    lowLine->attachAxis(radiusAxis);

    normalLine->attachAxis(angleAxis);
    normalLine->attachAxis(radiusAxis);

    highLine->attachAxis(angleAxis);
    highLine->attachAxis(radiusAxis);

    QPen lowPen(Qt::red);
    lowPen.setStyle(Qt::DashLine);
    lowLine->setPen(lowPen);

    QPen normalPen(Qt::black);
    normalPen.setStyle(Qt::DashLine);
    normalLine->setPen(normalPen);

    QPen highPen(Qt::green);
    highPen.setStyle(Qt::DashLine);
    highLine->setPen(highPen);

    // Add the left side data
    QLineSeries *leftSeries = new QLineSeries();
    for (int i = 0; i < leftData.size(); ++i) {
        leftSeries->append(i * 30, leftData[i]); // 30 degrees per data point
    }

    // Add the right side data
    QLineSeries *rightSeries = new QLineSeries();
    for (int i = 0; i < rightData.size(); ++i) {
        rightSeries->append(i * 30, rightData[i]); // 30 degrees per data point
    }

    // Create area series for left and right sides
    QAreaSeries *leftArea = new QAreaSeries(leftSeries);
    leftArea->setName("Left Side");
    QColor leftColor = Qt::blue;
    leftColor.setAlpha(150);
    leftArea->setBrush(QBrush(leftColor));

    QAreaSeries *rightArea = new QAreaSeries(rightSeries);
    rightArea->setName("Right Side");
    QColor rightColor = Qt::red;
    rightColor.setAlpha(100);
    rightArea->setBrush(QBrush(rightColor));

    polarChart->addSeries(leftArea);
    polarChart->addSeries(rightArea);

    leftSeries->attachAxis(angleAxis);
    leftSeries->attachAxis(radiusAxis);

    rightSeries->attachAxis(angleAxis);
    rightSeries->attachAxis(radiusAxis);

    leftArea->attachAxis(angleAxis);
    leftArea->attachAxis(radiusAxis);

    rightArea->attachAxis(angleAxis);
    rightArea->attachAxis(radiusAxis);

    // Set the polar chart to the QChartView
    ui->polarGraph->setChart(polarChart);
    ui->polarGraph->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::populateOrganList()
{
    // Create a QStringList to hold the associations
    QStringList organAssociations = {
        "H1 - Lungs",
        "H2 - Pericardium",
        "H3 - Heart",
        "H4 - Small Intestines",
        "H5 - Immune System (Triple Heater)",
        "H6 - Large Intestines",
        "F1 - Spleen and Pancreas",
        "F2 - Liver",
        "F3 - Kidneys",
        "F4 - Bladder",
        "F5 - Gallbladder",
        "F6 - Stomach",
    };

    // Create a QStringListModel
    QStringListModel *model = new QStringListModel(this);
    model->setStringList(organAssociations);

    // Set the model to the QListView
    ui->organList->setModel(model);

    // Optional: Make the list view read-only
    ui->organList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_editBattery(int value){
    ui->battery->setValue(value);
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

void MainWindow::batteryOutMessage(QString msg)
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("OUT OF BATTERY");

    QString text = QString("Device is Out of Battery! %1 Charge to Measure...").arg(msg);
    msgBox.setText(text);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Close);

    msgBox.exec();

    ui->label_deviceStatus->setText("OUT OF BATTERY!");
    ui->button_startMeasure->setEnabled(false);
}

// **MENU SELECTION CODE**
void MainWindow::on_button_home_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    if(!(app->getActiveUser()->getScanList().empty())){
           initializeHomeGraph(app->getActiveUser()->getMostRecentScan());
    }


}

void MainWindow::on_button_history_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->User_History->clear();
    loadUserHistory();
}

void MainWindow::on_button_measure_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_button_profiles_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
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

void MainWindow::changeMeasurePointUI(int const* point){
    ui->currentPointText->setText(QString::number(*point+1));


}

void MainWindow::changeBodyImageUI(int const* point){

    ui->imageWidget->setCurrentIndex(*point);
}
void MainWindow::changeSkinContact(bool isContact){
    if(isContact){
        ui->label_contact->setText(QString("On Skin!"));
    }
    else{
        ui->label_contact->setText(QString("Off Skin!"));
    }
}

//updates active user
void MainWindow::update_Active_User(const QString &text) {
    ui->activeUser->setText(text);
    ui->Welcome_label->setText("Welcome");
    ui->Name_label->setText(text);

    User* activeUser = app->getUserFromName(text);
    app->setActiveUser(activeUser);
    //Enable buttons once user is created
    ui->button_history->setEnabled(true);
    ui->button_measure->setEnabled(true);
    ui->details_button->setEnabled(true);
    ui->button_charge->setEnabled(true);
    ui->button_on->setEnabled(true);
    ui->button_off->setEnabled(false);
    ui->mesNow_button->setEnabled(true);
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

void MainWindow::loadUserHistory(){
    User* u = app->getActiveUser();
    if (!u){
        qWarning() << "No active user found.";
        return;
    }

    //Add scans to the  list model
    for (int i = u->getScanList().size() - 1; i >= 0; --i){
        Scan* scan = u->getScan(i);
        if (scan) {
            QListWidgetItem* item = new QListWidgetItem(scan->toString(), ui->User_History);
            item->setData(Qt::UserRole, QVariant::fromValue(scan));
        }
    }
}

void MainWindow::onScanSelected(QListWidgetItem* item) {
    if (!item) return;

    //Get the scan that the user has selected
    Scan* selectedScan = item->data(Qt::UserRole).value<Scan*>();

    if (selectedScan) {
        QStringList organs= {
            "Lungs",
            "Pericardium",
            "Heart",
            "Small Intestines",
            "Immune System (Triple Heater)",
            "Large Intestines",
            "Spleen and Pancreas",
            "Liver",
            "Kidneys",
            "Bladder",
            "Gallbladder",
            "Stomach",
        };
        qDebug() << "Selected Scan:" << selectedScan;
        initializeHistoryBar(selectedScan);
        QVector<int> processedData = selectedScan->getProccesedPoints();
        //graphing the data
        initializePolarGraph(processedData);
        ui->scan_title_label->setText(selectedScan->toString());

        //getting the worst functioning organ for reccomendations
        auto min_it = std::min_element(processedData.begin(), processedData.end());
        if (min_it != processedData.end()) {
               // Compute the index of the smallest element
               int index = std::distance(processedData.begin(), min_it);
               //if its left side
               if(index < 11){
                   ui->label_reccomendation->setText(organs.at(index));
               }
               //if its right side
               else{
                   ui->label_reccomendation->setText(organs.at(index -12)); //could be wrong. Think it should be index - 12 cause it needs to loop back around
               }
        }
    }
}

void MainWindow::deviceStateUI(bool status){
    if(status){
        ui->button_startMeasure->setEnabled(true);
        ui->label_deviceStatus->setText(QString("ON"));
    }
    else{
        ui->button_startMeasure->setEnabled(false);
        ui->label_deviceStatus->setText(QString("OFF"));
    }
}


void MainWindow::on_button_on_clicked()
{
    ui->button_on->setEnabled(false);
    ui->button_off->setEnabled(true);
    device->turnOn();
}


void MainWindow::on_button_off_clicked()
{
    ui->button_on->setEnabled(true);
    ui->button_off->setEnabled(false);
    device->turnOff();
}


void MainWindow::on_button_charge_clicked()
{
    ui->button_startMeasure->setEnabled(true);
    ui->label_deviceStatus->setEnabled("ON");
    device->plugIn();
}


void MainWindow::on_button_startMeasure_clicked()
{
    ui->button_startMeasure->setEnabled(false);
    app->MeasureFunctionTemplate();
}

void MainWindow::updateMeasureButtonUI()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("DONE SCAN");
    msgBox.setText("Scan Complete! Added to History.");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Close);

    msgBox.exec();

    ui->button_startMeasure->setEnabled(true);
}

