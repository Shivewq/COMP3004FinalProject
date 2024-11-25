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

    userModel = new QStringListModel(this);
    ui->profileList->setModel(userModel);
    connect(ui->button_add, &QPushButton::clicked, this, &MainWindow::on_Add_User_clicked);
    connect(ui->button_delete, &QPushButton::clicked, this, &MainWindow::on_Delete_User_clicked);
    connect(ui->button_update, &QPushButton::clicked, this, &MainWindow::on_Update_User_clicked);
    connect(ui->userSelect, &QComboBox::currentTextChanged, this, &MainWindow::update_Active_User);



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
}



void MainWindow::on_button_history_clicked()
{
}


void MainWindow::on_button_profiles_clicked()
{
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
