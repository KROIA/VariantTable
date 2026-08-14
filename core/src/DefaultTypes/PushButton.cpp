#include "DefaultTypes/PushButton.h"
#include "ClipboardData/QVariantClipboardData.h"
#include "IconManager.h"

#include <QPushButton>
#include <QApplication>
#include <QPainter>
#include <QHBoxLayout>

namespace VariantTable
{
	PushButtonCellWidget::PushButtonCellWidget(QWidget* parent)
		: CellWidgetBase(parent)
		, m_pushButton(new QPushButton(this))
	{
		auto* l = new QHBoxLayout(this);
		l->setContentsMargins(0, 0, 0, 0);
		l->addWidget(m_pushButton);
	}

	void PushButtonCellWidget::setData(const QVariant& data)
	{
		if (m_pushButton && data.isValid())
			m_pushButton->setText(data.toString());
	}

	QVariant PushButtonCellWidget::getData() const
	{
		if (m_pushButton)
			return QVariant(m_pushButton->text());
		return QVariant();
	}

	void PushButtonCellWidget::onAlignmentChanged(Qt::Alignment a)
	{
		if (m_pushButton && layout())
			layout()->setAlignment(m_pushButton, a);
	}


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
		if(data.isValid() && data.userType() == QMetaType::QString)
		{
			m_text = data.toString();
			setEditorPlaceholderText(m_text);
			dataChanged();
			return true;
		}
		return false;
	}
	void PushButton::setData(CellWidgetBase* editor)
	{
		VT_UNUSED(editor);
		if (m_editor && m_editor->pushButton())
		{
			m_text = m_editor->pushButton()->text();
			setEditorPlaceholderText(m_text);
			dataChanged();
		}
	}
	QVariant PushButton::getData() const
	{
		return QVariant(m_text);
	}
	void PushButton::getData(CellWidgetBase* editor)
	{
		VT_UNUSED(editor);
		if (m_editor && m_editor->pushButton())
		{
			m_editor->pushButton()->setText(m_text);
		}
	}


	CellWidgetBase* PushButton::createEditorWidget(QWidget* parent)
	{
		if (m_editor)
			return m_editor;
		m_editor = new PushButtonCellWidget(parent);
		m_editor->pushButton()->setText(m_text);

		connect(m_editor->pushButton(), &QPushButton::clicked, this, &PushButton::onButtonClickedInternal);

		return m_editor;
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
