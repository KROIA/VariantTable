#pragma once

#include "VariantTable_base.h"
#include "Model.h"
#include <QTableView>
#include <QHeaderView>
#include <QTimer>
#include <QPainter>
#include "OverlayRect.h"

namespace VariantTable
{
    class VARIANT_TABLE_EXPORT TableView : public QTableView
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

			void selectRow(unsigned int row);
			void selectColumn(unsigned int column);
			void selectCell(unsigned int row, unsigned int column);

			void deselectRow(unsigned int row);
			void deselectColumn(unsigned int column);
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

		std::vector<Internal::OverlayRect::Rect> getDrawRects()
		{
			std::vector<Internal::OverlayRect::Rect> rects;
			/*if (m_column >= 0)
			{
				rects.push_back(getColumnRect(m_column));
				rects.push_back(getRowRect(m_column));
			}*/
			return rects;
		}

		QRect getColumnRect(unsigned int column)
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
		QRect getRowRect(unsigned int row)
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
		QRect getCellRect(unsigned int row, unsigned int column)
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