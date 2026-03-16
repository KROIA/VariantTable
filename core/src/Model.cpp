#include "Model.h"
#include "Delegate.h"
#include <QTableView>
#include <qdebug.h>

#include <QVBoxLayout>
#include <QTimer>
#include "DefaultTypes/DefaultTypes.h"
#include "TableView.h"
#include "CellTypeRegistry.h"


namespace VariantTable
{
    QVector<Model*> Model::m_models;

    Model::Model(TableView* parent)
        : QAbstractTableModel(parent)
        , m_data(0, QVector<CellData>(0))
        , m_delegate(new Delegate(this))
        , m_view(nullptr)
    {
        setTableView(parent);
        m_models.push_back(this);
    }
    Model::~Model()
    {
        m_models.removeOne(this);
    }


    QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            return m_headers.value(section);
        }
        return QVariant();
    }

    bool Model::setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role)
    {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            m_headers[section] = value.toString();
            emit headerDataChanged(orientation, section, section);
            return true;
        }
        return false;
    }

    void Model::setTableView(TableView* view)
    {
        if (!view)
            return;

        m_view = view;
        m_view->setModel(this);
        m_view->setItemDelegate(m_delegate);
    }
    int Model::rowCount(const QModelIndex& parent) const
    {
        VT_UNUSED(parent);
        return m_data.size();
    }

    int Model::columnCount(const QModelIndex& parent) const
    {
        VT_UNUSED(parent);
        return m_data.isEmpty() ? 0 : m_data[0].size();
    }

    QVariant Model::data(const QModelIndex& index, int role) const
    {
        if (!index.isValid() || index.row() >= rowCount() || index.column() >= columnCount())
            return QVariant();

        CellDataBasePtr data = m_data[index.row()][index.column()].data;
        switch (role)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
        {
            if (data)
                return data->getData();
        }
        case Qt::SizeHintRole:
        {
            if (data)
            {
                QStyleOptionViewItem option;
                return data->getSizeHint(option);
            }
        }
        case Qt::ToolTipRole:
        {
            if (data)
                return data->getToolTip();
            return QVariant("No cell widget set at this position!");
        }
        }
        return QVariant();
    }



    Qt::ItemFlags Model::flags(const QModelIndex& index) const
    {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    }




    void Model::iconThemeChanged()
    {
        for (Model* model : m_models)
        {
            model->iconThemeChanged_internal();
        }
    }
    void Model::onDataChanged()
    {
        emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
    }
    void Model::createColumnHeaderIfNeeded()
    {
        int currentColumnCount = columnCount();
        for (int i = 0; i < currentColumnCount; ++i)
        {
            if (!m_headers.contains(i))
            {
                m_headers[i] = QString("%1").arg(i + 1);
            }
        }
    }
    void Model::onDataChanged(const CellDataBase* data)
    {
        for (int i = 0; i < rowCount(); ++i)
        {
            for (int j = 0; j < columnCount(); ++j)
            {
#ifdef VT_USE_SHARED_PTR
                if (m_data[i][j].data.get() == data)
#else
                if (m_data[i][j].data == data)
#endif
                {
                    emit dataChanged(index(i, j), index(i, j));
                    if (m_view)
                        m_view->update(index(i, j));
                }
            }
        }
    }
    void Model::iconThemeChanged_internal()
    {
        for (int i = 0; i < rowCount(); ++i)
        {
            for (int j = 0; j < columnCount(); ++j)
            {
                CellDataBasePtr data = m_data[i][j].data;
                if (data)
                    data->updateIcon();
            }
        }
        onDataChanged();
    }
    bool Model::isIndexSelected(const QModelIndex& index) const
    {
        // Get the selection model from the view
        const QItemSelectionModel* selectionModel = m_view->selectionModel();
        // Check if the index is selected
        return selectionModel->isSelected(index);
    }
    QVector<unsigned int> Model::getSelectedRows() const
    {
        QSet<unsigned int> rowSet;
        const QItemSelectionModel* selectionModel = m_view->selectionModel();
        const QModelIndexList& selectedIndexes = selectionModel->selectedRows();
        for (const QModelIndex& index : selectedIndexes)
        {
            rowSet.insert(index.row());
        }
        // shrink list and sort
		QVector<unsigned int> rows = rowSet.values().toVector();
        std::sort(rows.begin(), rows.end());
        return rows;
    }
    QVector<unsigned int> Model::getSelectedColumns() const
    {
        QSet<unsigned int> columnSet;
        const QItemSelectionModel* selectionModel = m_view->selectionModel();
        const QModelIndexList& selectedIndexes = selectionModel->selectedColumns();
        for (const QModelIndex& index : selectedIndexes)
        {
            columnSet.insert(index.column());
        }
        // shrink list and sort
		QVector<unsigned int> columns = columnSet.values().toVector();
        std::sort(columns.begin(), columns.end());
        return columns;
    }



    /*bool Model::insertRow(int row, const QModelIndex& parent)
    {
        beginInsertRows(parent, row, row);
        QVector<CellData> rowData;
        for (int i = 0; i < columnCount(); ++i)
        {
            rowData.append(CellData{ CellTypeRegistry::createDefaultType() });
        }
        m_data.insert(row, rowData);
        endInsertRows();
        return true;
    }*/
    bool Model::insertRow(int row, const QVector<CellDataBasePtr>& data)
    {
        /*if (data.size() < columnCount())
        {
#ifdef _DEBUG
            qWarning() << "Data size does not match column count";
#endif
            return false;
        }*/
        if (m_data.size() <= row)
        {
            insertRows(m_data.size(), row - m_data.size() + 1);
        }
        if (m_data[row].size() <= data.size() - 1)
        {
            insertColumns(m_data[row].size(), data.size() - m_data[row].size());
        }
        //beginInsertRows(QModelIndex(), row, row);
        for (int i = 0; i < data.size(); ++i)
        {
            CellDataBasePtr obj = data[i];
            obj->setTableData(this);

            if (obj)
            {
                Model* model = obj->getModel();
                if (model)
                {
                    model->remove(obj);
                }
                obj->setTableData(this);
            }
            m_data[row][i] = CellData{ obj };
        }
        //endInsertRows();
        return true;
    }
    bool Model::insertRow(int row, CellDataTypeID defaultType)
    {
        beginInsertRows(QModelIndex(), row, row);
        QVector<CellData> rowData;
        for (int i = 0; i < columnCount(); ++i)
        {
            CellDataBasePtr obj = CellTypeRegistry::create(defaultType);
            obj->setTableData(this);
            rowData.append(CellData{ obj });
        }
        m_data.insert(row, rowData);
        endInsertRows();
        return true;
    }
    bool Model::insertRow(int row, CellDataBasePtr typeTemplate)
    {
        beginInsertRows(QModelIndex(), row, row);
        QVector<CellData> rowData;
        for (int i = 0; i < columnCount(); ++i)
        {
            CellDataBasePtr obj = typeTemplate->clone();
            obj->setTableData(this);
            rowData.append(CellData{ obj });
        }
        m_data.insert(row, rowData);
        endInsertRows();
        return true;
    }
    bool Model::insertRows(int row, int count, const QModelIndex& parent)
    {
        beginInsertRows(parent, row, row + count - 1);
        for (int i = 0; i < count; ++i)
        {
            QVector<CellData> rowData;
            for (int j = 0; j < columnCount(); ++j)
            {
                CellDataBasePtr obj = CellTypeRegistry::createDefaultType();
                obj->setTableData(this);
                rowData.append(CellData{ obj });
            }
            m_data.insert(row + i, rowData);
        }
        endInsertRows();
        return true;
    }




    /* bool Model::insertColumn(int col, const QModelIndex& parent)
     {
         beginInsertColumns(parent, col, col);
         for (int i = 0; i < rowCount(); ++i)
         {
             m_data[i].insert(col, CellData{ CellTypeRegistry::createDefaultType() });
         }
         endInsertColumns();
         return true;
     }*/
    bool Model::insertColumn(int col, const QVector<CellDataBasePtr>& data)
    {
        if (data.size() < rowCount())
        {
#ifdef _DEBUG
            qWarning() << "Data size does not match row count";
#endif
            return false;
        }
        beginInsertColumns(QModelIndex(), col, col + data.size() - 1);
        for (int i = 0; i < rowCount(); ++i)
        {
            m_data[i].insert(col, CellData{ data[i] });
        }
        endInsertColumns();

        createColumnHeaderIfNeeded();
        return true;
    }
    bool Model::insertColumn(int col, CellDataTypeID defaultType)
    {
        beginInsertColumns(QModelIndex(), col, col);
        for (int i = 0; i < rowCount(); ++i)
        {
            m_data[i].insert(col, CellData{ CellTypeRegistry::create(defaultType) });
        }
        endInsertColumns();

        createColumnHeaderIfNeeded();
        return true;
    }
    bool Model::insertColumn(int col, CellDataBasePtr typeTemplate)
    {
        beginInsertColumns(QModelIndex(), col, col);
        for (int i = 0; i < rowCount(); ++i)
        {
            m_data[i].insert(col, CellData{ typeTemplate->clone() });
        }
        endInsertColumns();

        createColumnHeaderIfNeeded();
        return true;
    }
    bool Model::insertColumns(int col, int count, const QModelIndex& parent)
    {
        beginInsertColumns(parent, col, col + count - 1);
        for (int i = 0; i < count; ++i)
        {
            for (int j = 0; j < rowCount(); ++j)
            {
                m_data[j].insert(col + i, CellData{ CellTypeRegistry::createDefaultType() });
            }
        }
        endInsertColumns();

        createColumnHeaderIfNeeded();
        return true;
    }


    bool Model::removeRows(int row, int count, const QModelIndex& parent)
    {
        beginRemoveRows(parent, row, row + count - 1);
        for (int i = 0; i < count; ++i)
        {
            m_data.removeAt(row);
        }
        endRemoveRows();
        return true;
    }
    bool Model::removeRows(const QVector<unsigned int>& rows, const QModelIndex& parent)
    {
		// Sort rows in descending order to avoid index shifting issues
        QVector<unsigned int> sortedRows = rows;
        std::sort(sortedRows.begin(), sortedRows.end(), std::greater<unsigned int>());
        for (unsigned int row : sortedRows)
        {
            if (row < (unsigned)m_data.size())
            {
                beginRemoveRows(parent, row, row);
                m_data.removeAt(row);
                endRemoveRows();
            }
        }
		return true;
    }

    bool Model::removeColumns(int column, int count, const QModelIndex& parent)
    {
        beginRemoveColumns(parent, column, column + count - 1);
        for (int i = 0; i < count; ++i)
        {
            for (int j = 0; j < rowCount(); ++j)
            {
                m_data[j].removeAt(column);
            }
        }
        endRemoveColumns();
        return true;
    }

    bool Model::removeColumns(const QVector<unsigned int>& columns, const QModelIndex& parent)
    {
		// Sort columns in descending order to avoid index shifting issues
        QVector<unsigned int> sortedColumns = columns;
        std::sort(sortedColumns.begin(), sortedColumns.end(), std::greater<unsigned int>());
        for (unsigned int column : sortedColumns)
        {
            if (column < (unsigned)columnCount())
            {
                beginRemoveColumns(parent, column, column);
                for (int j = 0; j < rowCount(); ++j)
                {
                    m_data[j].removeAt(column);
                }
                endRemoveColumns();
            }
		}
		return true;
    }


    bool Model::moveRowUp(unsigned int row, unsigned int amount)
    {
        if (row < amount)
        {
            return false;
        }
        QVector<CellData> temp = m_data[row];

        beginRemoveRows(QModelIndex(), row, row);
        m_data.removeAt(row);
        endRemoveRows();

        beginInsertRows(QModelIndex(), row - amount, row - amount);
        m_data.insert(row - amount, temp);
        endInsertRows();

        return true;
    }
    bool Model::moveRowsUp(QVector<unsigned int> rows, unsigned int amount)
    {
        if (rows.size() == 0)
            return false;
        
		// Sort rows
		std::sort(rows.begin(), rows.end());

		// Find groups that can be moved together
		QVector<std::pair<unsigned int, unsigned int>> groups;
		QVector<unsigned int> currentGroup;
		unsigned int lastRow = rows[0];
		unsigned int groupStart = lastRow;
        for (int i = 1; i < rows.size(); ++i)
        {
			unsigned int row = rows[i];
            if (row == lastRow + 1)
            {
				lastRow = row;
            }
            else
            {
				groups.append({ groupStart, lastRow-groupStart+1 });
				groupStart = row;
            }
        }
        if (groups.empty())
        {
            if (rows.size() == 1)
            {
				groups.append({ rows[0], 1 });
            }
            else
            {
                groups.append({ groupStart, lastRow - groupStart + 1 });
            }
        }

		// Move groups
		for (int i = 0; i < groups.size(); ++i)
		{
			unsigned int groupRow = groups[i].first;
			unsigned int groupCount = groups[i].second;
			if (!moveRowsUp(groupRow, groupCount, amount))
			{
				return false;
			}
		}


        return true;
    }
    bool Model::moveRowsUp(unsigned int row, unsigned int rowCount, unsigned int amount)
    {
        if (row < amount)
        {
			// clip amount to the maximum possible value
			amount = row;
        }
        if(row + rowCount > (unsigned)m_data.size())
        {
			rowCount = m_data.size() - row;
		}

        if(amount == 0 || rowCount == 0)
			return false; // Nothing to move


        QVector<QVector<CellData>> temp;
		temp.reserve(rowCount);
        beginRemoveRows(QModelIndex(), row, row + rowCount - 1);
        for (unsigned int i = 0; i < rowCount; ++i)
        {
            temp.append(m_data[row + i]);
            m_data.removeAt(row + i);
        }
        endRemoveRows();
        beginInsertRows(QModelIndex(), row - amount, row - amount);
        for (unsigned int i = 0; i < rowCount; ++i)
        {
            m_data.insert(row - amount + i, temp[i]);
        }
		endInsertRows();

		return true;
    }
    bool Model::moveRowDown(unsigned int row, unsigned int amount)
    {
        if (row + amount >= (unsigned)m_data.size())
        {
            return false;
        }

        QVector<CellData> temp = m_data[row];

        beginRemoveRows(QModelIndex(), row, row);
        m_data.removeAt(row);
        endRemoveRows();

        beginInsertRows(QModelIndex(), row + amount, row + amount);
        m_data.insert(row + amount, temp);
        endInsertRows();

        return true;
    }
    bool Model::moveRowsDown(QVector<unsigned int> rows, unsigned int amount)
    {
        if (rows.size() == 0)
			return false;

		// Sort rows
		std::sort(rows.begin(), rows.end());

		// Find groups that can be moved together
		QVector<std::pair<unsigned int, unsigned int>> groups;
		QVector<unsigned int> currentGroup;
		unsigned int lastRow = rows[0];
        unsigned int groupStart = lastRow;
        for (int i = 1; i < rows.size(); ++i)
		{
            unsigned int row = rows[i];
            if (row == lastRow + 1)
			{
                lastRow = row;
            }
			else
                {
                groups.append({ groupStart, lastRow - groupStart + 1 });
                groupStart = row;
            }
        }
        if (groups.empty())
        {
            if (rows.size() == 1)
            {
                groups.append({ rows[0], 1 });
            }
            else
            {
                groups.append({ groupStart, lastRow - groupStart + 1 });
            }
        }

		// Move groups
        for (int i = groups.size() - 1; i >= 0; --i)
        {
            unsigned int groupRow = groups[i].first;
            unsigned int groupCount = groups[i].second;
            if (!moveRowsDown(groupRow, groupCount, amount))
            {
                return false;
            }
        }
		return true;
    }

    bool Model::moveRowsDown(unsigned int row, unsigned int rowCount, unsigned int amount)
    {
        if (row + rowCount + amount > (unsigned)m_data.size())
        {
            // clip amount to the maximum possible value
            if (row + rowCount >= (unsigned)m_data.size())
            {
                return false; // Nothing to move
            }
            amount = m_data.size() - (row + rowCount);
        }
		QVector<QVector<CellData>> temp;

        beginRemoveRows(QModelIndex(), row, row + rowCount - 1);
        for (unsigned int i = 0; i < rowCount; ++i)
        {
            temp.append(m_data[row + i]);
            m_data.removeAt(row + i);
        }
        endRemoveRows();
        beginInsertRows(QModelIndex(), row + amount, row + amount);
        for (unsigned int i = 0; i < rowCount; ++i)
        {
            m_data.insert(row + amount + i, temp[i]);
        }
        endInsertRows();
		return true;

    }

    bool Model::swapRows(unsigned int row1, unsigned int row2)
    {
        if (row1 >= (unsigned)m_data.size() || row2 >= (unsigned)m_data.size())
            return false;
        QVector<CellData> temp = m_data[row1];
        beginMoveRows(QModelIndex(), row1, row1, QModelIndex(), row2);
        m_data[row1] = m_data[row2];
        m_data[row2] = temp;
        endMoveRows();
        return true;
    }
    bool Model::moveRow(unsigned int sourceRow, unsigned int destinationRow)
    {
        if (sourceRow >= (unsigned)m_data.size() || destinationRow >= (unsigned)m_data.size())
            return false;
        QVector<CellData> temp = m_data[sourceRow];
        if (sourceRow < destinationRow)
            destinationRow--;
        beginMoveRows(QModelIndex(), sourceRow, sourceRow, QModelIndex(), destinationRow);
        m_data.removeAt(sourceRow);

        m_data.insert(destinationRow, temp);
        endMoveRows();
        return true;
    }

    void Model::clear()
    {
        beginResetModel();
        m_data.clear();
        endResetModel();
    }

    bool Model::setData(const QModelIndex& index, const QVariant& value, int role)
    {
        if (index.isValid() && role == Qt::EditRole) {
            // Update the 2D array with the new value
            m_data[index.row()][index.column()].data->setData(value);

            // Emit dataChanged to update the view
            emit dataChanged(index, index, { role });
            return true;
        }
        return false;
    }
    void Model::setCellData(int row, int col, CellDataBasePtr data)
    {
        if (m_data.size() <= row)
        {
            insertRows(m_data.size(), row - m_data.size() + 1);
        }
        if (m_data[row].size() <= col)
        {
            insertColumns(m_data[row].size(), col - m_data[row].size() + 1);
        }

        if (m_data[row].size() <= col)
            return;
        CellDataBasePtr old = m_data[row][col].data;
        if (old)
            old->setTableData(nullptr);

        if (data)
        {
            Model* model = data->getModel();
            if (model)
            {
                model->remove(data);
            }
            data->setTableData(this);
        }
        m_data[row][col].data = data;
    }
    CellDataBasePtr Model::getCellData(int row, int col) const
    {
        if (m_data.size() <= row)
            return nullptr;
        if (m_data[row].size() <= col)
            return nullptr;
        return m_data[row][col].data;
    }

    void Model::remove(CellDataBasePtr data)
    {
        for (int row = 0; row < rowCount(); ++row)
        {
            for (int col = 0; col < columnCount(); ++col)
            {
                if (m_data[row][col].data == data)
                {
                    setCellData(row, col, nullptr);
                }
            }
        }
    }
}