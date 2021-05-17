//
// Created by julian on 5/17/21.
//

#include <QtCore/QString>
#include "inc/FileInfo.h"

FileInfo::FileInfo(QString name, unsigned long long int uncompressed_size) {
    this->name = std::move(name);
    this->uncompressed_size = uncompressed_size;
}

FileInfo::FileInfo() {

}