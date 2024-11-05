#pragma once

#include "VariantTable_base.h"
#include "Model.h"
#include <QTableView>
#include <QHeaderView>

namespace VariantTable
{
    class TableView : public QTableView {
        friend class Model;
        Q_OBJECT

        public:
        TableView(QWidget* parent = nullptr)
            : QTableView(parent) 
			, m_model(nullptr)
			, m_firstFocus(true)
        {
			m_model = new Model(5,3, this);
            // Connect selection changes to slot
            connect(selectionModel(), &QItemSelectionModel::selectionChanged,
                    this, &TableView::onSelectionChanged);

            // set all columns to equal width
            horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        }

		Model* getModel() const { return m_model; }

        private slots:
        void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected) {
            // Close editors for all deselected cells
            for (const QModelIndex& index : deselected.indexes()) {
                closePersistentEditor(index);
                setRowHeight(index.row(), verticalHeader()->defaultSectionSize());
                setColumnWidth(index.column(), horizontalHeader()->defaultSectionSize());
            }

            // Open editors for all newly selected cells
            for (const QModelIndex& index : selected.indexes()) {
                openPersistentEditor(index);
                resizeRowToContents(index.row());
                resizeColumnToContents(index.column());
            }
        }
        private:
        void onNewEditorCreated(QWidget* editor, const QModelIndex& index)
        {
			VT_UNUSED(editor);
            if (index.isValid()) {
                // Resize the row and column of the selected cell to fit its contents
                resizeRowToContents(index.row());
                resizeColumnToContents(index.column());
            }

        }
        void focusInEvent(QFocusEvent* event) override {
            if (m_firstFocus) {
                m_firstFocus = false; // Allow focus on subsequent events
                // Skip calling the base class to prevent auto selection
                return;
            }
            // Call the base class to allow default behavior
            QTableView::focusInEvent(event);
        }

        Model* m_model;
        bool m_firstFocus; // Flag to track the first focus event
    };
}