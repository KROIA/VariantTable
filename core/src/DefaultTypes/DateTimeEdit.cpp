#include "DefaultTypes/DateTimeEdit.h"
#include "ClipboardData/DateTimeClipboardData.h"
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
		updateEditorPlaceholderText();
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
		updateEditorPlaceholderText();
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
		updateEditorPlaceholderText();
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
		updateEditorPlaceholderText();
		dataChanged();
	}
	QDateTime DateTimeEdit::getDateTime() const
	{
		if (m_editor)
			return m_editor->dateTime();
		return m_dateTime;
	}


	bool DateTimeEdit::setData(const QVariant& data)
	{
		if (data.isValid() && data.canConvert<QDateTime>())
		{
			m_dateTime = data.toDateTime();
			updateEditorPlaceholderText();
			dataChanged();
			return true;
		}
		return false;
	}
	void DateTimeEdit::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			m_dateTime = m_editor->dateTime();
			updateEditorPlaceholderText();
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
			IgnoreSignalsContext context(this);
			m_editor->setDateTime(m_dateTime);
		}
	}


	QWidget* DateTimeEdit::createEditorWidget(QWidget* parent)
	{
		if (m_editor)
			return m_editor;
		IgnoreSignalsContext context(this);
		m_editor = new QDateTimeEdit(parent);
		m_editor->setDisplayFormat(s_format);
		m_editor->setDateTime(m_dateTime);

		return m_editor;
	}

	QString DateTimeEdit::getToolTip() const
	{
		return m_dateTime.toString(s_format);
	}
	void DateTimeEdit::editorWidgetDestroyed()
	{
		m_editor = nullptr;
	}
	void DateTimeEdit::updateIcon() const
	{
		setEditorPlaceholderIcon(IconManager::getIcon(s_dateIcon));
	}
	void DateTimeEdit::onDateTimeChanged(const QDateTime& newDateTime)
	{
		if (doIgnoreSignals())
			return;
		m_dateTime = newDateTime;
		dataChanged();
	}
	void DateTimeEdit::updateEditorPlaceholderText() const
	{
		setEditorPlaceholderText(m_dateTime.toString(s_format));
	}
	std::shared_ptr<ClipboardData> DateTimeEdit::createClipboadData() const
	{
		std::shared_ptr<DateTimeClipboardData> data = std::make_shared<DateTimeClipboardData>();
		if (hasCopyPolicy(CopyPastePolicy::Date))
			data->setDate(getDate());
		if (hasCopyPolicy(CopyPastePolicy::Time))
			data->setTime(getTime());
		return data;
	}
	bool DateTimeEdit::onPaste(std::shared_ptr<ClipboardData> pasteData)
	{
		std::shared_ptr<DateTimeClipboardData> variantData = std::dynamic_pointer_cast<DateTimeClipboardData>(pasteData);
		if (variantData)
		{
			if (hasPastePolicy(CopyPastePolicy::Date) && variantData->hasDate())
			{
				setDate(variantData->getDate());
			}
			if (hasPastePolicy(CopyPastePolicy::Time) && variantData->hasTime())
			{
				setTime(variantData->getTime());
			}
			return true;
		}		
		return false;
	}
}