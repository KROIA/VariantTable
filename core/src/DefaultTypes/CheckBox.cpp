#include "DefaultTypes/CheckBox.h"
#include "ClipboardData/CheckBoxClipboardData.h"
#include "IconManager.h"
#include "QtCompat.h"

#include <QCheckBox>
#include <QApplication>
#include <QPainter>
#include <QHBoxLayout>

namespace VariantTable
{
	CheckBoxCellWidget::CheckBoxCellWidget(QWidget* parent)
		: CellWidgetBase(parent)
		, m_checkBox(nullptr)
	{
		auto* layout = new QHBoxLayout(this);
		layout->setContentsMargins(0, 0, 0, 0);
		m_checkBox = new QCheckBox(this);
		layout->addWidget(m_checkBox);
		layout->setAlignment(m_checkBox, getAlignment());
	}
	void CheckBoxCellWidget::setData(const QVariant& data)
	{
		if (data.isValid() && data.canConvert<bool>())
			m_checkBox->setChecked(data.toBool());
	}
	QVariant CheckBoxCellWidget::getData() const
	{
		return QVariant(m_checkBox->isChecked());
	}
	void CheckBoxCellWidget::onAlignmentChanged(Qt::Alignment alignment)
	{
		if (layout() && m_checkBox)
			layout()->setAlignment(m_checkBox, alignment);
	}


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
			m_editor->checkBox()->setChecked(value);
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
			return m_editor->checkBox()->isChecked();
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
	void CheckBox::setData(CellWidgetBase* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			QCheckBox* cb = m_editor->checkBox();
			if (m_value != cb->isChecked())
			{
				m_value = cb->isChecked();
				updateIcon();
			}
		}
	}
	QVariant CheckBox::getData() const
	{
		return QVariant(m_value);
	}
	void CheckBox::getData(CellWidgetBase* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			IgnoreSignalsContext context(this);
			QCheckBox* cb = m_editor->checkBox();
			cb->setText(m_text);
			cb->setChecked(m_value);
		}
	}

	CellWidgetBase* CheckBox::createEditorWidget(QWidget* parent)
	{
		if (m_editor)
			return m_editor;
		IgnoreSignalsContext context(this);
		m_editor = new CheckBoxCellWidget(parent);
		QCheckBox* cb = m_editor->checkBox();
		cb->setText(m_text);
		cb->setChecked(m_value);
		connectCheckBoxStateChanged(cb, this, &CheckBox::onStateChanged);
		return m_editor;
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
	std::shared_ptr<ClipboardData> CheckBox::copyAction() const
	{
		std::shared_ptr< CheckBoxClipboardData> data = std::make_shared<CheckBoxClipboardData>();
		if(hasCopyPolicy(CopyPastePolicy::Text))
			data->setText(m_text);
		if (hasCopyPolicy(CopyPastePolicy::CheckBoxState))
			data->setCheckedState(m_value);
		return data;
	}
	bool CheckBox::pasteAction(std::shared_ptr<ClipboardData> pasteData)
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
