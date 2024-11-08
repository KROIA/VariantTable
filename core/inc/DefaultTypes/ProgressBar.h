#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"


class QProgressBar;

namespace VariantTable
{
	class VARIANT_TABLE_EXPORT ProgressBar : public CellDataBase
	{
		VT_CELL_DATA_OBJ(ProgressBar);
	public:
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


		void setData(const QVariant& data) override;
		void setData(QWidget* editor) override;
		QVariant getData() const override;
		void getData(QWidget* editor) override;

		QWidget* createEditorWidget(QWidget* parent) const override;
		QString getToolTip() const override;
		void editorWidgetDestroyed() const override;
		void updateIcon() override;
		void drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const override;
	private:
		void updateText();
		void drawLoadingBar(QPainter* painter, const QRect& rect, int percentage,
							const QPixmap& bar) const;
		int m_min = 0;
		int m_max = 100;
		int m_progress = 0;
		Qt::Orientation m_orientation = Qt::Horizontal;

		mutable QProgressBar* m_bar = nullptr;

		static QString s_ProgressBarIcon;
	};
}