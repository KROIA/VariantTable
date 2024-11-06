#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"


class QLineEdit;

namespace VariantTable
{
	class VARIANT_TABLE_EXPORT LineEdit : public CellDataBase
	{
		VT_CELL_DATA_OBJ(LineEdit);
	public:
		LineEdit();
		LineEdit(const LineEdit& other);
		LineEdit(const QString& text);



		void setText(const QString& text);
		const QString& getText() const;

		void setData(const QVariant& data) override;
		void setData(QWidget* editor) override;
		QVariant getData() const override;
		void getData(QWidget* editor) override;

		QSize getSizeHint(const QStyleOptionViewItem& option) const override;

		QWidget* createEditorWidget(QWidget* parent) const override;
		void drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const override;
		QString getToolTip() const override;
	private:
		QString m_text;
		mutable QLineEdit* m_editor = nullptr;
	};
}