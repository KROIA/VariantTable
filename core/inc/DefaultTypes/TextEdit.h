#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"


class QTextEdit;

namespace VariantTable
{
	class VARIANT_TABLE_API TextEdit : public CellDataBase
	{
		VT_CELL_DATA_OBJ(TextEdit);
		public:
		TextEdit();
		TextEdit(const TextEdit& other);
		TextEdit(const QString& text);



		void setText(const QString& text);
		QString getText() const;

		void setData(const QVariant& data) override;
		void setData(QWidget* editor) override;
		QVariant getData() const override;
		void getData(QWidget* editor) override;


		QWidget* createEditorWidget(QWidget* parent) const override;
		QString getToolTip() const override;
		void editorWidgetDestroyed() const override;
		void updateIcon() override;
		private:
		void updateText();
		QString m_text;
		mutable QTextEdit* m_editor = nullptr;

		static QString s_textEditIcon;
	};
}