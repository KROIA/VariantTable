#include "DefaultTypes/TextEdit.h"
#include "ClipboardData/QVariantClipboardData.h"
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


	bool TextEdit::setData(const QVariant& data)
	{
		if (data.isValid() && data.type() == QVariant::String)
		{
			m_text = data.toString();
			if (m_editor)
				m_editor->setText(m_text);
			updateEditorPlaceholderText();
			dataChanged();
			return true;
		}
		return false;
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
			IgnoreSignalsContext context(this);
			m_editor->setText(m_text);
		}
	}


	QWidget* TextEdit::createEditorWidget(QWidget* parent)
	{
		if (m_editor)
			return m_editor;

		IgnoreSignalsContext context(this);
		m_editor = new QTextEdit(parent);
		m_editor->setText(m_text);
		connect(m_editor, &QTextEdit::textChanged, this, &TextEdit::onTextChanged);

		return m_editor;
	}

	QString TextEdit::getToolTip() const
	{
		return m_text;
	}
	void TextEdit::editorWidgetDestroyed()
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

	std::shared_ptr<ClipboardData> TextEdit::copyAction() const
	{
		std::shared_ptr<QVariantClipboardData> data = std::make_shared<QVariantClipboardData>();
		if(hasCopyPolicy(CopyPastePolicy::Text))
			data->setData(QVariant(m_text));
		return data;
	}
	bool TextEdit::pasteAction(std::shared_ptr<ClipboardData> pasteData)
	{
		auto variantData = std::dynamic_pointer_cast<QVariantClipboardData>(pasteData);
		if (!variantData)
			return false;

		if (hasPastePolicy(CopyPastePolicy::Text))
			return setData(variantData->getData());

		return true;
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