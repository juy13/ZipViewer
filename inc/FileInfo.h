//
// Created by julian on 5/17/21.
//

#ifndef ZIPVIEWER_FILEINFO_H
#define ZIPVIEWER_FILEINFO_H

typedef struct FileInfo
{
    QString name;
    unsigned long int compressed_size{};
    unsigned long long uncompressed_size;

    FileInfo(QString, unsigned long long);
    FileInfo();
}FileInfo;

#endif //ZIPVIEWER_FILEINFO_H
