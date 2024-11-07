#include "Delegate.h"
#include "Model.h"
#include <QApplication>
#include <QPainter>


namespace VariantTable
{
    Delegate::Delegate(Model *parent)
        : QStyledItemDelegate(parent)
		, m_model(parent)
    {
        
    }

    QWidget* Delegate::createEditor(
        QWidget* parent, 
        const QStyleOptionViewItem& option, 
        const QModelIndex& index) const
    {
		VT_UNUSED(option);
        // Track the active editor index
        QWidget* editor = nullptr;
        const Model* model = qobject_cast<const Model*>(index.model());
        if (model)
        {
            CellDataBasePtr cellData = model->getCellData(index.row(), index.column());
            if (cellData)
            {
                editor = cellData->createEditorWidget_internal(parent);
            }
        }
		return editor;
    }

    void Delegate::setEditorData(
        QWidget* editor, 
        const QModelIndex& index) const
    {
        const Model* customModel = qobject_cast<const Model*>(index.model());
        if (!customModel)
            return;

        CellDataBasePtr cellData = customModel->getCellData(index.row(), index.column());
        if (!cellData)
            return;
        cellData->getData(editor);
    }

    void Delegate::setModelData(
        QWidget* editor, 
        QAbstractItemModel* model, 
        const QModelIndex& index) const
    {
        const Model* customModel = qobject_cast<const Model*>(model);
        if (!customModel)
            return;

        CellDataBasePtr cellData = customModel->getCellData(index.row(), index.column());
        if (!cellData)
			return;
		cellData->setData(editor);
    }

    QSize Delegate::sizeHint(
        const QStyleOptionViewItem& option, 
        const QModelIndex& index) const 
    {
        const Model* model = qobject_cast<const Model*>(index.model());
        if (!model)
            return QStyledItemDelegate::sizeHint(option, index);

        CellDataBasePtr cellData = model->getCellData(index.row(), index.column());
        if (!cellData)
            return QStyledItemDelegate::sizeHint(option, index);

		return cellData->getSizeHint(option); 
    }


    void Delegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        // Check if this index is the active editor index
        if (m_model->isIndexSelected(index)) {
            // Skip drawing the cell's data if the editor is active
            return;
        }

        const Model* model = qobject_cast<const Model*>(index.model());
        if (!model)
            return;

        CellDataBasePtr cellData = model->getCellData(index.row(), index.column());
        if (!cellData)
            return;

        return cellData->drawEditorPlaceholder(painter, option);
    }
}