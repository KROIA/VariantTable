#include "DefaultTypes/CheckBox.h"
#include "ClipboardData/CheckBoxClipboardData.h"
#include "IconManager.h"

#include <QCheckBox>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{
	QString CheckBox::s_checkedIcon = "checkBox-checked.png";
	QString CheckBox::s_uncheckedIcon = "checkBox-unchecked.png";

	CheckBox::CheckBox()
		: CellDataBase()
		, m_text("QCheckBox")
		, m_value(false)
	{
		updateIcon();
	}
	CheckBox::CheckBox(const CheckBox& other)
		: CellDataBase(other)
		, m_text(other.m_text)
		, m_value(other.m_value)
	{
		updateIcon();
	}
	CheckBox::CheckBox(const QString& text, bool value)
		: CellDataBase()
		, m_text(text)
		, m_value(value)
	{
		updateIcon();
	}


	void CheckBox::setText(const QString& text)
	{
		m_text = text;
		setEditorPlaceholderText(m_text);
		dataChanged();
	}
	const QString& CheckBox::getText() const
	{
		return m_text;
	}
	void CheckBox::setChecked(bool value)
	{
		if (m_editor)
			m_editor->setChecked(value);
		else
		{
			updateIcon();
		}
		m_value = value;
		dataChanged();
	}
	bool CheckBox::isChecked() const
	{
		if (m_editor)
			return m_editor->isChecked();
		return m_value;
	}

	bool CheckBox::setData(const QVariant& data)
	{
		if (data.isValid() && data.canConvert<bool>())
		{
			m_value = data.toBool();
			dataChanged();
			return true;
		}
		return false;
	}
	void CheckBox::setData(QWidget* editor) 
	{
		VT_UNUSED(editor);
		//QCheckBox* checkBox = qobject_cast<QCheckBox*>(editor);
		if (m_editor)
		{
			if (m_value != m_editor->isChecked())
			{
				m_value = m_editor->isChecked();
				updateIcon();
			}
		}
	}
	QVariant CheckBox::getData() const 
	{
		return QVariant(m_value);
	}
	void CheckBox::getData(QWidget* editor) 
	{
		VT_UNUSED(editor);
		//QCheckBox* checkBox = qobject_cast<QCheckBox*>(editor);
		if (m_editor)
		{
			IgnoreSignalsContext context(this);
			m_editor->setText(m_text);
			m_editor->setChecked(m_value);
			//checkBox->setMinimumSize(QSize(250, 100));
		}
	}

	QWidget* CheckBox::createEditorWidget(QWidget* parent)
	{
		if (m_editor)
			return m_editor->parentWidget();
		IgnoreSignalsContext context(this);
		QWidget* editor = new QWidget(parent);
		// Add Layout
		QVBoxLayout* layout = new QVBoxLayout(editor);
		layout->setContentsMargins(5, 5, 5, 5);
		editor->setLayout(layout);
		m_editor = new QCheckBox(editor);
		m_editor->setText(m_text);
		m_editor->setChecked(m_value);
		connect(m_editor, &QCheckBox::stateChanged, this, &CheckBox::onStateChanged);
		layout->addWidget(m_editor);

		return editor;
	}

	QString CheckBox::getToolTip() const
	{
		return (m_value?"[X] ":"[ ] ") + m_text;
	}
	void CheckBox::editorWidgetDestroyed()
	{
		m_editor = nullptr;
	}
	void CheckBox::updateIcon() const
	{
		if (m_value)
			setEditorPlaceholderIcon(IconManager::getIcon(s_checkedIcon));
		else
			setEditorPlaceholderIcon(IconManager::getIcon(s_uncheckedIcon));
	}
	std::shared_ptr<ClipboardData> CheckBox::createClipboadData() const
	{
		std::shared_ptr< CheckBoxClipboardData> data = std::make_shared<CheckBoxClipboardData>();
		if(hasCopyPolicy(CopyPastePolicy::Text))
			data->setText(m_text);
		if (hasCopyPolicy(CopyPastePolicy::CheckBoxState))
			data->setCheckedState(m_value);
		return data;
	}
	bool CheckBox::onPaste(std::shared_ptr<ClipboardData> pasteData)
	{
		auto checkBoxData = std::dynamic_pointer_cast<CheckBoxClipboardData>(pasteData);
		if (checkBoxData)
		{
			if (checkBoxData->hasText() && hasPastePolicy(CopyPastePolicy::Text))
				setText(checkBoxData->getText());
			if (checkBoxData->hasCheckedState() && hasPastePolicy(CopyPastePolicy::CheckBoxState))
				setChecked(checkBoxData->getCheckedState());
			return true;
		}
		return false;
	}
	void CheckBox::onStateChanged(int state)
	{
		if (doIgnoreSignals())
			return;
		m_value = (state == Qt::Checked);
		dataChanged();
	}
}