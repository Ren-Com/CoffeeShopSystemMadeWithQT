#include "drinktablemodel.h"
#include "csvreader.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

DrinksTableModel::DrinksTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    headers = {"Name", "ID", "Price", "Size", "Quantity Sold", "Explanation"};
}

int DrinksTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return drinksList.size();
}

int DrinksTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return 6;
}

QVariant DrinksTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    const Drinks &drink = drinksList.at(index.row());

    switch (index.column()) {
    case 0: return drink.getName();
    case 1: return drink.getId();
    case 2: return QString::number(drink.getPrice(), 'f', 2);
    case 3: return drink.getSize();
    case 4: return drink.getQuantitySold();
    case 5: return drink.getExplanation();
    default: return QVariant();
    }
}

QVariant DrinksTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section < headers.size()) {
            return headers[section];
        }
    }
    return QVariant();
}

Qt::ItemFlags DrinksTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    if (index.column() == 1) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    }
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool DrinksTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    Drinks &drink = drinksList[index.row()];
    bool changed = false;

    switch (index.column()) {
    case 0: // Name
        if (!value.toString().isEmpty()) {
            drink.setName(value.toString());
            changed = true;
        }
        break;

    case 1:
        qDebug() << "ID cannot be edited";
        return false;

    case 2: // Price
    {
        bool ok;
        double newPrice = value.toDouble(&ok);
        if (ok && newPrice >= 0 && validatePrice(newPrice)) {
            drink.setPrice(newPrice);
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
        //harus S, M, atau L
        if (newSize == "S" || newSize == "M" || newSize == "L") {
            drink.setSize(newSize);
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
            drink.setQuantitySold(newQuantity);
            changed = true;
        } else {
            qDebug() << "Invalid quantity:" << value.toString();
            return false;
        }
        break;
    }

    case 5: // Explanation
        if (!value.toString().isEmpty()) {
            drink.setExplanation(value.toString());
            changed = true;
        }
        break;

    default:
        return false;
    }

    if (changed) {
        emit dataChanged(index, index);
        emit dataModified();
        return true;
    }

    return false;
}

// Method validasiiiii
bool DrinksTableModel::validatePrice(double price) const
{
    return price >= 0 && price <= 100; // Maksimal harga 100
}

bool DrinksTableModel::validateQuantity(int quantity) const
{
    return quantity >= 0;
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

bool DrinksTableModel::saveToCSV()
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

    for (const Drinks &drink : drinksList) {
        out << escapeCSV(drink.getName()) << ","
            << drink.getId() << ","
            << QString::number(drink.getPrice(), 'f', 2) << ","
            << drink.getSize() << ","
            << drink.getQuantitySold() << ","
            << escapeCSV(drink.getExplanation()) << "\n";
    }

    file.close();
    qDebug() << "Data successfully saved to:" << currentFilePath;
    return true;
}

void DrinksTableModel::loadDataFromCSV(const QString &filePath)
{
    beginResetModel();
    drinksList.clear();
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
            Drinks drink(
                row[0],                          // name
                row[1].toInt(),                  // id
                row[2].toDouble(),               // price
                row[3],                          // size
                row[4].toInt(),                  // quantitySold
                row[5]                           // explanation
                );
            drinksList.append(drink);
        }
    }

    endResetModel();
    qDebug() << "Loaded" << drinksList.size() << "drink items";
}


void DrinksTableModel::clear()
{
    beginResetModel();
    drinksList.clear();
    endResetModel();
}

int DrinksTableModel::getRowCount() const
{
    return drinksList.size();
}

double DrinksTableModel::calculateTotalRevenue() const
{
    double totalRevenue = 0.0;

    for (int i = 0; i < drinksList.size(); ++i) {  //---> kena warning jirr
        const Drinks &drink = drinksList.at(i);
        totalRevenue += drink.getPrice() * drink.getQuantitySold();
    }

    return totalRevenue;
}

QString DrinksTableModel::getBestSellingDrinksName() const
{
    if (drinksList.isEmpty()) {
        return "No Data";
    }

    int maxQuantity = -1;
    QString bestCoffeeName;

    for (int i = 0; i < drinksList.size(); ++i) {
        const Drinks &drink = drinksList.at(i);
        int quantity = drink.getQuantitySold();

        if (quantity > maxQuantity) {
            maxQuantity = quantity;
            bestCoffeeName = drink.getName();
        }
    }

    return bestCoffeeName;
}

void DrinksTableModel::addDrink(const Drinks &drink)
{
    beginInsertRows(QModelIndex(), drinksList.size(), drinksList.size());
    drinksList.append(drink);
    endInsertRows();

    emit dataModified();
}

bool DrinksTableModel::removeRow(int row)
{
    if (row < 0 || row >= drinksList.size()) {
        qDebug() << "Invalid row index:" << row;
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row);
    drinksList.removeAt(row);
    endRemoveRows();

    emit dataModified();
    return true;
}