#ifndef ADDITEM_H
#define ADDITEM_H

#include <QDialog>
#include "coffeetablemodel.h"

namespace Ui {
class addItem;
}

class addItem : public QDialog
{
    Q_OBJECT

public:
    explicit addItem(QWidget *parent = nullptr, CoffeeTableModel *model = nullptr);
    ~addItem();

private slots:
    void on_saveToCSVButton_clicked();
    void onSizeCheckboxChanged();

private:
    Ui::addItem *ui;
    CoffeeTableModel *coffeeModel;  // Reference ke model utama
    int getNextId();  // Method untuk mendapatkan ID berikutnya
};

#endif // ADDITEM_H