#include "additem.h"
#include "ui_additem.h"
#include <QMessageBox>
#include <QDebug>
#include <QDoubleValidator>
#include <QIntValidator>

addItem::addItem(QWidget *parent, CoffeeTableModel *model)
    : QDialog(parent)
    , ui(new Ui::addItem)
    , coffeeModel(model)
{
    ui->setupUi(this);

    //validator untuk price dan quantity sold (hanya angka)
    ui->price_lineEdit->setValidator(new QDoubleValidator(0, 999999, 2, this));
    ui->quantitySold_lineEdit->setValidator(new QIntValidator(0, 999999, this));

    // Hubungkan stateChanged dari setiap checkbox ke slot yang sama
    connect(ui->S_checkBox, &QCheckBox::stateChanged, this, &addItem::onSizeCheckboxChanged);
    connect(ui->M_checkBox, &QCheckBox::stateChanged, this, &addItem::onSizeCheckboxChanged); //--->biarin aja warningnya, yang penting gak eror
    connect(ui->L_checkBox, &QCheckBox::stateChanged, this, &addItem::onSizeCheckboxChanged); // bingung mau diperbaiki
}

addItem::~addItem()
{
    delete ui;
}

// Slot untuk menangani perubahan checkbox ukuran
void addItem::onSizeCheckboxChanged()
{
    // Hitung berapa checkbox yang sedang dicentang
    int checkedCount = 0;
    if (ui->S_checkBox->isChecked()) checkedCount++;
    if (ui->M_checkBox->isChecked()) checkedCount++;
    if (ui->L_checkBox->isChecked()) checkedCount++;

    // Jika lebih dari 1 dicentang, uncheck yang baru saja dicentang
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
    if (!coffeeModel) {
        qDebug() << "Model is null!";
        return 1;
    }

    int maxId = 0;
    int rowCount = coffeeModel->getRowCount();

    for (int i = 0; i < rowCount; i++) {
        QModelIndex index = coffeeModel->index(i, 1); // Kolom ID adalah kolom 1
        int currentId = coffeeModel->data(index).toInt();
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

    // Validasi size checkboxes - PASTIKAN HANYA SATU YANG DIPILIH
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

    // Cek apakah tidak ada ukuran yang dipilih
    if (selectedCount == 0) {
        QMessageBox::warning(this, "Validation Error",
                             "Please select a size (S/M/L)!");
        return;
    }

    // Cek memilih lebih dari satu ukuran
    if (selectedCount > 1) {
        QMessageBox::warning(this, "Validation Error",
                             "You can only select ONE size!\n"
                             "Please select only S, M, or L, not multiple sizes.");
        return;
    }

    // Cek field wajib
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

    // Validasi nilai
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

    // 2. Bikin ID baru
    int newId = getNextId();

    // 3. Tambahkan data ke model
    if (coffeeModel) {
        // Buat coffee object baru
        Coffee newCoffee(itemName, newId, price, selectedSize, quantity, explanation);

        // Tambahkan ke model
        coffeeModel->addCoffee(newCoffee);

        if (coffeeModel->saveToCSV()) {
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

            // Clear form
            ui->itemName_lineEdit->clear();
            ui->price_lineEdit->clear();
            ui->quantitySold_lineEdit->clear();
            ui->explanation_lineEdit->clear();
            ui->S_checkBox->setChecked(false);
            ui->M_checkBox->setChecked(false);
            ui->L_checkBox->setChecked(false);

            // tutup dialog setelah sukses (uncomment)!!! simpen aja dlu
            // accept();
        } else {
            QMessageBox::critical(this, "Error", "Failed to save data to CSV file!");
        }
    } else {
        QMessageBox::critical(this, "Error", "Model not available!");
    }
}