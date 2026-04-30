#include "csvreader.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

CSVReader::CSVReader(const QString &fileName) : fileName(fileName) {}

bool CSVReader::fileExists() const
{
    return QFile::exists(fileName);
}

QStringList CSVReader::parseCSVLine(const QString &line) const
{
    QStringList fields;
    bool inQuote = false;
    QString currentField;

    for (int i = 0; i < line.length(); i++) {
        QChar ch = line[i];

        if (ch == '"') {
            inQuote = !inQuote;
        } else if (ch == ',' && !inQuote) {
            fields.append(currentField);
            currentField.clear();
        } else {
            currentField.append(ch);
        }
    }
    fields.append(currentField);

    // Clean up quotes
    for (int i = 0; i < fields.size(); i++) {
        if (fields[i].startsWith('"') && fields[i].endsWith('"')) {
            fields[i] = fields[i].mid(1, fields[i].length() - 2);
        }
        fields[i] = fields[i].trimmed();
    }

    return fields;
}

QVector<QStringList> CSVReader::readAll() const
{
    QVector<QStringList> allData;
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << fileName;
        return allData;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.trimmed().isEmpty()) {
            allData.append(parseCSVLine(line));
        }
    }

    file.close();
    return allData;
}

QStringList CSVReader::readHeader() const
{
    QVector<QStringList> allData = readAll();
    if (!allData.isEmpty()) {
        return allData.first();
    }
    return QStringList();
}

QVector<QStringList> CSVReader::readData() const
{
    QVector<QStringList> allData = readAll();
    if (allData.size() > 1) {
        QVector<QStringList> dataOnly;
        for (int i = 1; i < allData.size(); i++) {
            dataOnly.append(allData[i]);
        }
        return dataOnly;
    }
    return QVector<QStringList>();
}