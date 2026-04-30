#include "coffee.h"

Coffee::Coffee() : id(0), price(0.0), quantitySold(0) {}

Coffee::Coffee(QString name, int id, double price, QString size, int quantitySold, QString explanation)
    : name(name), id(id), price(price), size(size), quantitySold(quantitySold), explanation(explanation) {}

// Getter implementations
QString Coffee::getName() const { return name; }
int Coffee::getId() const { return id; }
double Coffee::getPrice() const { return price; }
QString Coffee::getSize() const { return size; }
int Coffee::getQuantitySold() const { return quantitySold; }
QString Coffee::getExplanation() const { return explanation; }

// Setter implementations
void Coffee::setName(const QString &name) { this->name = name; }
void Coffee::setId(int id) { this->id = id; }
void Coffee::setPrice(double price) { this->price = price; }
void Coffee::setSize(const QString &size) { this->size = size; }
void Coffee::setQuantitySold(int quantitySold) { this->quantitySold = quantitySold; }
void Coffee::setExplanation(const QString &explanation) { this->explanation = explanation; }