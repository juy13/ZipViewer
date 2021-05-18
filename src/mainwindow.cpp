//
// Created by julian on 5/13/21.
//

#include <iostream>
#include <utility>
#include "inc/mainwindow.h"
#include "QHeaderView"
#include <QMessageBox>


ZipViewerWin::ZipViewerWin(QWidget *parent)
        : QMainWindow(parent)
{
    setCentralWidget(ZipViewerWid);
    this->nmNchsLayoutH->addWidget(nmFile);
    this->nmNchsLayoutH->addWidget(chsFile);
    this->table = new TableModel(this);
    this->ZipTable->setModel(table);
    this->ZipTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ZipLayoutV->addLayout(nmNchsLayoutH);
    ZipLayoutV->addWidget(ZipTable);
    ZipViewerWid->setLayout(ZipLayoutV);
    connect(chsFile, SIGNAL(clicked()), this, SLOT(chooseFl()));
    setFixedSize(300, 600);
}



void ZipViewerWin::on_actionExit_triggered()
{
    this->close();
    delete chsFile;
    delete nmFile;
    delete nmNchsLayoutH;
    delete ZipViewerWid;
}

void ZipViewerWin::chooseFl()
{
    if(table->rowCount(QModelIndex()) > 0)
    {
        QModelIndex index = QModelIndex();
        table->removeRows(0, table->rowCount(QModelIndex()), index);
    }
    fileInfoVec.clear();
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Save Address Book"), "",
                                                    tr("Zip files (*.zip)"));
    nmFile->setText(fileName);
    unzip_compat(fileName);
    resWin();

}

int32_t ZipViewerWin::unzip_compat(const QString& fileName) {
    unzFile unzip;
    int32_t err = UNZ_OK;

    unzip = unzOpen(fileName.toStdString().c_str());
    if (unzip == nullptr)
    {
        QMessageBox::critical(this, "Attention","Failed to open test zip file");
        //printf("Failed to open test zip file\n");
        return UNZ_PARAMERROR;
    }
    err = test_unzip_compat_int(unzip);
    unzClose(unzip);

    if (err != UNZ_OK)
        return err;

    return UNZ_OK;
}

int32_t ZipViewerWin::test_unzip_compat_int(unzFile unzip) {
    unz_global_info64 global_info64;
    unz_global_info global_info;
    unz_file_info64 file_info64;
    unz_file_info file_info;
    int32_t err = UNZ_OK;
    int32_t flEnd = UNZ_OK;
    bool flag = true;
    char filename[120];

    memset(&file_info, 0, sizeof(file_info));
    memset(&file_info64, 0, sizeof(file_info64));
    memset(&global_info, 0, sizeof(global_info));
    memset(&global_info64, 0, sizeof(global_info64));


    err = unzGetGlobalInfo(unzip, &global_info);
    if (err != UNZ_OK)
    {
        QString msg = QString("Failed to get global info  (") + err + ")";
        QMessageBox::critical(this, "Attention", msg);
        //printf("Failed to get global info  (%" PRId32 ")\n", err);
        return err;
    }
    err = unzGetGlobalInfo64(unzip, &global_info64);
    if (err != UNZ_OK)
    {
        QString msg = QString("Failed to get global info 64-bit (") + err + ")";
        QMessageBox::critical(this, "Attention", msg);
        //printf("Failed to get global info 64-bit (%" PRId32 ")\n", err);
        return err;
    }
    if (global_info.number_disk_with_CD != 0 || global_info64.number_disk_with_CD != 0)
    {
        QString msg = QString("Invalid disk with cd (") + err + ")";
        QMessageBox::critical(this, "Attention", msg);
        //printf("Invalid disk with cd (%" PRIu32 ")\n", global_info.number_disk_with_CD);
        return err;
    }

    while (flEnd != UNZ_END_OF_LIST_OF_FILE)
    {
        if (flag)
        {
            flEnd = unzGoToFirstFile(unzip);
            flag = false;
        }
        else
        {
            flEnd = unzGoToNextFile(unzip);
        }

        if (flEnd == UNZ_OK)
        {
            err = unzGetCurrentFileInfo64(unzip, &file_info64, filename, sizeof(filename), nullptr, 0, nullptr, 0);
            if(err == UNZ_OK)
            {
                FileInfo fl;
                fl.name = nmFile->text() + '/' + filename;
                fl.uncompressed_size = file_info64.uncompressed_size;
                fileInfoVec.push_back(fl);
            }
        }
    }

    int row = 0;
    QModelIndex index = QModelIndex();
    for(const auto& i : fileInfoVec)
    {
        //std::cout << i.name.toStdString() << " " << i.uncompressed_size << " " << std::endl;
        if(row == 0)
        {
            table->insertRows(row, 1, QModelIndex());
        } else {
            table->insertRows(row, 1, index);
        }
        index = table->index(row, 0, index);
        if( !table->setData(index, i.name, Qt::EditRole))
        {
            QMessageBox::critical(this, "Attention", "Can't set data");
            return UNZ_ERRNO;
        }
        index = table->index(row, 1, index);
        if( !table->setData(index, i.uncompressed_size, Qt::EditRole))
        {
            QMessageBox::critical(this, "Attention", "Can't set data");
            return UNZ_ERRNO;
        }

        row++;
    }

    return UNZ_OK;

}

void ZipViewerWin::resWin() {

    int len1 = this->ZipTable->columnWidth(0);
    int len2 = this->ZipTable->columnWidth(1);

    std::cout << len1 << " " << len2 << std::endl;

    setFixedSize(len1 + len2 + 30, 600);
    std::cout << "Hi" << std::endl;

}


