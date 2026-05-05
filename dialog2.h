#ifndef DIALOG2_H
#define DIALOG2_H
#include <QObject>
#include <QDialog>
#include <QVector>
#include <QTimer>
#include "additem.h"

class DrinksTableModel;

namespace Ui {
class Dialog2;
}

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
    void updateBestSellingDrink();
    void updateChart();
    void setupChart();

    Ui::Dialog2 *ui;
    DrinksTableModel *drinkModel;
    QChartView *chartView;
    QChart *currentChart;
    addItem * additem;
};

#endif // DIALOG2_H