//
// Created by julian on 5/17/21.
//

#ifndef ZIPVIEWER_TABLEMODEL_H
#define ZIPVIEWER_TABLEMODEL_H


#include <QAbstractItemModel>
#include "inc/FileInfo.h"

class TableModel: public QAbstractTableModel {

    Q_OBJECT

public:
    explicit TableModel(QObject *parent=nullptr);
    //TableModel(QList< QPair<QString, QString> > listofPairs, QObject *parent=0);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

//    QModelIndex index( int row, int column, const QModelIndex &parent ) const override;
//    QModelIndex parent( const QModelIndex &index ) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole) override;
    bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index=QModelIndex()) override;
    QList<FileInfo> getList();

private:
    QList<FileInfo> listOfPairs;
};


#endif //ZIPVIEWER_TABLEMODEL_H
