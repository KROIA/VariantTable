#include "DefaultTypes/LineEdit.h"
#include "IconManager.h"

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
		if (m_editor)
			m_editor->setText(text);
	}
	QString LineEdit::getText() const
	{
		if(m_editor)
			return m_editor->text();
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

	QWidget* LineEdit::createEditorWidget(QWidget* parent) const
	{
		if (m_editor)
			return m_editor;
		//QWidget* editor = new QWidget(parent);
		// Add Layout
		//QVBoxLayout* layout = new QVBoxLayout(parent);
		//layout->setContentsMargins(5, 5, 5, 5);
		//editor->setLayout(layout);
		m_editor = new QLineEdit(parent);
		m_editor->setText(m_text);

		return m_editor;
	}
	void LineEdit::drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const
	{
		QRect rect = option.rect;
		QPoint TL = rect.topLeft();
		float xPos = 5;
		float height = rect.height();
		float size = 20;
		float yOffset = (height - size) / 2;

		QPen origPen = painter->pen();
		QBrush origBrush = painter->brush();
		// Set the brush color
		painter->setBrush(getColor());
		painter->setPen(getColor());
		painter->drawRect(rect); // x, y, width, height
		painter->setBrush(origBrush);
		painter->setPen(origPen);

		// Draw text
		QRect textRect = QRect(xPos + TL.x() + size*2, yOffset + TL.y(), rect.width() - size, size);
		painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, m_text);

		// Draw icon
		QRect iconRect = QRect(xPos + TL.x(), yOffset + TL.y(), size*2, size);
		painter->drawPixmap(iconRect, IconManager::getIcon("lineEdit.png").pixmap(size, size));
	}
	QString LineEdit::getToolTip() const
	{
		return m_text;
	}
	void LineEdit::editorWidgetDestroyed() const
	{
		m_editor = nullptr;
	}
}