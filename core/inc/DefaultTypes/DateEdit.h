#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"
#include <QDate>


class QDateEdit;

namespace VariantTable
{
	class VARIANT_TABLE_EXPORT DateEdit : public CellDataBase
	{
		VT_CELL_DATA_OBJ(DateEdit);
		public:
		static void setFormat(const QString& format);
		static const QString& getFormat();

		DateEdit();
		DateEdit(const DateEdit& other);
		DateEdit(const QDate& date);



		void setDate(const QDate& date);
		QDate getDate() const;

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
		QDate m_date;
		mutable QDateEdit* m_editor = nullptr;

		static QString s_format;
		static QString s_dateIcon;
	};
}