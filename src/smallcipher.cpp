#include "smallcipher.h"


SmallCipher::SmallCipher()
{

}


SmallCipher::SmallCipher(QString key)
{
    setKey(key);
}


QByteArray SmallCipher::intToBArray(int32_t x)
{
    // Put x to ret using big-endian
    QByteArray ret;
    ret += (char) (x >> 030 & 0xFF); // 1st byte
    ret += (char) (x >> 020 & 0xFF); // 2nd byte
    ret += (char) (x >> 010 & 0xFF); // 3rd byte
    ret += (char) (x >> 000 & 0xFF); // 4th byte

    return ret;
}


int32_t SmallCipher::bArrayToInt(QByteArray x)
{
    int ret;
    int len = x.length() > 4 ? 4 : x.length();

    for (int i = 0; i < len; i++)
        ret = ret << 8 | x[i];

    return ret;
}


QByteArray SmallCipher::encrypt(QByteArray data)
{
    for (int i = 0; i < key.length(); i++) {
        data[i % data.length()] = data[i % data.length()] + (char) key[i].unicode();
    }

    return data;
}


QByteArray SmallCipher::decrypt(QByteArray data)
{
    for (int i = 0; i < key.length(); i++) {
        data[i % data.length()] = data[i % data.length()] - (char) key[i].unicode();
    }

    return data;
}
