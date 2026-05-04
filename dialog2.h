#ifndef DIALOG2_H
#define DIALOG2_H
#include <QObject>
#include <QDialog>
#include <QVector>
#include <QTimer>
#include "additem.h"

//Forward declaration
class CoffeeTableModel;

namespace Ui {
class Dialog2;
}

// Forward declaration untuk QtCharts classes
QT_BEGIN_NAMESPACE
class QChartView;
class QChart;
class QBarSeries;
class QBarSet;
QT_END_NAMESPACE

class Dialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog2(QWidget *parent = nullptr);
    ~Dialog2();

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void delayedUpdateChart();
    void onDataModified();
    void on_deletButton_clicked();
    void on_addButton_clicked();

private:
    void loadData();
    void updateTotalRevenue();
    void updateBestSellingCoffee();
    void updateChart();
    void setupChart();

    Ui::Dialog2 *ui;
    CoffeeTableModel *coffeeModel;
    QChartView *chartView;
    QChart *currentChart;  // Simpan chart reference
    addItem * additem;
};

#endif // DIALOG2_H