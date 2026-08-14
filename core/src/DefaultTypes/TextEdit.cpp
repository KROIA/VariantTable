#include "DefaultTypes/TextEdit.h"
#include "ClipboardData/QVariantClipboardData.h"
#include "IconManager.h"

#include <QTextEdit>
#include <QApplication>
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace VariantTable
{
	// TextEditCellWidget ----------------------------------------------------
	TextEditCellWidget::TextEditCellWidget(QWidget* parent)
		: CellWidgetBase(parent)
		, m_textEdit(new QTextEdit(this))
	{
		auto* l = new QHBoxLayout(this);
		l->setContentsMargins(0, 0, 0, 0);
		l->addWidget(m_textEdit);
		setFocusProxy(m_textEdit);
	}
	void TextEditCellWidget::setData(const QVariant& data)
	{
		m_textEdit->setText(data.toString());
	}
	QVariant TextEditCellWidget::getData() const
	{
		return m_textEdit->toPlainText();
	}
	void TextEditCellWidget::onAlignmentChanged(Qt::Alignment alignment)
	{
		m_textEdit->setAlignment(alignment);
		if (layout())
			layout()->setAlignment(m_textEdit, alignment);
	}

	// TextEdit --------------------------------------------------------------
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
			m_editor->textEdit()->setText(text);

		updateEditorPlaceholderText();
		dataChanged();
	}
	QString TextEdit::getText() const
	{
		if (m_editor)
			return m_editor->textEdit()->toPlainText();
		return m_text;
	}


	bool TextEdit::setData(const QVariant& data)
	{
		if (data.isValid() && data.userType() == QMetaType::QString)
		{
			m_text = data.toString();
			if (m_editor)
				m_editor->textEdit()->setText(m_text);
			updateEditorPlaceholderText();
			dataChanged();
			return true;
		}
		return false;
	}
	void TextEdit::setData(CellWidgetBase* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			m_text = m_editor->textEdit()->toPlainText();
			updateEditorPlaceholderText();
		}
	}
	QVariant TextEdit::getData() const
	{
		return QVariant(m_text);
	}
	void TextEdit::getData(CellWidgetBase* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			IgnoreSignalsContext context(this);
			m_editor->textEdit()->setText(m_text);
		}
	}


	CellWidgetBase* TextEdit::createEditorWidget(QWidget* parent)
	{
		if (m_editor)
			return m_editor;

		IgnoreSignalsContext context(this);
		m_editor = new TextEditCellWidget(parent);
		QTextEdit* te = m_editor->textEdit();
		te->setText(m_text);
		connect(te, &QTextEdit::textChanged, this, &TextEdit::onTextChanged);

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
			m_text = m_editor->textEdit()->toPlainText();
			dataChanged();
		}
		updateEditorPlaceholderText();
	}
}
