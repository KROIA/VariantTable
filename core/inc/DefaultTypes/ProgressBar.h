#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"


class QProgressBar;

namespace VariantTable
{
	class VARIANT_TABLE_API ProgressBar : public CellDataBase
	{
		VT_CELL_DATA_OBJ(ProgressBar);
	public:
		enum CopyPastePolicy : int
		{
			ProgressValue = 0x1,
		};
		ProgressBar();
		ProgressBar(const ProgressBar& other);
		ProgressBar(int min, int max, int value);

		void setMinimum(int min);
		int getMinimum() const { return m_min; }

		void setMaximum(int max);
		int getMaximum() const { return m_max; }

		void setValue(int progress);
		int getValue() const;

		void setOrientation(Qt::Orientation orientation);
		Qt::Orientation getOrientation() const { return m_orientation; }


		bool setData(const QVariant& data) override;
		void setData(QWidget* editor) override;
		QVariant getData() const override;
		void getData(QWidget* editor) override;

		QWidget* createEditorWidget(QWidget* parent) override;
		QString getToolTip() const override;
		void editorWidgetDestroyed() override;
		void drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const override;
		void updateEditorPlaceholderText() const;

		/**
		* CopyPastePolicy enum values can be combined using bitwise OR to specify what data should be included when copying.
		* Defines which aspects of the ComboBox should be included when copying to the clipboard.
		* For example, you can choose to include only the text list, only the selected index, or both.
		*/
		void setCopyPolicy(int policy) { m_copyPolicy = policy; }
		void addCopyPolicy(CopyPastePolicy policy) { m_copyPolicy |= policy; }
		void removeCopyPolicy(CopyPastePolicy policy) { m_copyPolicy &= ~policy; }
		int getCopyPolicy() const { return m_copyPolicy; }
		bool hasCopyPolicy(CopyPastePolicy policy) const { return (m_copyPolicy & policy) != 0; }

		/**
		* CopyPastePolicy enum values can be combined using bitwise OR to specify what data should be included when pasting.
		* Defines which aspects of the ComboBox can be applied when pasting from the clipboard.
		* For example, you can choose to include only the text list, only the selected index, or both.
		*/
		void setPastePolicy(int policy) { m_pastePolicy = policy; }
		void addPastePolicy(CopyPastePolicy policy) { m_pastePolicy |= policy; }
		void removePastePolicy(CopyPastePolicy policy) { m_pastePolicy &= ~policy; }
		int getPastePolicy() const { return m_pastePolicy; }
		bool hasPastePolicy(CopyPastePolicy policy) const { return (m_pastePolicy & policy) != 0; }

		std::shared_ptr<ClipboardData> createClipboadData() const override;
		bool onPaste(std::shared_ptr<ClipboardData> pasteData) override;
	private:
		
		void drawLoadingBar(QPainter* painter, const QRect& rect, int percentage,
							const QPixmap& bar) const;
		int m_min = 0;
		int m_max = 100;
		int m_progress = 0;
		Qt::Orientation m_orientation = Qt::Horizontal;

		QProgressBar* m_bar = nullptr;

		int m_copyPolicy = CopyPastePolicy::ProgressValue;
		int m_pastePolicy = CopyPastePolicy::ProgressValue;

		static QString s_ProgressBarIcon;
	};
}