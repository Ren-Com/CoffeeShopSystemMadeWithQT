#ifndef DRINKTABLEMODEL_H
#define DRINKTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include "Drinks.h"

class DrinksTableModel : public QAbstractTableModel
{
    Q_OBJECT

private:
    QVector<Drinks> drinksList;
    QStringList headers;
    QString currentFilePath;
    bool isEditableColumn(int column) const;

public:
    explicit DrinksTableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool validatePrice(double price) const;
    bool validateQuantity(int quantity) const;

    void loadDataFromCSV(const QString &filePath);
    bool saveToCSV();
    void clear();
    int getRowCount() const;
    double calculateTotalRevenue() const;
    QString getBestSellingDrinksName() const;

    void addDrink(const Drinks &drink);
    bool removeRow(int row);

signals:
    void dataModified();
};

#endif // DRINKTABLEMODEL_H
