#include "DefaultTypes/LineEdit.h"
#include "IconManager.h"

#include <QLineEdit>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{
	QString LineEdit::s_lineEditIcon = "lineEdit.png";

	LineEdit::LineEdit()
		: CellDataBase()
		, m_text("")
	{
		updateIcon();
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
		updateIcon();
		setEditorPlaceholderText(m_text);
	}


	void LineEdit::setText(const QString& text)
	{
		m_text = text;
		if (m_editor)
			m_editor->setText(text);
		setEditorPlaceholderText(m_text);
		dataChanged();
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
		setEditorPlaceholderText(m_text);
		dataChanged();
	}
	void LineEdit::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			m_text = m_editor->text();
			setEditorPlaceholderText(m_text);
		}
	}
	QVariant LineEdit::getData() const
	{
		return QVariant(m_text);
	}
	void LineEdit::getData(QWidget* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			m_editor->setText(m_text);
		}
	}

	QWidget* LineEdit::createEditorWidget(QWidget* parent) const
	{
		if (m_editor)
			return m_editor;

		m_editor = new QLineEdit(parent);
		m_editor->setText(m_text);

		return m_editor;
	}

	QString LineEdit::getToolTip() const
	{
		return m_text;
	}
	void LineEdit::updateIcon()
	{
		setEditorPlaceholderIcon(IconManager::getIcon(s_lineEditIcon));
	}
	void LineEdit::editorWidgetDestroyed() const
	{
		m_editor = nullptr;
	}
}