#include "DefaultTypes/TextEdit.h"
#include "IconManager.h"

#include <QTextEdit>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{

	QString TextEdit::s_textEditIcon = "lineEdit.png";


	TextEdit::TextEdit()
		: CellDataBase()
		, m_text("")
	{
		updateIcon();
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
		updateIcon();
		updateText();
	}


	void TextEdit::setText(const QString& text)
	{
		m_text = text;
		if (m_editor)
			m_editor->setText(text);
		
		updateText();
		dataChanged();
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
		if (m_editor)
			m_editor->setText(m_text);
		updateText();
		dataChanged();
	}
	void TextEdit::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			m_text = m_editor->toPlainText();
			updateText();
		}
	}
	QVariant TextEdit::getData() const
	{
		return QVariant(m_text);
	}
	void TextEdit::getData(QWidget* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			m_editor->setText(m_text);
		}
	}


	QWidget* TextEdit::createEditorWidget(QWidget* parent) const
	{
		if (m_editor)
			return m_editor;

		m_editor = new QTextEdit(parent);
		m_editor->setText(m_text);

		return m_editor;
	}

	QString TextEdit::getToolTip() const
	{
		return m_text;
	}
	void TextEdit::editorWidgetDestroyed() const
	{
		m_editor = nullptr;
	}
	void TextEdit::updateIcon()
	{
		setEditorPlaceholderIcon(IconManager::getIcon(s_textEditIcon));
	}
	void TextEdit::updateText()
	{
		setEditorPlaceholderText(m_text.split("\n").first());
	}
}