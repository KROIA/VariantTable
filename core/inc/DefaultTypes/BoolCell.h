#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"
#include <QCheckBox>

namespace VariantTable
{
	class BoolCell : public CellDataBase
	{
		public:
		BoolCell()
			: CellDataBase()
			, m_text("QCheckBox")
			, m_value(false)
		{

		}
		BoolCell(const BoolCell& other)
			: CellDataBase(other)
			, m_text(other.m_text)
			, m_value(other.m_value)
		{

		}
		BoolCell(const QString &text, bool value = false)
			: CellDataBase()
			, m_text(text)
			, m_value(value)
		{

		}
		~BoolCell() = default;

		CellDataBase* clone() const override
		{
			return new BoolCell(*this);
		}

		void setText(const QString& text)
		{
			m_text = text;
		}
		const QString &getText() const
		{
			return m_text;
		}
		void setValue(bool value)
		{
			m_value = value;
		}
		bool getValue() const
		{
			return m_value;
		}


		void setData(const QVariant& data) override
		{
			m_value = data.toBool();
		}
		void setData(QWidget* editor) override
		{
			QCheckBox* checkBox = qobject_cast<QCheckBox*>(editor);
			if (checkBox)
			{
				m_value = checkBox->isChecked();
			}
		}
		QVariant getData() const override
		{
			return QVariant(m_value);
		}
		void getData(QWidget* editor) override
		{
			QCheckBox* checkBox = qobject_cast<QCheckBox*>(editor);
			if (checkBox)
			{
				checkBox->setText(m_text);
				checkBox->setChecked(m_value);
				//checkBox->setMinimumSize(QSize(250, 100));
			}
		}

		QSize getSizeHint(const QStyleOptionViewItem& option) const override
		{
			if (m_editor)
			{
				QSize size = m_editor->sizeHint();
				return size;
			}
			return QSize(option.rect.width(), option.rect.height()/2);
		}

		QWidget* createEditorWidget(QWidget* parent) const override
		{
			m_editor = new QCheckBox(parent);
			// Destroy event
			QObject::connect(m_editor, &QWidget::destroyed, parent, [this]()
					{
						m_editor = nullptr;
					});
			return m_editor;
		}
		

		private:
		QString m_text;
		bool m_value;
		mutable QWidget* m_editor = nullptr;
	};
}