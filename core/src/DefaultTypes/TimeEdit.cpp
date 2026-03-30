#include "DefaultTypes/TimeEdit.h"
#include "ClipboardData/DateTimeClipboardData.h"
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
		updateEditorPlaceholderText();
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
		updateEditorPlaceholderText();
		dataChanged();
	}
	QTime TimeEdit::getTime() const
	{
		if (m_editor)
			return m_editor->time();
		return m_time;
	}


	bool TimeEdit::setData(const QVariant& data)
	{
		if (data.isValid() && data.canConvert<QTime>())
		{
			m_time = data.toTime();
			updateEditorPlaceholderText();
			dataChanged();
			return true;
		}
		return false;
	}
	void TimeEdit::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			m_time = m_editor->time();
			updateEditorPlaceholderText();
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
			IgnoreSignalsContext context(this);
			m_editor->setTime(m_time);
		}
	}


	QWidget* TimeEdit::createEditorWidget(QWidget* parent)
	{
		if (m_editor)
			return m_editor;

		IgnoreSignalsContext context(this);
		m_editor = new QTimeEdit(parent);
		m_editor->setDisplayFormat(s_format);
		m_editor->setTime(m_time);
		connect(m_editor, &QTimeEdit::timeChanged, this, &TimeEdit::onTimeChanged);

		return m_editor;
	}

	QString TimeEdit::getToolTip() const
	{
		return m_time.toString(s_format);
	}
	void TimeEdit::editorWidgetDestroyed()
	{
		m_editor = nullptr;
	}
	void TimeEdit::updateIcon() const
	{
		setEditorPlaceholderIcon(IconManager::getIcon(s_timeIcon));
	}
	void TimeEdit::updateEditorPlaceholderText() const
	{
		setEditorPlaceholderText(m_time.toString(s_format));
	}

	void TimeEdit::onTimeChanged(const QTime& newTime)
	{
		if (doIgnoreSignals())
			return;
		m_time = newTime;
		updateEditorPlaceholderText();
		dataChanged();
	}

	std::shared_ptr<ClipboardData> TimeEdit::copyAction() const
	{
		std::shared_ptr<DateTimeClipboardData> data = std::make_shared<DateTimeClipboardData>();
		//if (hasCopyPolicy(CopyPastePolicy::Date))
		//	data->setDate(getDate());
		if (hasCopyPolicy(CopyPastePolicy::Time))
			data->setTime(getTime());
		return data;
	}
	bool TimeEdit::pasteAction(std::shared_ptr<ClipboardData> pasteData)
	{
		std::shared_ptr<DateTimeClipboardData> variantData = std::dynamic_pointer_cast<DateTimeClipboardData>(pasteData);
		if (variantData)
		{
			//if (hasPastePolicy(CopyPastePolicy::Date) && variantData->hasDate())
			//{
			//	setDate(variantData->getDate());
			//}
			if (hasPastePolicy(CopyPastePolicy::Time) && variantData->hasTime())
			{
				setTime(variantData->getTime());
			}
			return true;
		}
		return false;
	}
}