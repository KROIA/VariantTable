#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"
#include <QTime>


class QTimeEdit;

namespace VariantTable
{
	class VARIANT_TABLE_EXPORT TimeEdit : public CellDataBase
	{
		VT_CELL_DATA_OBJ(TimeEdit);
		public:
		static void setFormat(const QString& format);
		static const QString& getFormat();

		TimeEdit();
		TimeEdit(const TimeEdit& other);
		TimeEdit(const QTime& time);



		void setTime(const QTime& time);
		QTime getTime() const;

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
		QTime m_time;
		mutable QTimeEdit* m_editor = nullptr;
		
		static QString s_format;
		static QString s_timeIcon;
	};
}