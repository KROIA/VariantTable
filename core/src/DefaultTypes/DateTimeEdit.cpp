#include "DefaultTypes/DateTimeEdit.h"
#include "IconManager.h"

#include <QDateTimeEdit>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{
	QString DateTimeEdit::s_format = "dd.MM.yyyy hh:mm:ss";


	DateTimeEdit::DateTimeEdit()
		: CellDataBase()
	{

	}
	DateTimeEdit::DateTimeEdit(const DateTimeEdit& other)
		: CellDataBase(other)
	{

	}
	DateTimeEdit::DateTimeEdit(const QDateTime& dateTime)
		: CellDataBase()
		, m_dateTime(dateTime)
	{

	}

	void DateTimeEdit::setFormat(const QString& format)
	{
		s_format = format;
	}
	const QString& DateTimeEdit::getFormat()
	{
		return s_format;
	}


	void DateTimeEdit::setDate(const QDate& date)
	{
		m_dateTime.setDate(date);
		if (m_editor)
			m_editor->setDate(date);
	}
	QDate DateTimeEdit::getDate() const
	{
		if (m_editor)
			return m_editor->date();
		return m_dateTime.date();
	}
	void DateTimeEdit::setTime(const QTime& time)
	{
		m_dateTime.setTime(time);
		if (m_editor)
			m_editor->setTime(time);
	}
	QTime DateTimeEdit::getTime() const
	{
		if (m_editor)
			return m_editor->time();
		return m_dateTime.time();
	}
	void DateTimeEdit::setDateTime(const QDateTime& dateTime)
	{
		m_dateTime = dateTime;
		if (m_editor)
			m_editor->setDateTime(dateTime);
	}
	QDateTime DateTimeEdit::getDateTime() const
	{
		if (m_editor)
			return m_editor->dateTime();
		return m_dateTime;
	}


	void DateTimeEdit::setData(const QVariant& data)
	{
		m_dateTime = data.toDateTime();
	}
	void DateTimeEdit::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
		//QDateTimeEdit* DateTimeEdit = qobject_cast<QDateTimeEdit*>(editor);
		if (m_editor)
		{
			m_dateTime = m_editor->dateTime();
		}
	}
	QVariant DateTimeEdit::getData() const
	{
		return QVariant(m_dateTime);
	}
	void DateTimeEdit::getData(QWidget* editor)
	{
		VT_UNUSED(editor);
		//QDateTimeEdit* DateTimeEdit = qobject_cast<QDateTimeEdit*>(editor);
		if (m_editor)
		{
			m_editor->setDateTime(m_dateTime);
		}
	}
	void DateTimeEdit::setColor(const QColor& color)
	{
		CellDataBase::setColor(color);
		CellDataBase::applyColor(m_editor);
	}
	QSize DateTimeEdit::getSizeHint(const QStyleOptionViewItem& option) const
	{
		if (m_editor)
		{
			QSize size = m_editor->sizeHint();
			return size;
		}
		return QSize(option.rect.width(), option.rect.height());
	}

	QWidget* DateTimeEdit::createEditorWidget(QWidget* parent) const
	{
		if (m_editor)
			return m_editor;
		//QWidget* editor = new QWidget(parent);
		// Add Layout
		//QVBoxLayout* layout = new QVBoxLayout(parent);
		//layout->setContentsMargins(5, 5, 5, 5);
		//editor->setLayout(layout);
		m_editor = new QDateTimeEdit(parent);
		m_editor->setDisplayFormat(s_format);
		m_editor->setDateTime(m_dateTime);
		//layout->addWidget(m_editor);

		CellDataBase::applyColor(m_editor);

		// Destroy event
		QObject::connect(m_editor, &QWidget::destroyed, parent, [this]()
						 {
							 m_editor = nullptr;
						 });
		return m_editor;
	}
	void DateTimeEdit::drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const
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
		QString text = m_dateTime.toString(s_format);
		QRect textRect = QRect(xPos + TL.x() + size, yOffset + TL.y(), rect.width() - size, size);
		painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);

		// Draw icon
		QRect iconRect = QRect(xPos + TL.x(), yOffset + TL.y(), size, size);
		painter->drawPixmap(iconRect, IconManager::getIcon("calendar-and-clock.png").pixmap(size, size));
	}
	QString DateTimeEdit::getToolTip() const
	{
		return m_dateTime.toString(s_format);
	}
}