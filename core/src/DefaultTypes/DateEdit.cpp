#include "DefaultTypes/DateEdit.h"
#include "ClipboardData/DateTimeClipboardData.h"
#include "IconManager.h"

#include <QDateEdit>
#include <QApplication>
#include <QPainter>
#include <QHBoxLayout>

namespace VariantTable
{
	DateEditCellWidget::DateEditCellWidget(QWidget* parent)
		: CellWidgetBase(parent)
		, m_dateEdit(new QDateEdit(this))
	{
		auto* l = new QHBoxLayout(this);
		l->setContentsMargins(0, 0, 0, 0);
		l->addWidget(m_dateEdit);
	}
	void DateEditCellWidget::setData(const QVariant& data)
	{
		if (data.isValid() && data.canConvert<QDate>())
			m_dateEdit->setDate(data.toDate());
	}
	QVariant DateEditCellWidget::getData() const
	{
		return QVariant(m_dateEdit->date());
	}
	void DateEditCellWidget::onAlignmentChanged(Qt::Alignment a)
	{
		m_dateEdit->setAlignment(a);
	}


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
		updateEditorPlaceholderText();
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
			m_editor->dateEdit()->setDate(m_date);
		updateEditorPlaceholderText();
		dataChanged();
	}
	QDate DateEdit::getDate() const
	{
		if (m_editor)
			return m_editor->dateEdit()->date();
		return m_date;
	}


	bool DateEdit::setData(const QVariant& data)
	{
		if (data.isValid() && data.canConvert<QDate>())
		{
			m_date = data.toDate();
			updateEditorPlaceholderText();
			dataChanged();
			return true;
		}
		return false;
	}
	void DateEdit::setData(CellWidgetBase* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			m_date = m_editor->dateEdit()->date();
		}
	}
	QVariant DateEdit::getData() const
	{
		return QVariant(m_date);
	}
	void DateEdit::getData(CellWidgetBase* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			IgnoreSignalsContext context(this);
			m_editor->dateEdit()->setDate(m_date);
		}
	}



	CellWidgetBase* DateEdit::createEditorWidget(QWidget* parent)
	{
		if (m_editor)
			return m_editor;

		IgnoreSignalsContext context(this);
		m_editor = new DateEditCellWidget(parent);
		QDateEdit* inner = m_editor->dateEdit();
		inner->setDisplayFormat(s_format);
		inner->setDate(m_date);
		connect(inner, &QDateEdit::dateChanged, this, &DateEdit::onDateChanged);

		return m_editor;
	}

	QString DateEdit::getToolTip() const
	{
		return m_date.toString(s_format);
	}
	void DateEdit::editorWidgetDestroyed()
	{
		m_editor = nullptr;
	}
	void DateEdit::updateIcon() const
	{
		setEditorPlaceholderIcon(IconManager::getIcon(s_dateIcon));
	}
	void DateEdit::updateEditorPlaceholderText() const
	{
		setEditorPlaceholderText(m_date.toString(s_format));
	}

	std::shared_ptr<ClipboardData> DateEdit::copyAction() const
	{
		std::shared_ptr<DateTimeClipboardData> data = std::make_shared<DateTimeClipboardData>();
		if (hasCopyPolicy(CopyPastePolicy::Date))
			data->setDate(getDate());
		//if (hasCopyPolicy(CopyPastePolicy::Time))
		//	data->setTime(getTime());
		return data;
	}
	bool DateEdit::pasteAction(std::shared_ptr<ClipboardData> pasteData)
	{
		std::shared_ptr<DateTimeClipboardData> variantData = std::dynamic_pointer_cast<DateTimeClipboardData>(pasteData);
		if (variantData)
		{
			if (hasPastePolicy(CopyPastePolicy::Date) && variantData->hasDate())
			{
				setDate(variantData->getDate());
			}
			//if (hasPastePolicy(CopyPastePolicy::Time) && variantData->hasTime())
			//{
			//	setTime(variantData->getTime());
			//}
			return true;
		}
		return false;
	}

	void DateEdit::onDateChanged(const QDate& newDate)
	{
		if (doIgnoreSignals())
			return;
		m_date = newDate;
		dataChanged();
	}
}
