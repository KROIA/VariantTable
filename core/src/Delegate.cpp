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
        // Track the active editor index
        QWidget* editor = nullptr;
        const Model* model = qobject_cast<const Model*>(index.model());
        if (!model && !editor)
        {
            editor = QStyledItemDelegate::createEditor(parent, option, index);
        }
        else 
        {
            CellDataBasePtr cellData = model->getCellData(index.row(), index.column());
            if (!cellData)
                editor = QStyledItemDelegate::createEditor(parent, option, index);
            else
            {
                editor = cellData->createEditorWidget(parent);
            }
        }
		//m_model->onNewEditorCreated(editor, index);
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

        /*
        // Check if this index is the active editor index
        if (m_model->isIndexSelected(index)) {
            // Skip drawing the cell's data if the editor is active
            return;
        }
        QRect rect = option.rect;
		QPoint TL = rect.topLeft();
        float xPos = rect.width() / 10;
        float height = rect.height();
        float size = 30;
        float yOffset = (height - size) / 2;
        // Call the base class implementation to draw the item normally
       // QStyledItemDelegate::paint(painter, option, index);
        // Example: Draw a checkbox indicator for the first column
        if (index.column() == 0) {
            //checkBoxRect.moveTop(checkBoxRect.top() + (checkBoxRect.height() - 16) / 2); // Center the checkbox vertically
            bool checked = index.data(Qt::EditRole).toBool();
            QStyleOptionButton button;
            button.rect = QRect(xPos+TL.x(), yOffset+ TL.y(), size, size);
            button.state = checked ? QStyle::State_On : QStyle::State_Off;
            button.state |= QStyle::State_Enabled;

            QApplication::style()->drawControl(QStyle::CE_CheckBox, &button, painter);
        }
        // Example: Draw a dropdown indicator for the second column
        else if (index.column() == 1) {
            
            //dropDownIndicatorRect.moveTop(dropDownIndicatorRect.top() + (dropDownIndicatorRect.height() - 16) / 2); // Center the icon
            
            
           
            //float width = dropDownIndicatorRect.width();
            // Draw a simple dropdown arrow (triangle)
            QPointF points[3] = {
                QPointF(xPos - size / 2,yOffset) + TL,
                QPointF(xPos + size / 2,yOffset) + TL,
                QPointF(xPos,size + yOffset) + TL,
            };

            // Create a linear gradient from gray (tip) to black (base)
            QLinearGradient gradient(points[2], points[0]); // Gradient from bottom right to top
            gradient.setColorAt(0, Qt::gray); // Tip color
            gradient.setColorAt(1, Qt::black); // Base color

            // Set the gradient as brush and draw the triangle
            painter->setBrush(gradient);
            painter->drawPolygon(points, 3); // Draw the triangle
            
        }
        */

    }
}