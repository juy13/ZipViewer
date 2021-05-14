//
// Created by julian on 5/13/21.
//

#include <iostream>
#include "inc/mainwindow.h"

ZipViewerWin::ZipViewerWin(QWidget *parent)
        : QMainWindow(parent)
{
    setCentralWidget(ZipViewerWid);
    this->nmNchsLayoutH->addWidget(nmFile);
    this->nmNchsLayoutH->addWidget(chsFile);
    ZipLayoutV->addLayout(nmNchsLayoutH);
    ZipLayoutV->addWidget(ZipTable);
    ZipViewerWid->setLayout(ZipLayoutV);
    connect(chsFile, SIGNAL(clicked()), this, SLOT(chooseFl()));
    setFixedSize(690, 600);
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
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Save Address Book"), "",
                                                    tr("Zip files (*.zip)"));
    nmFile->setText(fileName);
    test_unzip_compat(fileName);

}

int32_t ZipViewerWin::test_unzip_compat(QString fileName) {
    unzFile unzip;
    int32_t err = UNZ_OK;

    unzip = unzOpen(fileName.toStdString().c_str());
    if (unzip == nullptr)
    {
        printf("Failed to open test zip file\n");
        return UNZ_PARAMERROR;
    }
    err = test_unzip_compat_int(unzip);
    unzClose(unzip);

    if (err != UNZ_OK)
        return err;

    printf("Compat unzip.. OK\n");

    return UNZ_OK;
}

int32_t ZipViewerWin::test_unzip_compat_int(unzFile unzip) {
    unz_global_info64 global_info64;
    unz_global_info global_info;
    unz_file_info64 file_info64;
    unz_file_info file_info;
    unz_file_pos file_pos;
    int32_t err = UNZ_OK;
    int32_t flEnd = UNZ_OK;
    bool flag = true;
    int32_t bytes_read = 0;
    char comment[120];
    char filename[120];
    char buffer[120];
    char *test_data = "test data";

    memset(&file_info, 0, sizeof(file_info));
    memset(&file_info64, 0, sizeof(file_info64));
    memset(&global_info, 0, sizeof(global_info));
    memset(&global_info64, 0, sizeof(global_info64));

    comment[0] = 0;
    err = unzGetGlobalInfo(unzip, &global_info);
    if (err != UNZ_OK)
    {
        printf("Failed to get global info  (%" PRId32 ")\n", err);
        return err;
    }
    err = unzGetGlobalInfo64(unzip, &global_info64);
    if (err != UNZ_OK)
    {
        printf("Failed to get global info 64-bit (%" PRId32 ")\n", err);
        return err;
    }
    if (global_info.number_disk_with_CD != 0 || global_info64.number_disk_with_CD != 0)
    {
        printf("Invalid disk with cd (%" PRIu32 ")\n", global_info.number_disk_with_CD);
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
                fl.name = filename;
                fl.compressed_size = file_info64.compressed_size;
                fl.uncompressed_size = file_info64.uncompressed_size;
                fileInfoVec.push_back(fl);
            }
        }
    }

    for(const auto& i : fileInfoVec)
    {
        std::cout << i.name.toStdString() << " " << i.uncompressed_size << " " << i.compressed_size << std::endl;
    }

//    err = unzGoToFirstFile(unzip);
//    if (err == UNZ_OK)
//    {
//        filename[0] = 0;
//        err = unzGetCurrentFileInfo64(unzip, &file_info64, filename, sizeof(filename), nullptr, 0, nullptr, 0);
//        if (err != UNZ_OK)
//        {
//            printf("Failed to get current file info 64-bit (%" PRId32 ")\n", err);
//            return err;
//        }
//
//        err = unzOpenCurrentFile(unzip);
//        if (err != UNZ_OK)
//        {
//            printf("Failed to open current file (%" PRId32 ")\n", err);
//            return err;
//        }
//        bytes_read = unzReadCurrentFile(unzip, buffer, sizeof(buffer));
//        if (bytes_read != (int32_t)strlen(test_data))
//        {
//            printf("Failed to read zip entry data (%" PRId32 ")\n", err);
//            unzCloseCurrentFile(unzip);
//            return err;
//        }
//        if (unzEndOfFile(unzip) != 1)
//        {
//            printf("End of unzip not reported correctly\n");
//            return UNZ_INTERNALERROR;
//        }
//        err = unzCloseCurrentFile(unzip);
//        if (err != UNZ_OK)
//        {
//            printf("Failed to close current file (%" PRId32 ")\n", err);
//            return err;
//        }
//
//        if (unztell(unzip) != bytes_read)
//        {
//            printf("Unzip position not reported correctly\n");
//            return UNZ_INTERNALERROR;
//        }
//
//        err = unzGoToNextFile(unzip);
//        if (err != UNZ_OK)
//        {
//            printf("Failed to get next file info (%" PRId32 ")\n", err);
//            return err;
//        }
//
//        comment[0] = 0;
//        err = unzGetCurrentFileInfo(unzip, &file_info, filename, sizeof(filename), NULL, 0, comment, sizeof(comment));
//        if (err != UNZ_OK)
//        {
//            printf("Failed to get current file info (%" PRId32 ")\n", err);
//            return err;
//        }
//        if (strcmp(comment, "test local comment") != 0)
//        {
//            printf("Unexpected local comment value (%s)\n", comment);
//            return err;
//        }
//
//        err = unzGetFilePos(unzip, &file_pos);
//        if (err != UNZ_OK)
//        {
//            printf("Failed to get file position (%" PRId32 ")\n", err);
//            return err;
//        }
//        if (file_pos.num_of_file != 1)
//        {
//            printf("Unzip file position not reported correctly\n");
//            return UNZ_INTERNALERROR;
//        }
//
//        err = unzGetOffset(unzip);
//        if (err <= 0)
//        {
//            printf("Unzip invalid offset reported\n");
//            return UNZ_INTERNALERROR;
//        }
//
//        err = unzGoToNextFile(unzip);
//
//        if (err != UNZ_END_OF_LIST_OF_FILE)
//        {
//            printf("Failed to reach end of zip entries (%" PRId32 ")\n", err);
//            unzCloseCurrentFile(unzip);
//            return err;
//        }
//        err = unzSeek64(unzip, 0, SEEK_SET);
//    }
//
//    return UNZ_OK;
}