#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"


class QCheckBox;

namespace VariantTable
{
	class VARIANT_TABLE_EXPORT CheckBoxList : public CellDataBase
	{
		VT_CELL_DATA_OBJ(CheckBoxList);
	public:
		CheckBoxList();
		CheckBoxList(const CheckBoxList& other);
		CheckBoxList(const QStringList& options);



		void setOptions(const QStringList& text);
		const QStringList& getOptions() const;

		void setCheckedIndexes(const QVector<int>& indexes);
		QVector<int> getCheckedIndexes() const;


		void setData(const QVariant& data) override;
		void setData(QWidget* editor) override;
		QVariant getData() const override;
		void getData(QWidget* editor) override;

		void setColor(const QColor& color) override;


		QSize getSizeHint(const QStyleOptionViewItem& option) const override;

		QWidget* createEditorWidget(QWidget* parent) const override;
		void drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const override;
		QString getToolTip() const override;
	private:
		QStringList m_options;
		QVector<int> m_selectedIndexes;

		mutable QWidget* m_editorWidget = nullptr;
		mutable QVector<QCheckBox*> m_checkBoxes;
	};
}