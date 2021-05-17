//
// Created by julian on 5/13/21.
//

#ifndef ZIPVIEWER_MAINWINDOW_H
#define ZIPVIEWER_MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QtWidgets/QFileDialog>
#include <zip.h>
#include <QtWidgets/QTableView>
#include "inc/TableModel.h"
#include "inc/FileInfo.h"

namespace Ui {
    class MainWindow;
}

class ZipViewerWin : public QMainWindow
{
    //Q_INIT_RESOURCE(resources)

Q_OBJECT

public:
    explicit ZipViewerWin(QWidget *parent = Q_NULLPTR);
    QWidget *ZipViewerWid = new QWidget;
    QPushButton *chsFile = new QPushButton(QString("Choose zip"));
    QLineEdit *nmFile = new QLineEdit;
    QVBoxLayout *ZipLayoutV = new QVBoxLayout;
    QHBoxLayout *nmNchsLayoutH = new QHBoxLayout;
    //static EventShowClass *ev;

private:

    QTableView *ZipTable = new QTableView;
    //QAbstractItemModel *model = new StringListModel(numbers);
    std::vector<FileInfo> fileInfoVec;

    int32_t unzip_compat(const QString& fileName);
    int32_t test_unzip_compat_int(unzFile unzip);

    TableModel *table ;
    void on_actionExit_triggered();

private slots:
    void chooseFl();

};

#endif //ZIPVIEWER_MAINWINDOW_H
