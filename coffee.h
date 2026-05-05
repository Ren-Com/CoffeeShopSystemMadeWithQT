#ifndef COFFEE_H
#define COFFEE_H

#include <QString>

class Coffee
{
private:
    QString name;
    int id;
    double price;
    QString size;
    int quantitySold;
    QString explanation;

public:
    Coffee();
    Coffee(QString name, int id, double price, QString size, int quantitySold, QString explanation);

    // ini getter nya
    QString getName() const;
    int getId() const;
    double getPrice() const;
    QString getSize() const;
    int getQuantitySold() const;
    QString getExplanation() const;

    // ini setter nya
    void setName(const QString &name);
    void setId(int id);
    void setPrice(double price);
    void setSize(const QString &size);
    void setQuantitySold(int quantitySold);
    void setExplanation(const QString &explanation);
};

#endif // COFFEE_H