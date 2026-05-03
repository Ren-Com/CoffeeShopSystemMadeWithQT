#include "dialog2.h"
#include "ui_dialog2.h"
#include "coffeetablemodel.h"
#include <QHeaderView>
#include <QCoreApplication>
#include <QDebug>
#include <QVBoxLayout>
#include <QFile>
#include <algorithm>
#include <QtCharts/QHorizontalBarSeries>  // Tambahkan ini
#include <QDir>
#include <QMessageBox>

// Include Charts
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

Dialog2::Dialog2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog2),
    chartView(nullptr),
    currentChart(nullptr)
{
    ui->setupUi(this);

    // Buat model
    coffeeModel = new CoffeeTableModel(this);
    ui->tableView->setModel(coffeeModel);

    // Atur tampilan tabel
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Setup chart
    setupChart();

    // edit triggers
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

    // Load data
    loadData();
    updateTotalRevenue();

    // Update chart setelah window ditampilkan
    QTimer::singleShot(100, this, &Dialog2::delayedUpdateChart);

    connect(coffeeModel, &CoffeeTableModel::dataModified, this, &Dialog2::onDataModified);
}

Dialog2::~Dialog2()
{
    // Bersihkan dengan aman
    if (currentChart) {
        currentChart->deleteLater();
        currentChart = nullptr;
    }
    if (chartView) {
        chartView->deleteLater();
        chartView = nullptr;
    }
    delete ui;
}

void Dialog2::onDataModified()
{
    qDebug() << "=== Data Modified - Updating revenue and chart ===";
    updateTotalRevenue();
    updateChart();
    updateBestSellingCoffee();

    if (coffeeModel->saveToCSV()) {
        qDebug() << "Changes auto-saved to CSV successfully";
    }
}

void Dialog2::delayedUpdateChart()
{
    updateChart();
    updateBestSellingCoffee();
}

void Dialog2::setupChart()
{
    // Bersihkan layout lama
    QLayout *oldLayout = ui->chartWidget->layout();
    if (oldLayout) {
        delete oldLayout;
    }

    // Buat chart view
    chartView = new QChartView(this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(400, 350);

    // Buat layout baru
    QVBoxLayout *layout = new QVBoxLayout(ui->chartWidget);
    layout->addWidget(chartView);
    layout->setContentsMargins(0, 0, 0, 0);
    ui->chartWidget->setLayout(layout);

    qDebug() << "ChartView setup completed";
}

void Dialog2::showEvent(QShowEvent *event)
{
    qDebug() << "Dialog2 showEvent called";
    QDialog::showEvent(event);
}

void Dialog2::loadData()
{
    QString filePath = "../../database/dataSet.csv"; //-------------> Buat jadi relatif [Important!!!!!] Done

    qDebug() << "Loading data from:" << filePath;

    if (!QFile::exists(filePath)) {
        qDebug() << "ERROR: File not found!";
        return;
    }

    coffeeModel->loadDataFromCSV(filePath);
    qDebug() << "Loaded" << coffeeModel->getRowCount() << "coffee items";

    ui->tableView->resizeColumnsToContents();

    updateBestSellingCoffee();
}

void Dialog2::updateTotalRevenue()
{
    double totalRevenue = coffeeModel->calculateTotalRevenue();
    QString formattedRevenue = QString("RMB %1").arg(totalRevenue, 0, 'f', 2);
    ui->totalSalesRavenue->setText(QString("<html><head/><body><p><span style=' font-size:24pt; font-weight:700;'>%1</span></p></body></html>").arg(formattedRevenue));
    qDebug() << "Total Revenue:" << formattedRevenue;
}

void Dialog2::updateBestSellingCoffee()
{
    QString bestCoffee = coffeeModel->getBestSellingCoffeeName();

    if (bestCoffee.isEmpty() || bestCoffee == "No Data") {
        ui->bestSellingCoffee->setText(QString("<html><head/><body><p><span style=' font-size:24pt; font-weight:700;'>No Data</span></p></body></html>"));
    } else {
        ui->bestSellingCoffee->setText(QString("<html><head/><body><p><span style=' font-size:24pt; font-weight:700;'>%1</span></p></body></html>").arg(bestCoffee));
    }

    qDebug() << "Best Selling Coffee:" << bestCoffee;
}

void Dialog2::updateChart()
{
    qDebug() << "=== Starting updateChart ===";

    if (!chartView) {
        qDebug() << "chartView is null!";
        return;
    }

    if (coffeeModel->getRowCount() == 0) {
        qDebug() << "No data in model!";
        return;
    }

    // Hapus chart lama dengan aman
    if (currentChart) {
        currentChart->deleteLater();
        currentChart = nullptr;
    }

    // Kumpulkan data revenue
    QVector<QPair<QString, double>> items;

    for (int i = 0; i < coffeeModel->getRowCount(); i++) {
        QString name = coffeeModel->data(coffeeModel->index(i, 0)).toString();
        double price = coffeeModel->data(coffeeModel->index(i, 2)).toDouble();
        int quantity = coffeeModel->data(coffeeModel->index(i, 4)).toInt();
        double revenue = price * quantity;

        if (revenue > 0) {
            items.append({name, revenue});
        }
    }

    if (items.isEmpty()) {
        qDebug() << "No revenue data found";
        return;
    }

    // ========== URUTKAN DARI TERKECIL KE TERBESAR ==========
    // Agar yang terbesar muncul di bagian atas horizontal chart
    std::sort(items.begin(), items.end(), [](const QPair<QString, double> &a, const QPair<QString, double> &b) {
        return a.second < b.second;  // Ascending (kecil ke besar)
    });

    // Ambil top 8 (8 terbesar akan berada di akhir array)
    const int maxItems = 8;
    if (items.size() > maxItems) {
        items = items.mid(items.size() - maxItems);
    }

    qDebug() << "Creating HORIZONTAL bar chart with" << items.size() << "items";

    // Buat chart baru
    currentChart = new QChart();
    currentChart->setTitle("Top Coffee Revenue");
    currentChart->setAnimationOptions(QChart::NoAnimation);
    currentChart->setBackgroundBrush(QBrush(Qt::white));

    // Horizontal Bar Chart
    QHorizontalBarSeries *series = new QHorizontalBarSeries();
    QBarSet *barSet = new QBarSet("Revenue (RMB)");

    QStringList categories;
    for (const auto &item : items) {
        *barSet << item.second;
        categories.append(item.first);
        qDebug() << item.first << ":" << item.second;
    }

    series->append(barSet);

    currentChart->addSeries(series);

    //Y untuk nama menu
    QBarCategoryAxis *axisY = new QBarCategoryAxis();
    axisY->append(categories);
    axisY->setTitleText("Coffee Menu");
    axisY->setLabelsFont(QFont("Arial", 9));

    //X untuk nilai revenue
    QValueAxis *axisX = new QValueAxis();
    double maxRevenue = items.last().second;
    axisX->setRange(0, maxRevenue * 1.2);
    axisX->setTitleText("Revenue (RMB)");
    axisX->setLabelFormat("%.0f");

    currentChart->addAxis(axisX, Qt::AlignBottom);
    currentChart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    currentChart->legend()->setVisible(false);

    chartView->setChart(currentChart);

    qDebug() << "=== Horizontal chart update completed successfully ===";
}