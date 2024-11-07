#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"
#include <QDateTime>


class QDateTimeEdit;

namespace VariantTable
{
	class VARIANT_TABLE_EXPORT DateTimeEdit : public CellDataBase
	{
		VT_CELL_DATA_OBJ(DateTimeEdit);
		public:
		static void setFormat(const QString& format);
		static const QString& getFormat();

		DateTimeEdit();
		DateTimeEdit(const DateTimeEdit& other);
		DateTimeEdit(const QDateTime& dateTime);



		void setDate(const QDate& date);
		QDate getDate() const;
		void setTime(const QTime& time);
		QTime getTime() const;
		void setDateTime(const QDateTime& dateTime);
		QDateTime getDateTime() const;

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
		QDateTime m_dateTime;
		mutable QDateTimeEdit* m_editor = nullptr;

		static QString s_format;
		static QString s_dateIcon;
		
	};
}