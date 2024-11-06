#include "DefaultTypes/TextEdit.h"
#include "IconManager.h"

#include <QTextEdit>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{
	TextEdit::TextEdit()
		: CellDataBase()
		, m_text("QTextEdit")
	{

	}
	TextEdit::TextEdit(const TextEdit& other)
		: CellDataBase(other)
		, m_text(other.m_text)
	{

	}
	TextEdit::TextEdit(const QString& text)
		: CellDataBase()
		, m_text(text)
	{

	}


	void TextEdit::setText(const QString& text)
	{
		m_text = text;
		if (m_editor)
			m_editor->setText(text);
	}
	QString TextEdit::getText() const
	{
		if (m_editor)
			return m_editor->toPlainText();
		return m_text;
	}


	void TextEdit::setData(const QVariant& data)
	{
		m_text = data.toString();
	}
	void TextEdit::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
		//QTextEdit* TextEdit = qobject_cast<QTextEdit*>(editor);
		if (m_editor)
		{
			m_text = m_editor->toPlainText();
		}
	}
	QVariant TextEdit::getData() const
	{
		return QVariant(m_text);
	}
	void TextEdit::getData(QWidget* editor)
	{
		VT_UNUSED(editor);
		//QTextEdit* TextEdit = qobject_cast<QTextEdit*>(editor);
		if (m_editor)
		{
			m_editor->setText(m_text);
		}
	}
	void TextEdit::setColor(const QColor& color)
	{
		CellDataBase::setColor(color);
		CellDataBase::applyColor(m_editor);
	}
	QSize TextEdit::getSizeHint(const QStyleOptionViewItem& option) const
	{

		if (m_editor)
		{
			QSize size = m_editor->sizeHint();
			return size;
		}
		return QSize(option.rect.width(), option.rect.height());
	}

	QWidget* TextEdit::createEditorWidget(QWidget* parent) const
	{
		if (m_editor)
			return m_editor;
		//QWidget* editor = new QWidget(parent);
		// Add Layout
		//QVBoxLayout* layout = new QVBoxLayout(parent);
		//layout->setContentsMargins(5, 5, 5, 5);
		//editor->setLayout(layout);
		m_editor = new QTextEdit(parent);
		m_editor->setText(m_text);
		//layout->addWidget(m_editor);

		CellDataBase::applyColor(m_editor);

		// Destroy event
		QObject::connect(m_editor, &QWidget::destroyed, parent, [this]()
						 {
							 m_editor = nullptr;
						 });
		return m_editor;
	}
	void TextEdit::drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const
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
		QString text = m_text.split("\n").first();
		QRect textRect = QRect(xPos + TL.x() + size * 2, yOffset + TL.y(), rect.width() - size, size);
		painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);

		// Draw icon
		QRect iconRect = QRect(xPos + TL.x(), yOffset + TL.y(), size * 2, size);
		painter->drawPixmap(iconRect, IconManager::getIcon("lineEdit.png").pixmap(size, size));
	}
	QString TextEdit::getToolTip() const
	{
		return m_text;
	}
}