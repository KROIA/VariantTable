#pragma once

#include "VariantTable_base.h"

#include <QTableView>
#include "OverlayRect.h"
#include "CopyPasteDelegate.h"

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

			void setEnableCellCopy(bool enable) { m_enableCellCopy = enable; }
			bool isCellCopyEnabled() const { return m_enableCellCopy; }
			void setCopyCellIndicatorColor(const QColor& color) { m_copyCellIndicatorColor = color; }
			const QColor& getCopyCellIndicatorColor() const { return m_copyCellIndicatorColor; }

			void setEnableCellPaste(bool enable) { m_enableCellPaste = enable; }
			bool isCellPasteEnabled() const { return m_enableCellPaste; }
			void setPasteCellIndicatorColor(const QColor& color) { m_pasteCellIndicatorColor = color; }
			const QColor& getPasteCellIndicatorColor() const { return m_pasteCellIndicatorColor; }
			
	
			// Key events
			void keyPressEvent(QKeyEvent* event) override;


			void setCopyPasteDelegate(std::shared_ptr<CopyPasteDelegate> delegate) { m_copyPasteDelegate = delegate; }
			template<typename DelegateType, typename... Args>
			void setCopyPasteDelegate(Args&&... args)
			{
				m_copyPasteDelegate = std::make_shared<DelegateType>(std::forward<Args>(args)...);
			}
			std::shared_ptr<CopyPasteDelegate> getCopyPasteDelegate() const { return m_copyPasteDelegate; }

		protected:
		void resizeEvent(QResizeEvent* event);
		void doRelayout();

		QRect getColumnRect(unsigned int column) const;
		QRect getRowRect(unsigned int row) const;
		QRect getCellRect(unsigned int row, unsigned int column) const;
		void currentChanged(const QModelIndex& current, const QModelIndex& previous) override;


		private slots:
		
            void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

        private:

			class DefaultCopyPasteDelegate : public CopyPasteDelegate
			{
			public:
				bool copyCell(CellDataBasePtr cellData, const QModelIndex& index) override
				{
					VT_UNUSED(index);
					m_clipboardData = cellData->copyAction();
					return m_clipboardData != nullptr;
				}
				bool pasteCell(CellDataBasePtr cellData, const QModelIndex& index) override
				{
					VT_UNUSED(index);
					if(!m_clipboardData)
						return false;
					return cellData->pasteAction(m_clipboardData);
				}
			private:
				std::shared_ptr<ClipboardData> m_clipboardData;
			};

            //void onNewEditorCreated(QWidget* editor, const QModelIndex& index);
            void focusInEvent(QFocusEvent* event) override;

            Model* m_model;
            bool m_firstFocus; // Flag to track the first focus event

			std::shared_ptr<CopyPasteDelegate> m_copyPasteDelegate = nullptr;

            
			bool m_enableCellCopy = true;
			bool m_enableCellPaste = true;
			QColor m_copyCellIndicatorColor = QColor(3, 252, 78);
			QColor m_pasteCellIndicatorColor = QColor(3, 128, 40);
			Internal::OverlayRect* m_overlay;
    };
}