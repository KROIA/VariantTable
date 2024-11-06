#pragma once

#include "VariantTable_base.h"
#include "Model.h"
#include <QTableView>
#include <QHeaderView>

namespace VariantTable
{
    class TableView : public QTableView 
    {
        friend class Model;
        Q_OBJECT

        public:
            TableView(QWidget* parent = nullptr);

		    Model* getModel() const { return m_model; }

        private slots:
            void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

        private:
            //void onNewEditorCreated(QWidget* editor, const QModelIndex& index);
            void focusInEvent(QFocusEvent* event) override;

            Model* m_model;
            bool m_firstFocus; // Flag to track the first focus event
    };
}