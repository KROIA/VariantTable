#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"
#include <QAbstractTableModel>
#include <QVector>
#include <QVariant>
#include <QDebug>
#include <type_traits>
#include <typeinfo>

class QTableView;

namespace VariantTable
{
	class Delegate;
    class TableView;
    class VARIANT_TABLE_EXPORT Model : public QAbstractTableModel {
        Q_OBJECT

        public:

        template<typename CellDataType>
        static void registerType()
        {
            // Check base type
            static_assert(std::is_base_of<CellDataBase, CellDataType>::value,
                          "CelllDataType must be derived from CellDataBase");
			
			size_t typeId = typeid(CellDataType).hash_code();
            if (s_cellFactories.find(typeId) != s_cellFactories.end())
            {
				qDebug() << "Type already registered";
                return;
            }
			s_cellFactories[typeId] = new CellDataType();
        }
		static const std::unordered_map<size_t, CellDataBase*>& getRegisteredTypes()
		{
			return s_cellFactories;
		}

        Model(int rows, int cols, TableView* parent = nullptr);
        
		void setTableView(TableView* view);
        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

        bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

        Qt::ItemFlags flags(const QModelIndex& index) const;

		static const CellDataBase* getRegisteredType(size_t typeId);
        template<typename CellDataType>
		static const CellDataType* getRegisteredType()
        {
			// Check base type
			static_assert(std::is_base_of<CellDataBase, CellDataType>::value,
						  "CelllDataType must be derived from CellDataBase");
			size_t typeId = typeid(CellDataType).hash_code();
			return dynamic_cast<const CellDataType*>(getRegisteredType(typeId));
        }

		Delegate* getDelegate() const { return m_delegate; }

		void setCellData(int row, int col, CellDataBase* data);
		CellDataBase* getCellData(int row, int col) const;

		bool isIndexSelected(const QModelIndex& index) const;
		void onNewEditorCreated(QWidget* editor, const QModelIndex& index) const;

        private:
        struct CellData
        {
            CellDataBase* data;
        };
        QVector<QVector<CellData>> m_data;  // 2D array of data values
        Delegate *m_delegate;
		TableView* m_view;

		static std::unordered_map<size_t, CellDataBase*> s_cellFactories;
		static bool s_defaultFactoriesAdded;
       
    };
}