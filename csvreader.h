#ifndef CSVREADER_H
#define CSVREADER_H

#include <QString>
#include <QStringList>
#include <QVector>

class CSVReader
{
private:
    QString fileName;
    QStringList parseCSVLine(const QString &line) const;

public:
    explicit CSVReader(const QString &fileName);

    QVector<QStringList> readAll() const;
    QStringList readHeader() const;
    QVector<QStringList> readData() const;
    bool fileExists() const;
};

#endif