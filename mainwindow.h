#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialog2.h"
#include "signup.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void on_loginBtn_clicked();
    void onSignUpLabelClicked();

private:
    Ui::MainWindow *ui;
    Dialog2 * dialog2;
    SignUp * signUp;

    bool validateLogin(const QString &username, const QString &password);
};
#endif // MAINWINDOW_H
