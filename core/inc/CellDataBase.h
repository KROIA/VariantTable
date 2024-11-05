#pragma once

#include "VariantTable_base.h"
#include <QVariant>
#include <QSize>
#include <QStyleOptionViewItem>

class QWidget;

namespace VariantTable
{
	class CellDataBase
	{
		public:
		CellDataBase() = default;
		virtual ~CellDataBase() = default;

		virtual CellDataBase* clone() const = 0;

		virtual void setData(const QVariant& data) = 0;
		virtual void setData(QWidget* editor) = 0;
		virtual QVariant getData() const = 0;
		virtual void getData(QWidget* editor) = 0;

		virtual QSize getSizeHint(const QStyleOptionViewItem& option) const = 0;

		virtual QWidget* createEditorWidget(QWidget *parent) const = 0;
		
	};
}