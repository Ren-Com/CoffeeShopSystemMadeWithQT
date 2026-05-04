#include "additem.h"
#include "ui_additem.h"

addItem::addItem(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addItem)
{
    ui->setupUi(this);
}

addItem::~addItem()
{
    delete ui;
}
