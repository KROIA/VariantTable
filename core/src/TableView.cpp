#include "TableView.h"
#include "Model.h"
#include <QHeaderView>
#include <QKeyEvent>

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
        horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

		//setFocusPolicy(Qt::StrongFocus);

        
        
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


        setCopyPasteDelegate<DefaultCopyPasteDelegate>();
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

    bool TableView::isIndexSelected(const QModelIndex& index) const
    {
        // Check if the index is selected
        return selectionModel()->isSelected(index);
    }
    QVector<unsigned int> TableView::getSelectedRows() const
    {
        QSet<unsigned int> rowSet;
        const QModelIndexList& selectedIndexes = selectionModel()->selectedRows();
        for (const QModelIndex& index : selectedIndexes)
        {
            rowSet.insert(index.row());
        }
        // shrink list and sort
        QVector<unsigned int> rows = rowSet.values().toVector();
        std::sort(rows.begin(), rows.end());
        return rows;
    }
    QVector<unsigned int> TableView::getSelectedColumns() const
    {
        QSet<unsigned int> columnSet;
        const QModelIndexList& selectedIndexes = selectionModel()->selectedColumns();
        for (const QModelIndex& index : selectedIndexes)
        {
            columnSet.insert(index.column());
        }
        // shrink list and sort
        QVector<unsigned int> columns = columnSet.values().toVector();
        std::sort(columns.begin(), columns.end());
        return columns;
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
    void TableView::selectRows(const QVector<unsigned int>& rows)
    {
        QItemSelection currentSelection = selectionModel()->selection();

        for (unsigned int row : rows)
        {
            QModelIndex topLeft = m_model->index(row, 0); // First column in the row
            QModelIndex bottomRight = m_model->index(row, m_model->columnCount() - 1); // Last column in the row
            QItemSelection selection(topLeft, bottomRight);
            currentSelection.merge(selection, QItemSelectionModel::Select);
        }
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
    void TableView::selectColumns(const QVector<unsigned int>& columns)
    {
        QItemSelection currentSelection = selectionModel()->selection();
        for (unsigned int column : columns)
        {
            QModelIndex topLeft = m_model->index(0, column); // First row in the column
            QModelIndex bottomRight = m_model->index(m_model->rowCount() - 1, column); // Last row in the column
            QItemSelection selection(topLeft, bottomRight);
            currentSelection.merge(selection, QItemSelectionModel::Select);
        }
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
    void TableView::deselectRows(const QVector<unsigned int>& rows)
    {
        for (unsigned int row : rows)
        {
            QModelIndex topLeft = m_model->index(row, 0); // First column in the row
            QModelIndex bottomRight = m_model->index(row, m_model->columnCount() - 1); // Last column in the row
            QItemSelection selection(topLeft, bottomRight);
            selectionModel()->select(selection, QItemSelectionModel::Deselect);
        }
    }
    void TableView::deselectColumn(unsigned int column)
    {
        QModelIndex topLeft = m_model->index(0, column); // First row in the column
        QModelIndex bottomRight = m_model->index(m_model->rowCount() - 1, column); // Last row in the column

		// Remove the column cells from the selection
		QItemSelection selection(topLeft, bottomRight);

		// Set the new selection
		selectionModel()->select(selection, QItemSelectionModel::Deselect);
    }
    void TableView::deselectColumns(const QVector<unsigned int>& columns)
    {
        for (unsigned int column : columns)
        {
            QModelIndex topLeft = m_model->index(0, column); // First row in the column
            QModelIndex bottomRight = m_model->index(m_model->rowCount() - 1, column); // Last row in the column
            QItemSelection selection(topLeft, bottomRight);
            selectionModel()->select(selection, QItemSelectionModel::Deselect);
        }
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

    void TableView::keyPressEvent(QKeyEvent* event)
    {
		bool modifierControl = event->modifiers() == Qt::ControlModifier;

        if (modifierControl)
        {
            // Get selected indexes
            QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();

            switch (event->key())
            {
                case  Qt::Key_C:
                {
                    // Get first index
                    if (selectedIndexes.isEmpty() || !m_enableCellCopy)
                        break;
                    QModelIndex firstIndex = selectedIndexes.first();

                    // Get cell data of the index
                    CellDataBasePtr cellData = m_model->getCellData(firstIndex.row(), firstIndex.column());
                    if (cellData)
                    {
                        if (m_copyPasteDelegate)
                        {
                            if(m_copyPasteDelegate->copyCell(cellData, firstIndex))
                            {
                                highlightCell(firstIndex.row(), firstIndex.column(), 2, m_copyCellIndicatorColor, 5, Internal::OverlayRect::Mode::fadeOut);
                                event->accept(); // Mark the event as handled
                                return;
							}
                        }
                    }                    
                    break;
                }
                case  Qt::Key_V:
                {
                    if (selectedIndexes.isEmpty() || !m_enableCellPaste)
                        break;
					// For each selected cell, paste the clipboard data
					bool hasAccepted = false;
                    for (const QModelIndex& index : selectedIndexes)
                    {
                        CellDataBasePtr cellData = m_model->getCellData(index.row(), index.column());
                        if (cellData)
                        {
                            if (m_copyPasteDelegate)
                            {
                                if(m_copyPasteDelegate->pasteCell(cellData, index))
                                {
                                    highlightCell(index.row(), index.column(), 2, m_pasteCellIndicatorColor, 5, Internal::OverlayRect::Mode::fadeOut);
                                    hasAccepted = true;
								}
                                
                            }
                        }
					}
                    if (hasAccepted)
                    {
                        event->accept(); // Mark the event as handled
                        return;
                    }   
                    break;
                }
            }
        }

        QTableView::keyPressEvent(event); // Call base class for other key events
    }


    void TableView::resizeEvent(QResizeEvent* event) 
    {
        QTableView::resizeEvent(event);
        // Adjust the overlay position and size to match the table viewport
        QRect viewportRect = geometry(); // Get the viewport's geometry
        viewportRect.setX(0); // Set the X position to 0
        viewportRect.setY(0); // Set the Y position to 0
        m_overlay->setGeometry(viewportRect); // Position the overlay over the viewport
        doRelayout();
    }
    void TableView::doRelayout()
    {
        resizeColumnsToContents();
        resizeRowsToContents();
    }


    QRect TableView::getColumnRect(unsigned int column) const
    {
        QRect rect;
        for (int row = 0; row < model()->rowCount(); ++row)
        {
            QRect cellRect = visualRect(model()->index(row, column));
            if (rect.isNull())
                rect = cellRect;
            else
                rect = rect.united(cellRect);
        }
        int height = rect.height() + horizontalHeader()->height() + CELL_PADDING;
        int width = rect.width() + CELL_PADDING;

        rect.setX(rect.x() + verticalHeader()->width());
        rect.setY(rect.y());
        rect.setHeight(height);
        rect.setWidth(width);
        return rect;
    }
    QRect TableView::getRowRect(unsigned int row) const
    {

        QRect rect;
        for (int column = 0; column < model()->columnCount(); ++column)
        {
            QRect cellRect = visualRect(model()->index(row, column));
            if (rect.isNull())
                rect = cellRect;
            else
                rect = rect.united(cellRect);
        }
        int height = rect.height() + CELL_PADDING;
        int width = rect.width() + verticalHeader()->width() + CELL_PADDING;

        rect.setX(rect.x());
        rect.setY(rect.y() + horizontalHeader()->height());
        rect.setHeight(height);
        rect.setWidth(width);
        return rect;
    }
    QRect TableView::getCellRect(unsigned int row, unsigned int column) const
    {
        QRect rect = visualRect(model()->index(row, column));
        int width = rect.width() + CELL_PADDING;
        int height = rect.height() + CELL_PADDING;
        rect.setX(rect.x() + verticalHeader()->width());
        rect.setY(rect.y() + horizontalHeader()->height());
        rect.setHeight(height);
        rect.setWidth(width);
        return rect;
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
        }

        // Open editors for all newly selected cells
        for (const QModelIndex& index : selected.indexes())
            openPersistentEditor(index);
        
        doRelayout();        
    }

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