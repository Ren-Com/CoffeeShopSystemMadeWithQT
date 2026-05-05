#include "additem.h"
#include "ui_additem.h"
#include <QMessageBox>
#include <QDebug>
#include <QDoubleValidator>
#include <QIntValidator>

addItem::addItem(QWidget *parent, DrinksTableModel *model)
    : QDialog(parent)
    , ui(new Ui::addItem)
    , drinkModel(model)
{
    ui->setupUi(this);

    //validator untuk price sama quantity sold
    ui->price_lineEdit->setValidator(new QDoubleValidator(0, 999999, 2, this));
    ui->quantitySold_lineEdit->setValidator(new QIntValidator(0, 999999, this));

    connect(ui->S_checkBox, &QCheckBox::stateChanged, this, &addItem::onSizeCheckboxChanged);
    connect(ui->M_checkBox, &QCheckBox::stateChanged, this, &addItem::onSizeCheckboxChanged);
    connect(ui->L_checkBox, &QCheckBox::stateChanged, this, &addItem::onSizeCheckboxChanged);
}

addItem::~addItem()
{
    delete ui;
}

void addItem::onSizeCheckboxChanged()
{
    int checkedCount = 0;
    if (ui->S_checkBox->isChecked()) checkedCount++;
    if (ui->M_checkBox->isChecked()) checkedCount++;
    if (ui->L_checkBox->isChecked()) checkedCount++;

    // uncheck yang baru si user dicentang
    if (checkedCount > 1) {
        QCheckBox *senderBox = qobject_cast<QCheckBox*>(sender());
        if (senderBox) {
            senderBox->setChecked(false);
        }

        QMessageBox::warning(this, "Invalid Selection",
                             "You can only select ONE size (S, M, or L)!\n"
                             "Please select only one size for the coffee.");
    }
}

int addItem::getNextId()
{
    if (!drinkModel) {
        qDebug() << "Model is null!";
        return 1;
    }

    int maxId = 0;
    int rowCount = drinkModel->getRowCount();

    for (int i = 0; i < rowCount; i++) {
        QModelIndex index = drinkModel->index(i, 1); // Kolom ID nya kolom 1
        int currentId = drinkModel->data(index).toInt();
        if (currentId > maxId) {
            maxId = currentId;
        }
    }

    return maxId + 1;
}

void addItem::on_saveToCSVButton_clicked()
{
    // 1. Validasi input
    QString itemName = ui->itemName_lineEdit->text().trimmed();
    QString priceText = ui->price_lineEdit->text().trimmed();
    QString quantityText = ui->quantitySold_lineEdit->text().trimmed();
    QString explanation = ui->explanation_lineEdit->text().trimmed();

    // Validasi size checkboxes
    QString selectedSize;
    int selectedCount = 0;

    if (ui->S_checkBox->isChecked()) {
        selectedSize = "S";
        selectedCount++;
    }
    if (ui->M_checkBox->isChecked()) {
        selectedSize = "M";
        selectedCount++;
    }
    if (ui->L_checkBox->isChecked()) {
        selectedSize = "L";
        selectedCount++;
    }

    // tidak ada ukuran yang dipilih
    if (selectedCount == 0) {
        QMessageBox::warning(this, "Validation Error",
                             "Please select a size (S/M/L)!");
        return;
    }

    // kondisi kalau usr milih lebih dari satu ukuran
    if (selectedCount > 1) {
        QMessageBox::warning(this, "Validati0n Error", "You can only select ONE size!\n""Please select only S, M, or L, not multiple sizes.");
        return;
    }

    // ngecek kotak yang wajib diisi
    if (itemName.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter item name!");
        return;
    }

    if (priceText.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter price!");
        return;
    }

    if (quantityText.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter quantity sold!");
        return;
    }

    // validasi nilainya
    bool priceOk, quantityOk;
    double price = priceText.toDouble(&priceOk);
    int quantity = quantityText.toInt(&quantityOk);

    if (!priceOk || price < 0) {
        QMessageBox::warning(this, "Validation Error", "Invalid price value!");
        return;
    }

    if (!quantityOk || quantity < 0) {
        QMessageBox::warning(this, "Validation Error", "Invalid quantity value!");
        return;
    }

    // 2. bikin ID baru
    int newId = getNextId();

    // 3. masukin data ke model
    if (drinkModel) {
        Drinks newDrink(itemName, newId, price, selectedSize, quantity, explanation);
        drinkModel->addDrink(newDrink);

        if (drinkModel->saveToCSV()) {
            QMessageBox::information(this, "Success",
                                     QString("Item added successfully!\n"
                                             "ID: %1\n"
                                             "Name: %2\n"
                                             "Price: %3\n"
                                             "Size: %4\n"
                                             "Quantity Sold: %5")
                                         .arg(newId)
                                         .arg(itemName)
                                         .arg(price)
                                         .arg(selectedSize)
                                         .arg(quantity));

            // clear form
            ui->itemName_lineEdit->clear();
            ui->price_lineEdit->clear();
            ui->quantitySold_lineEdit->clear();
            ui->explanation_lineEdit->clear();
            ui->S_checkBox->setChecked(false);
            ui->M_checkBox->setChecked(false);
            ui->L_checkBox->setChecked(false);

        } else {
            QMessageBox::critical(this, "Error", "Failed to save data to CSV file!");
        }
    } else {
        QMessageBox::critical(this, "Error", "Model not available!");
    }
}