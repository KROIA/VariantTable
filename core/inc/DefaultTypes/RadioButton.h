#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"


class QRadioButton;

namespace VariantTable
{
	class VARIANT_TABLE_EXPORT RadioButton : public CellDataBase
	{
		VT_CELL_DATA_OBJ(RadioButton);
	public:
		RadioButton();
		RadioButton(const RadioButton& other);
		RadioButton(const QStringList& options);



		void setOptions(const QStringList& text);
		const QStringList& getOptions() const;


		void setData(const QVariant& data) override;
		void setData(QWidget* editor) override;
		QVariant getData() const override;
		void getData(QWidget* editor) override;

		QSize getSizeHint(const QStyleOptionViewItem& option) const override;

		QWidget* createEditorWidget(QWidget* parent) const override;
		void drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const override;
		QString getToolTip() const override;
	private:
		QStringList m_options;
		int m_selectedIndex = -1;

		mutable QWidget* m_editorWidget = nullptr;
		mutable QVector<QRadioButton*> m_editorButtons;
	};
}