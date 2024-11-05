#pragma once

#include "VariantTable_base.h"
#include <QWidget>
#include <QVariant>

namespace VariantTable
{
	class CellWidgetBase : public QWidget
	{
		Q_OBJECT
		public:
		CellWidgetBase() = delete;
		CellWidgetBase(const CellWidgetBase&) = delete;

		virtual ~CellWidgetBase() = default;

		virtual void setData(const QVariant& data) = 0;
		virtual QVariant getData() const = 0;

	};
}