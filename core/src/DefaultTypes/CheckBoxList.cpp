#include "DefaultTypes/CheckBoxList.h"
#include "ClipboardData/CheckBoxListClipboardData.h"
#include "IconManager.h"
#include <QCheckBox>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{
	QString CheckBoxList::s_checkedIcon = "checkBoxList-checked.png";

	CheckBoxList::CheckBoxList()
		: CellDataBase()
		, m_options({
			{"Option 1",QVariant()},
			{"Option 2",QVariant()},
			{"Option 3",QVariant()} })
	{
		updateIcon();
		updateEditorPlaceholderText();
	}
	CheckBoxList::CheckBoxList(const CheckBoxList& other)
		: CellDataBase(other)
		, m_options(other.m_options)
		, m_selectedIndexes(other.m_selectedIndexes)
	{

	}
	CheckBoxList::CheckBoxList(const QStringList& options)
		: CellDataBase()
		, m_options()
	{
		for (const auto& option : options)
		{
			m_options.push_back({ option, QVariant() });
		}
		updateIcon();
		updateEditorPlaceholderText();
	}
	CheckBoxList::CheckBoxList(const OptionsType& options)
		: CellDataBase()
		, m_options(options)
	{
		updateIcon();
		updateEditorPlaceholderText();
	}

	void CheckBoxList::setOptions(const QStringList& options)
	{
		m_options.clear();
		for (const auto& option : options)
		{
			m_options.push_back({ option, QVariant() });
		}
		m_selectedIndexes.clear();
		updateEditorPlaceholderText();
		dataChanged();
	}
	void CheckBoxList::setOptions(const QVector<QPair<QString, QVariant>>& data)
	{
		m_options = data;
		m_selectedIndexes.clear();
		updateEditorPlaceholderText();
		dataChanged();
	}
	const QVector<QPair<QString, QVariant>>& CheckBoxList::getOptions() const
	{
		return m_options;
	}
	void CheckBoxList::setCheckedIndexes(const QVector<int>& indexes)
	{
		IgnoreSignalsContext context(this);
		m_selectedIndexes = indexes;
		if (m_editorWidget)
		{
			for (int i = 0; i < m_checkBoxes.size(); ++i)
			{
				m_checkBoxes[i]->setChecked(m_selectedIndexes.contains(i));
			}
		}
		updateEditorPlaceholderText();
		dataChanged();
	}
	QVector<int> CheckBoxList::getCheckedIndexes() const
	{
		if (m_editorWidget)
		{
			QVector<int> selectedIndexes;
			selectedIndexes.reserve(m_checkBoxes.size());
			for (int i = 0; i < m_checkBoxes.size(); ++i)
			{
				if (m_checkBoxes[i]->isChecked())
				{
					selectedIndexes.push_back(i);
				}
			}
			return selectedIndexes;
		}
		return m_selectedIndexes;
	}


	bool CheckBoxList::setData(const QVariant& data)
	{
		if(data.isValid() && data.canConvert<OptionsType>())
		{
			m_options = data.value<OptionsType>();
			m_selectedIndexes.clear();
			updateEditorPlaceholderText();
			dataChanged();
			return true;
		}
		return false;
	}
	void CheckBoxList::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
		//QCheckBoxList* CheckBoxList = qobject_cast<QCheckBoxList*>(editor);
		if (m_editorWidget)
		{
			m_selectedIndexes.clear();
			m_selectedIndexes.reserve(m_checkBoxes.size());
			for (int i = 0; i < m_checkBoxes.size(); ++i)
			{
				if (m_checkBoxes[i]->isChecked())
				{
					m_selectedIndexes.push_back(i);
				}
			}
		}
		updateEditorPlaceholderText();
	}
	QVariant CheckBoxList::getData() const
	{
		QVariant v = QVariant::fromValue(m_options);
		QVector<QPair<QString, QVariant>> tmp = v.value<OptionsType>();
		return v;
	}
	void CheckBoxList::getData(QWidget* editor)
	{
		VT_UNUSED(editor);
		//QCheckBoxList* CheckBoxList = qobject_cast<QCheckBoxList*>(editor);
		if (m_editorWidget)
		{
			IgnoreSignalsContext context(this);
			buildEditorWidget();
			/*int maxIndex = std::min(m_checkBoxes.size(), m_options.size());
			if (m_checkBoxes.size() != m_options.size())
			{
				qWarning("CheckBoxList::getData: The number of options and checkboxes do not match");
			}
			for (int i = 0; i < maxIndex; ++i)
			{
				m_checkBoxes[i]->setText(m_options[i].first);
				m_checkBoxes[i]->setChecked(false);
			}
			for (int i : m_selectedIndexes)
			{
				if (i >= 0 && i < m_checkBoxes.size())
					m_checkBoxes[i]->setChecked(true);
			}*/
		}
	}


	/*QSize CheckBoxList::getSizeHint(const QStyleOptionViewItem& option) const
	{
		VT_UNUSED(option);
		QSize size;
		for (const auto& checkbox : m_checkBoxes)
		{
			size = size.expandedTo(checkbox->sizeHint());
		}
		return size;
	}*/
	QWidget* CheckBoxList::createEditorWidget(QWidget* parent)
	{
		if (m_editorWidget)
			return m_editorWidget;
		IgnoreSignalsContext context(this);

		m_editorWidget = new QWidget(parent);
		//buildEditorWidget();

		// Make widget adjusts its size to fit the minimum hight
		//m_editorWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
		//m_editorWidget->layout()->setSizeConstraint(QLayout::SetMinimumSize);
		
		
		return m_editorWidget;
	}
	void CheckBoxList::buildEditorWidget()
	{
		IgnoreSignalsContext context(this);
		for(auto checkbox : m_checkBoxes)
		{
			delete checkbox;
		}
		m_checkBoxes.clear();
		// Add Layout
		QLayout* layout = m_editorWidget->layout();
		if (!layout)
		{
			layout = new QVBoxLayout(m_editorWidget);
			layout->setContentsMargins(5, 5, 5, 5);
			m_editorWidget->setLayout(layout);
		}

		// Add Radio Buttons
		for (const auto& option : m_options)
		{
			QCheckBox* button = new QCheckBox(option.first, m_editorWidget);
			connect(button, &QCheckBox::stateChanged, this, &CheckBoxList::onStateChanged);
			layout->addWidget(button);
			m_checkBoxes.push_back(button);
		}

		// Set data
		int maxIndex = std::min(m_checkBoxes.size(), m_options.size());
		if (m_checkBoxes.size() != m_options.size())
		{
			qWarning("CheckBoxList::getData: The number of options and buttons do not match");
		}
		for (int i = 0; i < maxIndex; ++i)
		{
			m_checkBoxes[i]->setText(m_options[i].first);
			m_checkBoxes[i]->setChecked(false);
		}
		for (int i : m_selectedIndexes)
		{
			if (i >= 0 && i < m_checkBoxes.size())
				m_checkBoxes[i]->setChecked(true);
		}

		
	}
	
	QString CheckBoxList::getToolTip() const
	{
		QString text;
		for (int i=0; i< m_options.size(); ++i)
		{
			text += (m_selectedIndexes.contains(i) ? "[X] " : "[ ] ") + m_options[i].first + "\n";
		}
		text.chop(1); // Remove the last newline
		return text;
	}
	void CheckBoxList::editorWidgetDestroyed()
	{
		m_editorWidget = nullptr;
		m_checkBoxes.clear();
	}
	void CheckBoxList::updateIcon() const
	{
		setEditorPlaceholderIcon(IconManager::getIcon(s_checkedIcon));
	}
	std::shared_ptr<ClipboardData> CheckBoxList::copyAction() const
	{ 
		std::shared_ptr< CheckBoxListClipboardData> data = std::make_shared<CheckBoxListClipboardData>();
		if (hasCopyPolicy(CopyPastePolicy::Text))
		{
			QVector<QString> text;
			for (const auto& option : m_options)
			{
				text.push_back(option.first);
			}
			data->setText(text);
		}
		if (hasCopyPolicy(CopyPastePolicy::CheckBoxState))
		{
			QVector<bool> checked;
			for (int i = 0; i < m_options.size(); ++i)
			{
				checked.push_back(m_selectedIndexes.contains(i));
			}
			data->setCheckedState(checked);
		}
		return data;
	}
	bool CheckBoxList::pasteAction(std::shared_ptr<ClipboardData> pasteData)
	{
		auto checkBoxData = std::dynamic_pointer_cast<CheckBoxListClipboardData>(pasteData);
		if (checkBoxData)
		{
			if (checkBoxData->hasText() && hasPastePolicy(CopyPastePolicy::Text))
			{
				QVector<QPair<QString, QVariant>> options;
				for (const auto& text : checkBoxData->getText())
				{
					options.push_back({ text, QVariant() });
				}
				setOptions(options);
			}
			if (checkBoxData->hasCheckedState() && hasPastePolicy(CopyPastePolicy::CheckBoxState))
			{
				QVector<int> selectedIndexes;
				const auto& checkedState = checkBoxData->getCheckedState();
				for (int i = 0; i < checkedState.size(); ++i)
				{
					if (checkedState[i])
						selectedIndexes.push_back(i);
				}
				setCheckedIndexes(selectedIndexes);
			}
			return true;
		}
		return false;
	}
	void CheckBoxList::onStateChanged(int state)
	{
		VT_UNUSED(state);
		if (doIgnoreSignals())
			return;
		m_selectedIndexes.clear();
		for (int i = 0; i < m_checkBoxes.size(); ++i)
		{
			if (m_checkBoxes[i]->isChecked())
			{
				m_selectedIndexes.push_back(i);
			}
		}
		dataChanged();
	}
	void CheckBoxList::updateEditorPlaceholderText() const
	{
		QString text;
		for (int i : m_selectedIndexes)
		{
			if (i >= 0 && i < m_options.size())
				text += m_options[i].first + ", ";
		}
		if (!text.isEmpty())
			text.chop(2); // Remove the last comma and space
		//else
		//	text = "Nothing selected";
		setEditorPlaceholderText(text);
	}
}
