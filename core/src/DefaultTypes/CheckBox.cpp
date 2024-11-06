#include "DefaultTypes/CheckBox.h"

#include <QCheckBox>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{
	CheckBox::CheckBox()
		: CellDataBase()
		, m_text("QCheckBox")
		, m_value(false)
	{

	}
	CheckBox::CheckBox(const CheckBox& other)
		: CellDataBase(other)
		, m_text(other.m_text)
		, m_value(other.m_value)
	{

	}
	CheckBox::CheckBox(const QString& text, bool value)
		: CellDataBase()
		, m_text(text)
		, m_value(value)
	{

	}


	void CheckBox::setText(const QString& text)
	{
		m_text = text;
	}
	const QString& CheckBox::getText() const
	{
		return m_text;
	}
	void CheckBox::setValue(bool value)
	{
		m_value = value;
	}
	bool CheckBox::getValue() const
	{
		return m_value;
	}


	void CheckBox::setData(const QVariant& data) 	
	{
		m_value = data.toBool();
	}
	void CheckBox::setData(QWidget* editor) 
	{
		VT_UNUSED(editor);
		//QCheckBox* checkBox = qobject_cast<QCheckBox*>(editor);
		if (m_editor)
		{
			m_value = m_editor->isChecked();
		}
	}
	QVariant CheckBox::getData() const 
	{
		return QVariant(m_value);
	}
	void CheckBox::getData(QWidget* editor) 
	{
		VT_UNUSED(editor);
		//QCheckBox* checkBox = qobject_cast<QCheckBox*>(editor);
		if (m_editor)
		{
			m_editor->setText(m_text);
			m_editor->setChecked(m_value);
			//checkBox->setMinimumSize(QSize(250, 100));
		}
	}

	QSize CheckBox::getSizeHint(const QStyleOptionViewItem& option) const
	{
	
		if (m_editor)
		{
			QSize size = m_editor->sizeHint();
			return size;
		}
		return QSize(option.rect.width(), option.rect.height());
	}

	QWidget* CheckBox::createEditorWidget(QWidget* parent) const
	{
		if (m_editor)
			return m_editor->parentWidget();
		QWidget* editor = new QWidget(parent);
		// Add Layout
		QVBoxLayout* layout = new QVBoxLayout(editor);
		layout->setContentsMargins(5, 5, 5, 5);
		editor->setLayout(layout);
		m_editor = new QCheckBox(editor);
		m_editor->setText(m_text);
		m_editor->setChecked(m_value);
		layout->addWidget(m_editor);

		// Destroy event
		QObject::connect(layout, &QWidget::destroyed, parent, [this]()
			{
				m_editor = nullptr;
			});
		return editor;
	}
	void CheckBox::drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const
	{
		QRect rect = option.rect;
		QPoint TL = rect.topLeft();
		float xPos = 5;
		float height = rect.height();
		float size = 20;
		float yOffset = (height - size) / 2;

		QStyleOptionButton button;
		button.rect = QRect(xPos + TL.x(), yOffset + TL.y(), size, size);
		button.state = m_value ? QStyle::State_On : QStyle::State_Off;
		button.state |= QStyle::State_Enabled;

		QApplication::style()->drawControl(QStyle::CE_CheckBox, &button, painter);

		// Draw text
		QRect textRect = QRect(button.rect.x() + button.rect.width(), yOffset + TL.y(), rect.width() - size, size);
		painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, m_text);
	}
	QString CheckBox::getToolTip() const
	{
		return (m_value?"[X] ":"[  ] ") + m_text;
	}
}