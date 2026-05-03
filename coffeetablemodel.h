#ifndef COFFEETABLEMODEL_H
#define COFFEETABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include "coffee.h"

class CoffeeTableModel : public QAbstractTableModel
{
    Q_OBJECT

private:
    QVector<Coffee> coffees;
    QStringList headers;
    QString currentFilePath;
    bool isEditableColumn(int column) const;

public:
    explicit CoffeeTableModel(QObject *parent = nullptr);

    // Override virtual methods dari QAbstractTableModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool validatePrice(double price) const;
    bool validateQuantity(int quantity) const;

    // Custom methods
    void loadDataFromCSV(const QString &filePath);
    bool saveToCSV();
    void clear();
    int getRowCount() const;
    double calculateTotalRevenue() const;

signals:
    void dataModified();
};

#endif // COFFEETABLEMODEL_H