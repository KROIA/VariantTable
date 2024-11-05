#include "Delegate.h"
#include "Model.h"


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
        // Track the active editor index
        QWidget* editor = nullptr;
        const Model* model = qobject_cast<const Model*>(index.model());
        if (!model && !editor)
        {
            editor = QStyledItemDelegate::createEditor(parent, option, index);
        }
        else 
        {
            CellDataBase* cellData = model->getCellData(index.row(), index.column());
            if (!cellData)
                editor = QStyledItemDelegate::createEditor(parent, option, index);
            else
            {
                editor = cellData->createEditorWidget(parent);
            }
        }
		m_model->onNewEditorCreated(editor, index);
		return editor;
    }

    void Delegate::setEditorData(
        QWidget* editor, 
        const QModelIndex& index) const
    {
        const Model* customModel = qobject_cast<const Model*>(index.model());
        if (!customModel)
            return;

        CellDataBase* cellData = customModel->getCellData(index.row(), index.column());
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

        CellDataBase* cellData = customModel->getCellData(index.row(), index.column());
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

        CellDataBase* cellData = model->getCellData(index.row(), index.column());
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
        // Call the base class implementation to draw the item normally
        QStyledItemDelegate::paint(painter, option, index);
    }
}