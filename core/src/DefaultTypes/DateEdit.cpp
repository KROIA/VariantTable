#include "DefaultTypes/DateEdit.h"
#include "IconManager.h"

#include <QDateEdit>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{
	QString DateEdit::s_format = "dd.MM.yyyy";


	DateEdit::DateEdit()
		: CellDataBase()
	{

	}
	DateEdit::DateEdit(const DateEdit& other)
		: CellDataBase(other)
	{

	}
	DateEdit::DateEdit(const QDate& date)
		: CellDataBase()
		, m_date(date)
	{

	}

	void DateEdit::setFormat(const QString& format)
	{
		s_format = format;
	}
	const QString& DateEdit::getFormat()
	{
		return s_format;
	}


	void DateEdit::setDate(const QDate& date)
	{
		m_date = date;
		if (m_editor)
			m_editor->setDate(m_date);
	}
	QDate DateEdit::getDate() const
	{
		if (m_editor)
			return m_editor->date();
		return m_date;
	}


	void DateEdit::setData(const QVariant& data)
	{
		m_date = data.toDate();
	}
	void DateEdit::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
		//QDateEdit* DateEdit = qobject_cast<QDateEdit*>(editor);
		if (m_editor)
		{
			m_date = m_editor->date();
		}
	}
	QVariant DateEdit::getData() const
	{
		return QVariant(m_date);
	}
	void DateEdit::getData(QWidget* editor)
	{
		VT_UNUSED(editor);
		//QDateEdit* DateEdit = qobject_cast<QDateEdit*>(editor);
		if (m_editor)
		{
			m_editor->setDate(m_date);
		}
	}



	QWidget* DateEdit::createEditorWidget(QWidget* parent) const
	{
		if (m_editor)
			return m_editor;
		//QWidget* editor = new QWidget(parent);
		// Add Layout
		//QVBoxLayout* layout = new QVBoxLayout(parent);
		//layout->setContentsMargins(5, 5, 5, 5);
		//editor->setLayout(layout);
		m_editor = new QDateEdit(parent);
		m_editor->setDisplayFormat(s_format);
		m_editor->setDate(m_date);
		//layout->addWidget(m_editor);

		return m_editor;
	}
	void DateEdit::drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const
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
		QString text = m_date.toString(s_format);
		QRect textRect = QRect(xPos + TL.x() + size, yOffset + TL.y(), rect.width() - size, size);
		painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);

		// Draw icon
		QRect iconRect = QRect(xPos + TL.x(), yOffset + TL.y(), size, size);
		painter->drawPixmap(iconRect, IconManager::getIcon("calendar.png").pixmap(size, size));
	}
	QString DateEdit::getToolTip() const
	{
		return m_date.toString(s_format);
	}
	void DateEdit::editorWidgetDestroyed() const
	{
		m_editor = nullptr;
	}
}