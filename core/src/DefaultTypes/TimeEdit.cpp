#include "DefaultTypes/TimeEdit.h"
#include "IconManager.h"

#include <QTimeEdit>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{

	QString TimeEdit::s_format = "hh:mm:ss";
	QString TimeEdit::s_timeIcon = "clock.png";
	


	TimeEdit::TimeEdit()
		: CellDataBase()
	{
		updateIcon();
	}
	TimeEdit::TimeEdit(const TimeEdit& other)
		: CellDataBase(other)
	{

	}
	TimeEdit::TimeEdit(const QTime& time)
		: CellDataBase()
		, m_time(time)
	{
		updateIcon();
		updateText();
	}

	void TimeEdit::setFormat(const QString& format)
	{
		s_format = format;
	}
	const QString& TimeEdit::getFormat()
	{
		return s_format;
	}


	void TimeEdit::setTime(const QTime& time)
	{
		m_time = time;
		if (m_editor)
			m_editor->setTime(m_time);
		updateText();
	}
	QTime TimeEdit::getTime() const
	{
		if (m_editor)
			return m_editor->time();
		return m_time;
	}


	void TimeEdit::setData(const QVariant& data)
	{
		m_time = data.toTime();
		updateText();
	}
	void TimeEdit::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			m_time = m_editor->time();
			updateText();
		}
	}
	QVariant TimeEdit::getData() const
	{
		return QVariant(m_time);
	}
	void TimeEdit::getData(QWidget* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			m_editor->setTime(m_time);
		}
	}


	QWidget* TimeEdit::createEditorWidget(QWidget* parent) const
	{
		if (m_editor)
			return m_editor;

		m_editor = new QTimeEdit(parent);
		m_editor->setDisplayFormat(s_format);
		m_editor->setTime(m_time);

		return m_editor;
	}

	QString TimeEdit::getToolTip() const
	{
		return m_time.toString(s_format);
	}
	void TimeEdit::editorWidgetDestroyed() const
	{
		m_editor = nullptr;
	}
	void TimeEdit::updateIcon()
	{
		setEditorPlaceholderIcon(IconManager::getIcon(s_timeIcon));
	}
	void TimeEdit::updateText()
	{
		setEditorPlaceholderText(m_time.toString(s_format));
	}
}