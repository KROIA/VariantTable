#include "DefaultTypes/DateEdit.h"
#include "IconManager.h"

#include <QDateEdit>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{
	QString DateEdit::s_format = "dd.MM.yyyy";
	QString DateEdit::s_dateIcon = "calendar.png";


	DateEdit::DateEdit()
		: CellDataBase()
	{
		updateIcon();
	}
	DateEdit::DateEdit(const DateEdit& other)
		: CellDataBase(other)
	{

	}
	DateEdit::DateEdit(const QDate& date)
		: CellDataBase()
		, m_date(date)
	{
		updateIcon();
		updateText();
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
		updateText();
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
		updateText();
	}
	void DateEdit::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
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
		if (m_editor)
		{
			m_editor->setDate(m_date);
		}
	}



	QWidget* DateEdit::createEditorWidget(QWidget* parent) const
	{
		if (m_editor)
			return m_editor;

		m_editor = new QDateEdit(parent);
		m_editor->setDisplayFormat(s_format);
		m_editor->setDate(m_date);

		return m_editor;
	}

	QString DateEdit::getToolTip() const
	{
		return m_date.toString(s_format);
	}
	void DateEdit::editorWidgetDestroyed() const
	{
		m_editor = nullptr;
	}
	void DateEdit::updateIcon()
	{
		setEditorPlaceholderIcon(IconManager::getIcon(s_dateIcon));
	}
	void DateEdit::updateText()
	{
		setEditorPlaceholderText(m_date.toString(s_format));
	}
}