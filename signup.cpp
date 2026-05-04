#include "signup.h"
#include "ui_signup.h"
#include "mainwindow.h"
#include "hashutils.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>

SignUp::SignUp(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SignUp)
{
    ui->setupUi(this);

    // Setup loginLabel menjadi clickable
    if (ui->loginLabel) {
        ui->loginLabel->installEventFilter(this);
        ui->loginLabel->setCursor(Qt::PointingHandCursor);
    }

    // Connect sign up button
    connect(ui->signUpButton, &QPushButton::clicked, this, &SignUp::onSignUpButtonClicked);
}

SignUp::~SignUp()
{
    delete ui;
}

bool SignUp::eventFilter(QObject *watched, QEvent *event)
{
    // Deteksi klik pada loginLabel
    if (watched == ui->loginLabel && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            onLoginLabelClicked();
            return true;
        }
    }
    return QDialog::eventFilter(watched, event);
}

bool SignUp::loadAccountsFromFile(QVector<QPair<QString, QString>> &accounts)
{
    // Path ke file account.txt
    QString filePath = "../../database/account.txt";

    // Alternatif path jika tidak ditemukan
    if (!QFile::exists(filePath)) {
        filePath = "/home/ren-e/Desktop/project/CoffeeShopSystemMadeWithQT/database/account.txt";
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // File belum ada, itu OK
        qDebug() << "Account file not found, will create new one";
        return true;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty()) continue;

        // Format: username,hashed_password
        QStringList parts = line.split(',');
        if (parts.size() >= 2) {
            accounts.append({parts[0].trimmed(), parts[1].trimmed()});
        }
    }

    file.close();
    return true;
}

bool SignUp::isUsernameExists(const QString &username)
{
    QVector<QPair<QString, QString>> accounts;
    loadAccountsFromFile(accounts);

    for (const auto &account : accounts) {
        if (account.first == username) {
            return true;
        }
    }
    return false;
}

bool SignUp::saveAccountToFile(const QString &username, const QString &hashedPassword)
{
    // Path ke file account.txt
    QString filePath = "../../database/account.txt";

    // Buat direktori jika belum ada
    QDir dir("../../database");
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    // Alternatif absolute path
    if (!QFile::exists(filePath)) {
        filePath = "/home/ren-e/Desktop/project/CoffeeShopSystemMadeWithQT/database/account.txt";
        QDir absDir("/home/ren-e/Desktop/project/CoffeeShopSystemMadeWithQT/database");
        if (!absDir.exists()) {
            absDir.mkpath(".");
        }
    }

    QFile file(filePath);

    // Buka file dalam mode append
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Failed to open account file for writing:" << filePath;
        return false;
    }

    QTextStream out(&file);
    // Format: username,hashed_password
    out << username << "," << hashedPassword << "\n";

    file.close();
    qDebug() << "Account saved to:" << filePath;
    return true;
}

void SignUp::onSignUpButtonClicked()
{
    QString username = ui->signUpUSername_lineEdit->text().trimmed();
    QString password1 = ui->password1_lineEdit->text();
    QString password2 = ui->password2_lineEdit->text();

    // Validasi input
    if (username.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter username!");
        return;
    }

    if (password1.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter password!");
        return;
    }

    if (password1 != password2) {
        QMessageBox::warning(this, "Validation Error", "Passwords do not match!");
        return;
    }

    // Cek apakah username sudah ada
    if (isUsernameExists(username)) {
        QMessageBox::warning(this, "Validation Error",
                             "Username already exists!\nPlease choose another username.");
        return;
    }

    // Hash password dengan SHA-256
    QString hashedPassword = HashUtils::sha256(password1);
    qDebug() << "Hashed password for" << username << ":" << hashedPassword;

    // Simpan ke file account.txt
    if (saveAccountToFile(username, hashedPassword)) {
        QMessageBox::information(this, "Success",
                                 "Account created successfully!\nPlease login with your new account.");

        // Tutup SignUp dan buka MainWindow
        this->close();
        MainWindow *mainWindow = new MainWindow();
        mainWindow->show();
    } else {
        QMessageBox::critical(this, "Error",
                              "Failed to save account!\nPlease check file permissions.");
    }
}

void SignUp::onLoginLabelClicked()
{
    // Tutup SignUp dan buka MainWindow
    this->close();
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
}