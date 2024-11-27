#include "TableView.h"
#include <QScrollBar.h>
#include "OverlayRect.h"

namespace VariantTable
{
    




    TableView::TableView(QWidget* parent)
        : QTableView(parent)
        , m_model(nullptr)
        , m_firstFocus(true)
    {
        m_model = new Model(this);
        // Connect selection changes to slot
        connect(selectionModel(), &QItemSelectionModel::selectionChanged, this, &TableView::onSelectionChanged);

        // set all columns to equal width
        horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


        
        
        setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::SelectedClicked);

        // Set the last column and row to stretch to fill available space
        horizontalHeader()->setStretchLastSection(true);

        // Optionally set the size policy for the table view
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    
        m_overlay = new Internal::OverlayRect(this);  // Assuming 'this' is the parent QWidget
        m_overlay->setGeometry(0, 0, 1000, 1000);
        m_overlay->raise(); // Ensure it is above the QTableView
        m_overlay->show();

        setStyleSheet("QTableView::item { padding: "+QString::number(CELL_PADDING)+"px; }");

    }


    void TableView::scrollToRow(unsigned int row)
	{
		QModelIndex firstIndexInRow = model()->index(row, 0); // First cell in the row
		scrollTo(firstIndexInRow, QAbstractItemView::PositionAtTop);
    }
    void TableView::scrollToColumn(unsigned int column)
    {
        QModelIndex firstIndexInColumn = model()->index(0, column); // First cell in the column
        scrollTo(firstIndexInColumn, QAbstractItemView::PositionAtTop);
    }

    void TableView::selectRow(unsigned int row)
    {
        QModelIndex topLeft = m_model->index(row, 0); // First column in the row
        QModelIndex bottomRight = m_model->index(row, m_model->columnCount() - 1); // Last column in the row

		scrollToRow(row);

        // Current selection
		QItemSelection currentSelection = selectionModel()->selection();

		// Add the row cells to the selection
		QItemSelection selection(topLeft, bottomRight);
		currentSelection.merge(selection, QItemSelectionModel::Select);

		// Set the new selection
		selectionModel()->select(currentSelection, QItemSelectionModel::Select);
    }
    void TableView::selectColumn(unsigned int column)
    {
        QModelIndex topLeft = m_model->index(0, column); // First row in the column
        QModelIndex bottomRight = m_model->index(m_model->rowCount() - 1, column); // Last row in the column

		scrollToColumn(column);

		// Current selection
		QItemSelection currentSelection = selectionModel()->selection();

		// Add the column cells to the selection
		QItemSelection selection(topLeft, bottomRight);
		currentSelection.merge(selection, QItemSelectionModel::Select);

		// Set the new selection
		selectionModel()->select(currentSelection, QItemSelectionModel::Select);
    }
    void TableView::selectCell(unsigned int row, unsigned int column)
    {
		QModelIndex index = m_model->index(row, column);
		
		// Current selection
		QItemSelection currentSelection = selectionModel()->selection();

		// Add the cell to the selection
		QItemSelection selection(index, index);

		currentSelection.merge(selection, QItemSelectionModel::Select);

		// Set the new selection
		selectionModel()->select(currentSelection, QItemSelectionModel::Select);
    }

    void TableView::deselectRow(unsigned int row)
    {
        QModelIndex topLeft = m_model->index(row, 0); // First column in the row
        QModelIndex bottomRight = m_model->index(row, m_model->columnCount() - 1); // Last column in the row

		// Remove the row cells from the selection
		QItemSelection selection(topLeft, bottomRight);

		// Set the new selection
		selectionModel()->select(selection, QItemSelectionModel::Deselect);
    }
    void TableView::deselectColumn(unsigned int column)
    {
        QModelIndex topLeft = m_model->index(0, column); // First row in the column
        QModelIndex bottomRight = m_model->index(m_model->rowCount() - 1, column); // Last row in the column

		// Remove the column cells from the selection
		QItemSelection selection(topLeft, bottomRight);

		// Set the new selection
		selectionModel()->select(selection, QItemSelectionModel::Deselect);

        // Recalculate the size of rows and columns
       // resizeRowsToContents();
        //resizeColumnsToContents();

        // Update the geometry to reflect the new layout
        //updateGeometry();
        //m_model->onDataChanged();
    }
    void TableView::deselectCell(unsigned int row, unsigned int column)
    {
		QModelIndex index = m_model->index(row, column);

		// Remove the cell from the selection
		QItemSelection selection(index, index);

		// Set the new selection
		selectionModel()->select(selection, QItemSelectionModel::Deselect);
    }


    void TableView::highlightRow(unsigned int row)
    {
		highlightRow(row, 4, QColor(255, 0, 0, 255), 10, Internal::OverlayRect::Mode::fadeOut);
    }
    void TableView::highlightRow(unsigned int row, unsigned int lineWidth,
                                 const QColor& lineColor, float speed, Internal::OverlayRect::Mode mode)
    {
        auto rect = std::make_shared<Internal::OverlayRect::RowRect>(this, row);
        rect->speed = speed;
        rect->color = lineColor;
        rect->mode = mode;
        rect->width = lineWidth;
        m_overlay->add(rect);
    }
    void TableView::highlightColumn(unsigned int column)
    {
		highlightColumn(column, 4, QColor(255, 0, 0, 255), 10, Internal::OverlayRect::Mode::fadeOut);
    }
    void TableView::highlightColumn(unsigned int column, unsigned int lineWidth,
                                    const QColor& lineColor, float speed, Internal::OverlayRect::Mode mode)
    {
        auto rect = std::make_shared<Internal::OverlayRect::ColumnRect>(this, column);
        rect->speed = speed;
        rect->color = lineColor;
        rect->mode = mode;
        rect->width = lineWidth;
        m_overlay->add(rect);
    }
    void TableView::highlightCell(unsigned int row, unsigned int column)
    {
		highlightCell(row, column, 4, QColor(255, 0, 0, 255), 10, Internal::OverlayRect::Mode::fadeOut);
    }
    void TableView::highlightCell(unsigned int row, unsigned int column, unsigned int lineWidth,
                                  const QColor& lineColor, float speed, Internal::OverlayRect::Mode mode)
    {
        auto rect = std::make_shared<Internal::OverlayRect::CellRect>(this, row, column);
        rect->speed = speed;
        rect->color = lineColor;
		rect->mode = mode;
		rect->width = lineWidth;
        m_overlay->add(rect);
    }


    void TableView::resizeEvent(QResizeEvent* event) 
    {
        QTableView::resizeEvent(event);
        // Adjust the overlay position and size to match the table viewport
        QRect viewportRect = geometry(); // Get the viewport's geometry
        viewportRect.setX(0); // Set the X position to 0
        viewportRect.setY(0); // Set the Y position to 0
        m_overlay->setGeometry(viewportRect); // Position the overlay over the viewport
       // qDebug() << "Resizing overlay to" << viewportRect;

    }
    
    void TableView::onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected) 
    {
        // Close editors for all deselected cells
        for (const QModelIndex& index : deselected.indexes())
        {
            QWidget* editor = indexWidget(index);  // Get the editor widget for this index
            if (editor) {
                emit commitData(editor);  // Ensure setModelData is called
            }
            closePersistentEditor(index);
            setRowHeight(index.row(), verticalHeader()->defaultSectionSize());
            setColumnWidth(index.column(), horizontalHeader()->defaultSectionSize());
        }

        // Open editors for all newly selected cells
        for (const QModelIndex& index : selected.indexes()) 
        {
            openPersistentEditor(index);
            resizeRowToContents(index.row());
            resizeColumnToContents(index.column());
        }
        
    }

   //void TableView::onNewEditorCreated(QWidget* editor, const QModelIndex& index)
   //{
   //    VT_UNUSED(editor);
   //    if (index.isValid()) 
   //    {
   //        // Resize the row and column of the selected cell to fit its contents
   //        resizeRowToContents(index.row());
   //        resizeColumnToContents(index.column());
   //    }
   //}
    void TableView::focusInEvent(QFocusEvent* event) 
    {
        if (m_firstFocus) 
        {
            m_firstFocus = false; // Allow focus on subsequent events
            // Skip calling the base class to prevent auto selection
            return;
        }
        // Call the base class to allow default behavior
        QTableView::focusInEvent(event);
    }
}