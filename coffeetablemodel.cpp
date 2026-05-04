#include "coffeetablemodel.h"
#include "csvreader.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

CoffeeTableModel::CoffeeTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    headers = {"Name", "ID", "Price", "Size", "Quantity Sold", "Explanation"};
}

int CoffeeTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return coffees.size();
}

int CoffeeTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return 6;
}

QVariant CoffeeTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    const Coffee &coffee = coffees.at(index.row());

    switch (index.column()) {
    case 0: return coffee.getName();
    case 1: return coffee.getId();
    case 2: return QString::number(coffee.getPrice(), 'f', 2);
    case 3: return coffee.getSize();
    case 4: return coffee.getQuantitySold();
    case 5: return coffee.getExplanation();
    default: return QVariant();
    }
}

QVariant CoffeeTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section < headers.size()) {
            return headers[section];
        }
    }
    return QVariant();
}

Qt::ItemFlags CoffeeTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    // Semua kolom bisa diedit kecuali ID (kolom 1)
    if (index.column() == 1) {  // ID column tidak bisa diedit
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    }

    // Kolom lain bisa diedit
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool CoffeeTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    Coffee &coffee = coffees[index.row()];
    bool changed = false;

    switch (index.column()) {
    case 0: // Name
        if (!value.toString().isEmpty()) {
            coffee.setName(value.toString());
            changed = true;
        }
        break;

    case 1: // ID (sebenarnya tidak diedit, tapi jika ingin diijinkan)
        // Biarkan tetap atau beri warning
        qDebug() << "ID cannot be edited (auto-generated)";
        return false;

    case 2: // Price
    {
        bool ok;
        double newPrice = value.toDouble(&ok);
        if (ok && newPrice >= 0 && validatePrice(newPrice)) {
            coffee.setPrice(newPrice);
            changed = true;
        } else {
            qDebug() << "Invalid price:" << value.toString();
            return false;
        }
        break;
    }

    case 3: // Size
    {
        QString newSize = value.toString().toUpper();
        // Validasi size: harus S, M, atau L
        if (newSize == "S" || newSize == "M" || newSize == "L") {
            coffee.setSize(newSize);
            changed = true;
        } else {
            qDebug() << "Invalid size. Must be S, M, or L";
            return false;
        }
        break;
    }

    case 4: // Quantity Sold
    {
        bool ok;
        int newQuantity = value.toInt(&ok);
        if (ok && newQuantity >= 0 && validateQuantity(newQuantity)) {
            coffee.setQuantitySold(newQuantity);
            changed = true;
        } else {
            qDebug() << "Invalid quantity:" << value.toString();
            return false;
        }
        break;
    }

    case 5: // Explanation
        if (!value.toString().isEmpty()) {
            coffee.setExplanation(value.toString());
            changed = true;
        }
        break;

    default:
        return false;
    }

    if (changed) {
        emit dataChanged(index, index);
        // Optional: emit signal bahwa data berubah
        emit dataModified();
        return true;
    }

    return false;
}

// Method validasi (opsional)
bool CoffeeTableModel::validatePrice(double price) const
{
    return price >= 0 && price <= 100; // Maksimal harga 1000
}

bool CoffeeTableModel::validateQuantity(int quantity) const
{
    return quantity >= 0; // Maksimal quantity 10000
}

static QString escapeCSV(const QString &field)
{
    if (field.contains(',') || field.contains('"') || field.contains('\n')) {
        QString escaped = field;
        escaped.replace('"', "\"\"");
        return QString("\"%1\"").arg(escaped);
    }
    return field;
}

// Implementasi saveToCSV
bool CoffeeTableModel::saveToCSV()
{
    if (currentFilePath.isEmpty()) {
        qDebug() << "Error: No file path specified!";
        return false;
    }

    QFile file(currentFilePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error: Cannot open file for writing:" << currentFilePath;
        return false;
    }

    QTextStream out(&file);

    // Tulis header
    out << "name,id,price,size,quantity_sold,explanation\n";

    // Tulis semua data coffee yang sudah diedit
    for (const Coffee &coffee : coffees) {
        out << escapeCSV(coffee.getName()) << ","
            << coffee.getId() << ","
            << QString::number(coffee.getPrice(), 'f', 2) << ","
            << coffee.getSize() << ","
            << coffee.getQuantitySold() << ","
            << escapeCSV(coffee.getExplanation()) << "\n";
    }

    file.close();
    qDebug() << "Data successfully saved to:" << currentFilePath;
    return true;
}

void CoffeeTableModel::loadDataFromCSV(const QString &filePath)
{
    beginResetModel();
    coffees.clear();
    currentFilePath = filePath;

    CSVReader reader(filePath);

    if (!reader.fileExists()) {
        qDebug() << "File not found:" << filePath;
        endResetModel();
        return;
    }

    QVector<QStringList> data = reader.readData();

    for (const QStringList &row : data) {
        if (row.size() >= 6) {
            Coffee coffee(
                row[0],                          // name
                row[1].toInt(),                  // id
                row[2].toDouble(),               // price
                row[3],                          // size
                row[4].toInt(),                  // quantitySold
                row[5]                           // explanation
                );
            coffees.append(coffee);
        }
    }

    endResetModel();
    qDebug() << "Loaded" << coffees.size() << "coffee items";
}


void CoffeeTableModel::clear()
{
    beginResetModel();
    coffees.clear();
    endResetModel();
}

int CoffeeTableModel::getRowCount() const
{
    return coffees.size();
}

double CoffeeTableModel::calculateTotalRevenue() const
{
    double totalRevenue = 0.0;

    // Perbaiki range-loop untuk menghindari warning
    for (int i = 0; i < coffees.size(); ++i) {
        const Coffee &coffee = coffees.at(i);
        totalRevenue += coffee.getPrice() * coffee.getQuantitySold();
    }

    return totalRevenue;
}

QString CoffeeTableModel::getBestSellingCoffeeName() const
{
    if (coffees.isEmpty()) {
        return "No Data";
    }

    int maxQuantity = -1;
    QString bestCoffeeName;

    for (int i = 0; i < coffees.size(); ++i) {
        const Coffee &coffee = coffees.at(i);
        int quantity = coffee.getQuantitySold();

        if (quantity > maxQuantity) {
            maxQuantity = quantity;
            bestCoffeeName = coffee.getName();
        }
    }

    return bestCoffeeName;
}

void CoffeeTableModel::addCoffee(const Coffee &coffee)
{
    beginInsertRows(QModelIndex(), coffees.size(), coffees.size());
    coffees.append(coffee);
    endInsertRows();

    emit dataModified();
}

bool CoffeeTableModel::removeRow(int row)
{
    if (row < 0 || row >= coffees.size()) {
        qDebug() << "Invalid row index:" << row;
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row);
    coffees.removeAt(row);
    endRemoveRows();

    emit dataModified();
    return true;
}