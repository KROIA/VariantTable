#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"


class QCheckBox;

namespace VariantTable
{
	class VARIANT_TABLE_EXPORT CheckBox : public CellDataBase
	{
		VT_CELL_DATA_OBJ(CheckBox);
		public:
			CheckBox();
			CheckBox(const CheckBox& other);
			CheckBox(const QString& text, bool value = false);

			

			void setText(const QString& text);
			const QString& getText() const;
			void setChecked(bool value);
			bool isChecked() const;

			void setData(const QVariant& data) override;
			void setData(QWidget* editor) override;
			QVariant getData() const override;
			void getData(QWidget* editor) override;

			QWidget* createEditorWidget(QWidget* parent) const override;
			void drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const override;
			QString getToolTip() const override;
			void editorWidgetDestroyed() const override;
		private:
			QString m_text;
			bool m_value;
			mutable QCheckBox* m_editor = nullptr;
	};
}