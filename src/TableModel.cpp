//
// Created by julian on 5/17/21.
//

#include "inc/TableModel.h"

TableModel::TableModel(QObject *parent): QAbstractTableModel(parent) {

}

int TableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return listOfPairs.size();
}

int TableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 2;
}

QVariant TableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() >= listOfPairs.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        FileInfo pair = listOfPairs.at(index.row());

        if (index.column() == 0)
            return pair.name;
        else if (index.column() == 1)
            return pair.uncompressed_size;
    }
    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return tr("Name");

            case 1:
                return tr("Size");

            default:
                return QVariant();
        }
    }
    return QVariant();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();

        FileInfo p = listOfPairs.value(row);

        if (index.column() == 0)
            p.name = value.toString();
        else if (index.column() == 1)
            p.uncompressed_size = value.toLongLong();
        else
            return false;

        listOfPairs.replace(row, p);
        emit(dataChanged(index, index));

        return true;
    }

    return false;
}

bool TableModel::insertRows(int position, int rows, const QModelIndex &index) {
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; row++) {
        FileInfo pair(" ", 0);
        listOfPairs.insert(position, pair);
    }

    endInsertRows();
    return true;
}

bool TableModel::removeRows(int position, int rows, const QModelIndex &index) {
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; ++row) {
        listOfPairs.removeAt(position);
    }

    endRemoveRows();
    return true;
}

//QModelIndex TableModel::index( int row, int column, const QModelIndex &parent ) const
//{
//
////    if ( !parent.isValid() )
////        parentItem = rootItem;
////    else
////        parentItem = static_cast<ModelItem*>( parent.internalPointer() );
//
//    if(listOfPairs.size() == 0)
//    {
//
//    }
//    FileInfo childItem = listOfPairs.value(row);
//    return createIndex( row, column, &childItem );
//}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index);
}

QList<FileInfo> TableModel::getList() {
    return listOfPairs;
}


//QModelIndex TableModel::parent( const QModelIndex &index ) const
//{
//    if ( !index.isValid() )
//        return QModelIndex();
//
//    auto *childItem = static_cast<FileInfo*>( index.internalPointer() );
//    FileInfo *parentItem = childItem->parent();
//
//    if ( parentItem == rootItem )
//        return QModelIndex();
//
//    return createIndex( parentItem->row(), 0, parentItem );
//}
//
//QList<FileInfo> TableModel::getList() {
//    return listOfPairs;
//}

