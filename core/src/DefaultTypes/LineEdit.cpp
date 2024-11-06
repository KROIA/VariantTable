#include "DefaultTypes/LineEdit.h"

#include <QLineEdit>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{
	LineEdit::LineEdit()
		: CellDataBase()
		, m_text("QLineEdit")
	{

	}
	LineEdit::LineEdit(const LineEdit& other)
		: CellDataBase(other)
		, m_text(other.m_text)
	{

	}
	LineEdit::LineEdit(const QString& text)
		: CellDataBase()
		, m_text(text)
	{

	}


	void LineEdit::setText(const QString& text)
	{
		m_text = text;
	}
	const QString& LineEdit::getText() const
	{
		return m_text;
	}


	void LineEdit::setData(const QVariant& data)
	{
		m_text = data.toString();
	}
	void LineEdit::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
		//QLineEdit* LineEdit = qobject_cast<QLineEdit*>(editor);
		if (m_editor)
		{
			m_text = m_editor->text();
		}
	}
	QVariant LineEdit::getData() const
	{
		return QVariant(m_text);
	}
	void LineEdit::getData(QWidget* editor)
	{
		VT_UNUSED(editor);
		//QLineEdit* LineEdit = qobject_cast<QLineEdit*>(editor);
		if (m_editor)
		{
			m_editor->setText(m_text);
		}
	}

	QSize LineEdit::getSizeHint(const QStyleOptionViewItem& option) const
	{

		if (m_editor)
		{
			QSize size = m_editor->sizeHint();
			return size;
		}
		return QSize(option.rect.width(), option.rect.height());
	}

	QWidget* LineEdit::createEditorWidget(QWidget* parent) const
	{
		if (m_editor)
			return m_editor->parentWidget();
		QWidget* editor = new QWidget(parent);
		// Add Layout
		QVBoxLayout* layout = new QVBoxLayout(editor);
		layout->setContentsMargins(5, 5, 5, 5);
		editor->setLayout(layout);
		m_editor = new QLineEdit(editor);
		m_editor->setText(m_text);
		layout->addWidget(m_editor);

		// Destroy event
		QObject::connect(layout, &QWidget::destroyed, parent, [this]()
			{
				m_editor = nullptr;
			});
		return editor;
	}
	void LineEdit::drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const
	{
		QRect rect = option.rect;
		QPoint TL = rect.topLeft();
		float xPos = 5;
		float height = rect.height();
		float size = 20;
		float yOffset = (height - size) / 2;

		// Draw text
		QRect textRect = QRect(xPos + TL.x(), yOffset + TL.y(), rect.width() - size, size);
		painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, m_text);
	}
	QString LineEdit::getToolTip() const
	{
		return m_text;
	}
}