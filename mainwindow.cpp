#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginBtn_clicked()
{
    QString username = ui ->usernameLineedit->text();
    QString password = ui ->passwordLineedit->text();

    if (username == "admin" && password == "1234"){
        QMessageBox::information(this,"LogIn","Login Succes");
        hide();
        dialog2 = new Dialog2(this);
        dialog2 -> show();
    }
    else{
        QMessageBox::warning(this,"LogIn","Wrong username or password");
    }
}