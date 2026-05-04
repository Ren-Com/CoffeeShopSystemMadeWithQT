#ifndef HASHUTILS_H
#define HASHUTILS_H

#include <QString>
#include <QCryptographicHash>

class HashUtils {
public:
    static QString sha256(const QString &input) {
        QByteArray hash = QCryptographicHash::hash(input.toUtf8(), QCryptographicHash::Sha256);
        return QString(hash.toHex());
    }
};

#endif // HASHUTILS_H