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
		updateEditorPlaceholderText();
	}


	void TextEdit::setText(const QString& text)
	{
		m_text = text;
		if (m_editor)
			m_editor->setText(text);
		
		updateEditorPlaceholderText();
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
		updateEditorPlaceholderText();
		dataChanged();
	}
	void TextEdit::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			m_text = m_editor->toPlainText();
			updateEditorPlaceholderText();
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
		connect(m_editor, &QTextEdit::textChanged, this, &TextEdit::onTextChanged);

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
	void TextEdit::updateIcon() const
	{
		setEditorPlaceholderIcon(IconManager::getIcon(s_textEditIcon));
	}
	void TextEdit::updateEditorPlaceholderText() const
	{
		setEditorPlaceholderText(m_text.split("\n").first());
	}

	void TextEdit::onTextChanged()
	{
		if (doIgnoreSignals())
			return;
		if (m_editor)
		{
			m_text = m_editor->toPlainText();
			dataChanged();
		}
		updateEditorPlaceholderText();
	}
}