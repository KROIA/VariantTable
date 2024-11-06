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

        switch (role)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
        {
            return m_data[index.row()][index.column()].data->getData();
        }
        case Qt::SizeHintRole:
        {
            QStyleOptionViewItem option;
            return m_data[index.row()][index.column()].data->getSizeHint(option);
        }
        case Qt::ToolTipRole:
        {
			return m_data[index.row()][index.column()].data->getToolTip();
        }

        }

        

        return QVariant();
    }

    

    Qt::ItemFlags Model::flags(const QModelIndex& index) const 
    {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    }

   

    CellDataBasePtr Model::getCellData(int row, int col) const
    {
		if (m_data.size() <= row)
			return nullptr;
		if (m_data[row].size() <= col)
			return nullptr;
		return m_data[row][col].data;
    }
    void Model::iconThemeChanged()
    {
        for (Model* model : m_models)
        {
            model->iconThemeChanged_internal();
        }
    }
    void Model::iconThemeChanged_internal()
    {
		emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
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
        QVector<unsigned int> rows;
        const QItemSelectionModel* selectionModel = m_view->selectionModel();
        const QModelIndexList& selectedIndexes = selectionModel->selectedRows();
        for (const QModelIndex& index : selectedIndexes)
        {
            rows.append(index.row());
        }
        // shrink list and sort
		std::sort(rows.begin(), rows.end());
		rows.erase(std::unique(rows.begin(), rows.end()), rows.end());
        return rows;
    }
    QVector<unsigned int> Model::getSelectedColumns() const
    {
		QVector<unsigned int> columns;
		const QItemSelectionModel* selectionModel = m_view->selectionModel();
		const QModelIndexList& selectedIndexes = selectionModel->selectedColumns();
		for (const QModelIndex& index : selectedIndexes)
		{
			columns.append(index.column());
		}
		// shrink list and sort
		std::sort(columns.begin(), columns.end());
		columns.erase(std::unique(columns.begin(), columns.end()), columns.end());
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
		if (data.size() < columnCount())
		{
#ifdef _DEBUG
			qWarning() << "Data size does not match column count";
#endif
			return false;
		}
		beginInsertRows(QModelIndex(), row, row);
        for (int i = 0; i < columnCount(); ++i)
		{
			m_data[row].insert(i, CellData{ data[i] });
		}
		endInsertRows();
		return true;
	}
    bool Model::insertRow(int row, CellDataTypeID defaultType)
    {
        beginInsertRows(QModelIndex(), row, row);
        QVector<CellData> rowData;
        for (int i = 0; i < columnCount(); ++i)
        {
            rowData.append(CellData{ CellTypeRegistry::create(defaultType) });
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
            rowData.append(CellData{ typeTemplate->clone() });
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
                rowData.append(CellData{ CellTypeRegistry::createDefaultType() });
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
		return true;
    }
    bool Model::insertColumn(int col, CellDataBasePtr typeTemplate)
    {
        beginInsertColumns(QModelIndex(), col, col);
        for (int i = 0; i < rowCount(); ++i)
        {
            m_data[i].insert(col, CellData{ typeTemplate->clone()});
        }
        endInsertColumns();
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
    /*bool Model::moveRowsUp(QVector<unsigned int> rows, unsigned int amount)
    {
        if (rows.size() == 0)
            return false;
		QVector<QVector<CellData>> temp;

		beginRemoveRows(QModelIndex(), rows[0], rows[rows.size() - 1]);
        for (int i = 0; i < rows.size(); ++i)
        {
            temp.append(m_data[rows[i]]);
            m_data.removeAt(rows[i]);
        }
		endRemoveRows();

		beginInsertRows(QModelIndex(), rows[0] - amount, rows[0] - amount);
		for (int i = 0; i < rows.size(); ++i)
		{
			m_data.insert(rows[i] - amount, temp[i]);
		}
		endInsertRows();

		return true;
    }*/
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
    /*bool Model::moveRowsDown(QVector<unsigned int> rows, unsigned int amount)
    {
        if (rows.size() == 0)
            return false;
        // Sort rows
		std::sort(rows.begin(), rows.end());
		if (rows[rows.size() - 1] + amount >= m_data.size())
		{
            return false;
		}

        QVector<QVector<CellData>> temp = m_data;

        QVector<int> rowIndices;
		for (int i = 0; i < m_data.size(); ++i)
		{
			rowIndices.append(i);
		}

		// remove the row indices that are selected and reinsert them at the new position
		for (int i = 0; i < rows.size(); ++i)
		{
			rowIndices.removeOne(rows[i]);
		}
		// Check for out of bounds




		beginRemoveRows(QModelIndex(), rows[0], rows[rows.size() - 1]);
		m_data.clear();
		endRemoveRows();

		int insertIndex = rows[0] + amount;
		if (insertIndex > m_data.size())
			insertIndex = m_data.size();

		beginInsertRows(QModelIndex(), insertIndex, insertIndex);
        for (int i = 0; i < rows.size(); ++i)
        {
            m_data.insert(rows[i] + amount - rows.size(), temp[i]);
        }
		endInsertRows();
		return true;
    }*/

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
        m_data[row][col].data = data;
    }
}