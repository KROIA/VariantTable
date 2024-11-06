#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"
#include <QAbstractTableModel>
#include <QVector>
#include <QVariant>
#include <QDebug>
#include <type_traits>
#include <typeinfo>
#include <QMimeData>


class QTableView;

namespace VariantTable
{
	class Delegate;
    class TableView;
    class VARIANT_TABLE_EXPORT Model : public QAbstractTableModel 
    {
        Q_OBJECT

        public:
        Model(TableView* parent = nullptr);
        ~Model();
        
		void setTableView(TableView* view);
        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

        Qt::ItemFlags flags(const QModelIndex& index) const;

		

		bool isIndexSelected(const QModelIndex& index) const;
        QVector<unsigned int> getSelectedRows() const;
		QVector<unsigned int> getSelectedColumns() const;

		// Table operations
     //   bool insertRow(int row, const QModelIndex& parent = QModelIndex()) override;
		bool insertRow(int row, const QVector<CellDataBasePtr>& data);
        bool insertRow(int row, CellDataTypeID defaultType);
        bool insertRow(int row, CellDataBasePtr typeTemplate);
        bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

        
       // bool insertColumn(int col, const QModelIndex& parent = QModelIndex());
		bool insertColumn(int col, const QVector<CellDataBasePtr>& data);
		bool insertColumn(int col, CellDataTypeID defaultType);
		bool insertColumn(int col, CellDataBasePtr typeTemplate);
		bool insertColumns(int col, int count, const QModelIndex& parent = QModelIndex()) override;


        
       // bool removeRow(int row, const QModelIndex& parent = QModelIndex()) override;
        bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;


       // bool removeColumn(int column, const QModelIndex& parent = QModelIndex()) override;
        bool removeColumns(int column, int count, const QModelIndex& parent = QModelIndex()) override;


		bool moveRowUp(unsigned int row, unsigned int amount = 1);
		//bool moveRowsUp(QVector<unsigned int> rows, unsigned int amount = 1);

		bool moveRowDown(unsigned int row, unsigned int amount = 1);
		//bool moveRowsDown(QVector<unsigned int> rows, unsigned int amount = 1);

		bool swapRows(unsigned int row1, unsigned int row2);
		bool moveRow(unsigned int sourceRow, unsigned int destinationRow);


      //  bool moveRow(const QModelIndex& sourceParent, int sourceRow,
      //      const QModelIndex& destinationParent, int destinationChild) override;
      //  bool moveColumn(const QModelIndex& sourceParent, int sourceColumn,
      //      const QModelIndex& destinationParent, int destinationChild) override;



        void setCellData(int row, int col, CellDataBasePtr data);
        CellDataBasePtr getCellData(int row, int col) const;


        Delegate* getDelegate() const { return m_delegate; }

       
        private:
            //void onNewEditorCreated(QWidget* editor, const QModelIndex& index) const;
        struct CellData
        {
            CellDataBasePtr data = nullptr;
        };
        QVector<QVector<CellData>> m_data;  // 2D array of data values
        Delegate *m_delegate;
		TableView* m_view;

		
       
    };
}