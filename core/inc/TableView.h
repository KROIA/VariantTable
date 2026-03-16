#pragma once

#include "VariantTable_base.h"

#include <QTableView>
#include "OverlayRect.h"

namespace VariantTable
{
	class Model;
    class VARIANT_TABLE_API TableView : public QTableView
    {
        friend class Model;
		friend class Internal::OverlayRect;
		friend class Internal::OverlayRect::Rect;
		Q_OBJECT

			static constexpr int CELL_PADDING = 1;
        public:
            TableView(QWidget* parent = nullptr);

		    Model* getModel() const { return m_model; }

			void scrollToRow(unsigned int row);
			void scrollToColumn(unsigned int column);

			bool isIndexSelected(const QModelIndex& index) const;
			QVector<unsigned int> getSelectedRows() const;
			QVector<unsigned int> getSelectedColumns() const;

			void selectRow(unsigned int row);
			void selectRows(const QVector<unsigned int>& rows);

			void selectColumn(unsigned int column);
			void selectColumns(const QVector<unsigned int>& columns);

			void selectCell(unsigned int row, unsigned int column);

			void deselectRow(unsigned int row);
			void deselectRows(const QVector<unsigned int>& rows);

			void deselectColumn(unsigned int column);
			void deselectColumns(const QVector<unsigned int>& columns);

			void deselectCell(unsigned int row, unsigned int column);

			void highlightRow(unsigned int row);
			void highlightRow(unsigned int row, unsigned int lineWidth, 
							  const QColor &lineColor, float speed, Internal::OverlayRect::Mode mode);
			void highlightColumn(unsigned int column);
			void highlightColumn(unsigned int column, unsigned int lineWidth,
								 const QColor& lineColor, float speed, Internal::OverlayRect::Mode mode);
			void highlightCell(unsigned int row, unsigned int column);
			void highlightCell(unsigned int row, unsigned int column, unsigned int lineWidth,
							   const QColor& lineColor, float speed, Internal::OverlayRect::Mode mode);

	

		protected:
		void resizeEvent(QResizeEvent* event);
		void doRelayout();

		QRect getColumnRect(unsigned int column) const;
		QRect getRowRect(unsigned int row) const;
		QRect getCellRect(unsigned int row, unsigned int column) const;

		private slots:
		
            void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

        private:
            //void onNewEditorCreated(QWidget* editor, const QModelIndex& index);
            void focusInEvent(QFocusEvent* event) override;

            Model* m_model;
            bool m_firstFocus; // Flag to track the first focus event

            
           // int m_column;    // Selected column index
           // int m_row;    // Selected row index

			Internal::OverlayRect* m_overlay;
    };
}