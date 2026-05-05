#include "Drinks.h"

Drinks::Drinks() : id(0), price(0.0), quantitySold(0) {}

Drinks::Drinks(QString name, int id, double price, QString size, int quantitySold, QString explanation)
    : name(name), id(id), price(price), size(size), quantitySold(quantitySold), explanation(explanation) {}

// ini getter
QString Drinks::getName() const { return name; }
int Drinks::getId() const { return id; }
double Drinks::getPrice() const { return price; }
QString Drinks::getSize() const { return size; }
int Drinks::getQuantitySold() const { return quantitySold; }
QString Drinks::getExplanation() const { return explanation; }

// ini setter, gtw tapi di youtube gitu
void Drinks::setName(const QString &name) { this->name = name; }
void Drinks::setId(int id) { this->id = id; }
void Drinks::setPrice(double price) { this->price = price; }
void Drinks::setSize(const QString &size) { this->size = size; }
void Drinks::setQuantitySold(int quantitySold) { this->quantitySold = quantitySold; }
void Drinks::setExplanation(const QString &explanation) { this->explanation = explanation; }