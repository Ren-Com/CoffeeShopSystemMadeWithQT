#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hashutils.h"
#include <QMessageBox>
#include <QLabel>
#include <QMouseEvent>
#include <QFile>
#include <QTextStream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , signUp(nullptr)
{
    ui->setupUi(this);

    if (ui->signUpLabel) {
        ui->signUpLabel->installEventFilter(this);
        ui->signUpLabel->setCursor(Qt::PointingHandCursor);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    // ngedetect klik di tulisan signup
    if (watched == ui->signUpLabel && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            onSignUpLabelClicked();
            return true;
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

bool MainWindow::validateLogin(const QString &username, const QString &password)
{
    QString filePath = "../../database/account.txt";

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open account file:" << filePath;
        return false;
    }

    QString hashedInputPassword = HashUtils::sha256(password);

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty()) continue;

        QStringList parts = line.split(',');
        if (parts.size() >= 2) {
            QString storedUsername = parts[0].trimmed();
            QString storedHashedPassword = parts[1].trimmed();

            if (storedUsername == username && storedHashedPassword == hashedInputPassword) {
                file.close();
                return true;
            }
        }
    }

    file.close();
    return false;
}

void MainWindow::on_loginBtn_clicked()
{
    QString username = ui->usernameLineedit->text();
    QString password = ui->passwordLineedit->text();

    if (validateLogin(username, password)) {
        QMessageBox::information(this,"LogIn","Login Success");
        hide();
        dialog2 = new Dialog2(this);
        dialog2->show();
    }
    else{
        QMessageBox::warning(this,"LogIn","Wrong username or password");
    }
}

void MainWindow::onSignUpLabelClicked()
{
    this->close();
    signUp = new SignUp(this);
    signUp->show();
}