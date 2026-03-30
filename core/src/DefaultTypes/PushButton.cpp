#include "DefaultTypes/PushButton.h"
#include "ClipboardData/QVariantClipboardData.h"
#include "IconManager.h"

#include <QPushButton>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{
	QString PushButton::s_pushButtonIcon = "pushButton.png";

	PushButton::PushButton()
		: CellDataBase()
		, m_text("QPushButton")
	{
		updateIcon();
	}
	PushButton::PushButton(const PushButton& other)
		: CellDataBase(other)
		, m_text(other.m_text)
	{

	}
	PushButton::PushButton(const QString& text)
		: CellDataBase()
		, m_text(text)
	{
		updateIcon();
		setEditorPlaceholderText(m_text);
	}


	void PushButton::setText(const QString& text)
	{
		m_text = text;
		setEditorPlaceholderText(m_text);
		dataChanged();
	}
	const QString& PushButton::getText() const
	{
		return m_text;
	}


	bool PushButton::setData(const QVariant& data)
	{
		if(data.isValid() && data.type() == QVariant::String)
		{
			m_text = data.toString();
			setEditorPlaceholderText(m_text);
			dataChanged();
			return true;
		}
		return false;
	}
	void PushButton::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			m_text = m_editor->text();
			setEditorPlaceholderText(m_text);
			dataChanged();
		}
	}
	QVariant PushButton::getData() const
	{
		return QVariant(m_text);
	}
	void PushButton::getData(QWidget* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			m_editor->setText(m_text);
		}
	}


	QWidget* PushButton::createEditorWidget(QWidget* parent)
	{
		if (m_editor)
			return m_editor->parentWidget();
		QWidget* editor = new QWidget(parent);
		// Add Layout
		QVBoxLayout* layout = new QVBoxLayout(editor);
		layout->setContentsMargins(5, 5, 5, 5);
		editor->setLayout(layout);
		m_editor = new QPushButton(editor);
		m_editor->setText(m_text);
		layout->addWidget(m_editor);

		connect(m_editor, &QPushButton::clicked, this, &PushButton::onButtonClickedInternal);

		return editor;
	}
	
	QString PushButton::getToolTip() const
	{
		return m_text;
	}
	void PushButton::editorWidgetDestroyed()
	{
		m_editor = nullptr;
	}

	void PushButton::updateIcon() const
	{
		setEditorPlaceholderIcon(IconManager::getIcon(s_pushButtonIcon));
	}

	std::shared_ptr<ClipboardData> PushButton::copyAction() const
	{
		std::shared_ptr<QVariantClipboardData> data = std::make_shared<QVariantClipboardData>();
		if(hasCopyPolicy(CopyPastePolicy::Text))
		{
			data->setData(getData());
		}
		return data;
	}
	bool PushButton::pasteAction(std::shared_ptr<ClipboardData> pasteData)
	{
		std::shared_ptr<QVariantClipboardData> variantData = std::dynamic_pointer_cast<QVariantClipboardData>(pasteData);
		if (variantData)
		{
			if(hasPastePolicy(CopyPastePolicy::Text))
			{
				return setData(variantData->getData());
			}
		}
		return false;
	}

	void PushButton::onButtonClickedInternal()
	{
		emit clicked();
	}
}