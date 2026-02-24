#pragma once

#include "VariantTable_base.h"
#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QTableView>

namespace VariantTable
{
    class VARIANT_TABLE_API Delegate : public QStyledItemDelegate 
    {
        Q_OBJECT
        friend class Model;
        
        public:
        Delegate(Model* parent);

        QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
        void setEditorData(QWidget* editor, const QModelIndex& index) const override;
        void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
        QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    
        void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
        private:
		Model* m_model;
	};
}