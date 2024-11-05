#include "Model.h"
#include "Delegate.h"
#include <QTableView>
#include <qdebug.h>

#include <QVBoxLayout>
#include <QTimer>
#include "DefaultTypes/DefaultTypes.h"
#include "TableView.h"

namespace VariantTable
{
    std::unordered_map<size_t, CellDataBase*> Model::s_cellFactories;
    bool Model::s_defaultFactoriesAdded = false;

    Model::Model(int rows, int cols, TableView* parent)
        : QAbstractTableModel(parent), m_data(rows, QVector<CellData>(cols))
		, m_delegate(new Delegate(this))
		, m_view(nullptr)
    {
        if (!s_defaultFactoriesAdded)
        {
			s_defaultFactoriesAdded = true;
			registerType<BoolCell>();
        }
        setTableView(parent);

		const CellDataBase* defaultFactory = getRegisteredType<BoolCell>();
        if (!defaultFactory)
        {
			qDebug() << "No default factory found";
        }
        for (size_t x = 0; x < m_data.size(); ++x)
        {
            for (size_t y = 0; y < m_data[x].size(); ++y)
            {
                m_data[x][y].data = defaultFactory->clone();
            }
        }
    }
    void Model::setTableView(TableView* view)
    {
		if (view)
		{
			m_view = view;
			view->setItemDelegate(m_delegate);
			view->setModel(this);
			view->setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::SelectedClicked);

            // Set the last column and row to stretch to fill available space
            view->horizontalHeader()->setStretchLastSection(true);

            // Optionally set the size policy for the table view
            view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		}
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

        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            return m_data[index.row()][index.column()].data->getData();
        }
        if (role == Qt::SizeHintRole)
        {
            QStyleOptionViewItem option;
			return m_data[index.row()][index.column()].data->getSizeHint(option);
        }

        return QVariant();
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
    void Model::setCellData(int row, int col, CellDataBase* data)
    {
        if (m_data.size() <= row)
            return;
        if (m_data[row].size() <= col)
            return;
		m_data[row][col].data = data;
    }

    Qt::ItemFlags Model::flags(const QModelIndex& index) const 
    {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    }

    const CellDataBase* Model::getRegisteredType(size_t typeId)
    {
		if (s_cellFactories.find(typeId) == s_cellFactories.end())
		{
			qDebug() << "Type with id " << typeId << " not found";
			return nullptr;
		}
		return s_cellFactories[typeId];
    }

    CellDataBase* Model::getCellData(int row, int col) const
    {
		if (m_data.size() <= row)
			return nullptr;
		if (m_data[row].size() <= col)
			return nullptr;
		return m_data[row][col].data;
    }
    bool Model::isIndexSelected(const QModelIndex& index) const
    {
        // Get the selection model from the view
        const QItemSelectionModel* selectionModel = m_view->selectionModel();
        // Check if the index is selected
        return selectionModel->isSelected(index);
    }
    void Model::onNewEditorCreated(QWidget* editor, const QModelIndex& index) const
    {
		m_view->onNewEditorCreated(editor, index);
    }
}