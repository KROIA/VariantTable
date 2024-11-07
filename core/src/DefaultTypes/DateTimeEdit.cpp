#include "DefaultTypes/DateTimeEdit.h"
#include "IconManager.h"

#include <QDateTimeEdit>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{
	QString DateTimeEdit::s_format = "dd.MM.yyyy hh:mm:ss";
	QString DateTimeEdit::s_dateIcon = "calendar-and-clock.png";


	DateTimeEdit::DateTimeEdit()
		: CellDataBase()
	{
		updateIcon();
	}
	DateTimeEdit::DateTimeEdit(const DateTimeEdit& other)
		: CellDataBase(other)
	{

	}
	DateTimeEdit::DateTimeEdit(const QDateTime& dateTime)
		: CellDataBase()
		, m_dateTime(dateTime)
	{
		updateIcon();
		updateText();
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
		updateText();
		dataChanged();
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
		updateText();
		dataChanged();
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
		updateText();
		dataChanged();
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
		updateText();
		dataChanged();
	}
	void DateTimeEdit::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			m_dateTime = m_editor->dateTime();
			updateText();
		}
	}
	QVariant DateTimeEdit::getData() const
	{
		return QVariant(m_dateTime);
	}
	void DateTimeEdit::getData(QWidget* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			m_editor->setDateTime(m_dateTime);
		}
	}


	QWidget* DateTimeEdit::createEditorWidget(QWidget* parent) const
	{
		if (m_editor)
			return m_editor;

		m_editor = new QDateTimeEdit(parent);
		m_editor->setDisplayFormat(s_format);
		m_editor->setDateTime(m_dateTime);

		return m_editor;
	}

	QString DateTimeEdit::getToolTip() const
	{
		return m_dateTime.toString(s_format);
	}
	void DateTimeEdit::editorWidgetDestroyed() const
	{
		m_editor = nullptr;
	}
	void DateTimeEdit::updateIcon()
	{
		setEditorPlaceholderIcon(IconManager::getIcon(s_dateIcon));
	}
	void DateTimeEdit::updateText()
	{
		setEditorPlaceholderText(m_dateTime.toString(s_format));
	}
}