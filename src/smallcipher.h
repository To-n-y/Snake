#ifndef SMALLCIPHER_H
#define SMALLCIPHER_H

#include <QString>
#include <QByteArray>


class SmallCipher
{

private:

    QString key;

public:

    SmallCipher();
    SmallCipher(QString key);

    void setKey(QString key) { this->key = key; }

    /*! Converts a 32-bit integer to big-endian QByteArray ([0] is a highest byte) */
    static QByteArray intToBArray(int32_t x);

    /*! Converts a big-endian QByteArray to 32-bit integer */
    static int32_t bArrayToInt(QByteArray x);

    /*! Provides a simple (NOT STRONG) encryption */
    QByteArray encrypt(QByteArray data);

    /*! Decrypts a byte array that was encrypted by SmallCipher::encrypt(QByteArray) */
    QByteArray decrypt(QByteArray data);

};

#endif // SMALLCIPHER_H
