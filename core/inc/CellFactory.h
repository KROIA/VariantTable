#pragma once

#include "VariantTable_base.h"
#include "CellWidgetBase.h"
#include "CellDataBase.h"
#include <QSize>
#include <type_traits>

#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>


class QModelIndex;
class QAbstractItemModel;
class QStyleOptionViewItem;




namespace VariantTable
{
	class CellFactoryBase
	{
		public:
		CellFactoryBase() = default;
		virtual ~CellFactoryBase() = default;

		virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const = 0;
		virtual void setEditorData(QWidget* editor, const QModelIndex& index) const = 0;
		virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const = 0;
		virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const = 0;
	
		virtual size_t getFactoryId() const = 0;
	};


	template<typename CelllDataType, typename WidgetType>
	class CellFactory : public CellFactoryBase
	{
		static_assert(std::disjunction<
					  std::is_base_of<CellWidgetBase, WidgetType>,
					  std::is_base_of<QCheckBox, WidgetType>,
					  std::is_base_of<QLineEdit, WidgetType>,
					  std::is_base_of<QComboBox, WidgetType>>::value, 
					  "WidgetType must be derived from Base");

		static_assert(std::disjunction<
					  std::is_base_of<CellDataBase, CelllDataType>,
					  std::is_base_of<QVariant, CelllDataType>>::value,
					  "CelllDataType must be derived from CellDataBase");
		public:
		

		CellFactory() = default;
		~CellFactory() = default;

		QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override
		{
			Q_UNUSED(index);
			Q_UNUSED(option);
			return new WidgetType(parent);
		}
		void setEditorData(QWidget* editor, const QModelIndex& index) const override
		{
			WidgetType* widget = qobject_cast<WidgetType*>(editor);
			if (!widget)
				return;
			QVariant value = index.model()->data(index, Qt::EditRole);

			// Conditional implementation based on the base class
			if constexpr (std::is_base_of<CellWidgetBase, WidgetType>::value) 
			{
				CelllDataType cellData = value.value<CelllDataType>(); // Extract custom type
				static_cast<CellWidgetBase*>(editor)->setData(cellData);
			}
			else if constexpr (std::is_base_of<QCheckBox, WidgetType>::value)
			{
				widget->setChecked(value.toBool());
			}
			else if constexpr (std::is_base_of<QLineEdit, WidgetType>::value)
			{
				widget->setText(value.toString());
			}
			else if constexpr (std::is_base_of<QComboBox, WidgetType>::value)
			{
				widget->setCurrentText(value.toString());
			}
			else {
				// Handle other cases or provide a default implementation
				return;
			}			
		}
		void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override
		{
			QVariant value;
			WidgetType* widget = qobject_cast<WidgetType*>(editor);
			if constexpr (std::is_base_of<CellWidgetBase, WidgetType>::value)
			{
				value.setValue(widget->getData());
			}
			else if constexpr (std::is_base_of<QCheckBox, WidgetType>::value)
			{
				value.setValue(widget->isChecked());
			}
			else if constexpr (std::is_base_of<QLineEdit, WidgetType>::value)
			{
				value.setValue(widget->text());
			}
			else if constexpr (std::is_base_of<QComboBox, WidgetType>::value)
			{
				value.setValue(widget->currentText());
			}
			else {
				// Handle other cases or provide a default implementation
				return;
			}
			model->setData(index, value, Qt::EditRole);
		}
		QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override
		{
			// Get the cell data
			const QVariant value = index.model()->data(index, Qt::EditRole);
			if constexpr (std::is_base_of<CellDataBase, CelllDataType>::value)
			{
				CelllDataType cellData = value.value<CelllDataType>(); // Extract custom type
				return cellData->getSizeHint(option);
			}
			else if constexpr (std::is_base_of<QVariant, CelllDataType>::value)
			{
				return QSize(100, 20);
			}
			else {
				// Handle other cases or provide a default implementation
				return QSize();
			}
			return QSize();
		}

		size_t getFactoryId() const override
		{
			return getFactoryIdStatic();
		}

		static constexpr size_t getFactoryIdStatic()
		{
			return typeid(CellFactory<CelllDataType, WidgetType>).hash_code();
		}

		
	};
}