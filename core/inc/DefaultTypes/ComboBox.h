#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"


class QComboBox;

namespace VariantTable
{
	class VARIANT_TABLE_API ComboBox : public CellDataBase
	{
		VT_CELL_DATA_OBJ(ComboBox);
	public:
		ComboBox();
		ComboBox(const ComboBox& other);
		ComboBox(const QStringList& options);



		void setOptions(const QStringList& text);
		const QStringList& getOptions() const;

		void setCurrentIndex(int index);
	    int getCurrentIndex() const;


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
		QStringList m_options;
		int m_selectedIndex = -1;

		mutable QComboBox* m_combo = nullptr;

		static QString s_comboBoxIcon;
	};
}