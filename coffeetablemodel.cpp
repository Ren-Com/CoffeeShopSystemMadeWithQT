#include "coffeetablemodel.h"
#include "csvreader.h"
#include <QDebug>

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

void CoffeeTableModel::loadDataFromCSV(const QString &filePath)
{
    beginResetModel();
    coffees.clear();

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