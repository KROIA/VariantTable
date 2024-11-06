#include "TableView.h"

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