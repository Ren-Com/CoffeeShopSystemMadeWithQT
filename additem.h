#ifndef ADDITEM_H
#define ADDITEM_H

#include <QDialog>
#include "drinktablemodel.h"

namespace Ui {
class addItem;
}

class addItem : public QDialog
{
    Q_OBJECT

public:
    explicit addItem(QWidget *parent = nullptr, DrinksTableModel *model = nullptr);
    ~addItem();

private slots:
    void on_saveToCSVButton_clicked();
    void onSizeCheckboxChanged();

private:
    Ui::addItem *ui;
    DrinksTableModel *drinkModel;
    int getNextId();
};

#endif // ADDITEM_H