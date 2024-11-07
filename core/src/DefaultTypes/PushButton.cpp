#include "DefaultTypes/PushButton.h"
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


	void PushButton::setData(const QVariant& data)
	{
		VT_UNUSED(data);
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


	QWidget* PushButton::createEditorWidget(QWidget* parent) const
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
	void PushButton::editorWidgetDestroyed() const
	{
		m_editor = nullptr;
	}

	void PushButton::updateIcon()
	{
		setEditorPlaceholderIcon(IconManager::getIcon(s_pushButtonIcon));
	}

	void PushButton::onButtonClickedInternal()
	{
		emit clicked();
	}
}