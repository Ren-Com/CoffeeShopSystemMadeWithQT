#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>

namespace Ui {
class SignUp;
}

class SignUp : public QDialog
{
    Q_OBJECT

public:
    explicit SignUp(QWidget *parent = nullptr);
    ~SignUp();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onSignUpButtonClicked();
    void onLoginLabelClicked();

private:
    Ui::SignUp *ui;
    bool saveAccountToFile(const QString &username, const QString &hashedPassword);
    bool isUsernameExists(const QString &username);
    bool loadAccountsFromFile(QVector<QPair<QString, QString>> &accounts);
};

#endif // SIGNUP_H
