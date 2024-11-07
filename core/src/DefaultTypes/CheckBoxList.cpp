#include "DefaultTypes/CheckBoxList.h"
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
		, m_options({ "Option 1", "Option 2", "Option 3" })
	{
		updateIcon();
		updateText();
	}
	CheckBoxList::CheckBoxList(const CheckBoxList& other)
		: CellDataBase(other)
		, m_options(other.m_options)
	{

	}
	CheckBoxList::CheckBoxList(const QStringList& options)
		: CellDataBase()
		, m_options(options)
	{
		updateIcon();
		updateText();
	}


	void CheckBoxList::setOptions(const QStringList& options)
	{
		m_options = options;
		m_selectedIndexes.clear();
		updateText();
		dataChanged();
	}
	const QStringList& CheckBoxList::getOptions() const
	{
		return m_options;
	}
	void CheckBoxList::setCheckedIndexes(const QVector<int>& indexes)
	{
		m_selectedIndexes = indexes;
		if (m_editorWidget)
		{
			for (int i = 0; i < m_checkBoxes.size(); ++i)
			{
				m_checkBoxes[i]->setChecked(m_selectedIndexes.contains(i));
			}
		}
		updateText();
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


	void CheckBoxList::setData(const QVariant& data)
	{
		m_options = data.toStringList();
		updateText();
		dataChanged();
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
		updateText();
	}
	QVariant CheckBoxList::getData() const
	{
		return QVariant(m_options);
	}
	void CheckBoxList::getData(QWidget* editor)
	{
		VT_UNUSED(editor);
		//QCheckBoxList* CheckBoxList = qobject_cast<QCheckBoxList*>(editor);
		if (m_editorWidget)
		{
			int maxIndex = std::min(m_checkBoxes.size(), m_options.size());
			if (m_checkBoxes.size() != m_options.size())
			{
				qWarning("CheckBoxList::getData: The number of options and checkboxes do not match");
			}
			for (int i = 0; i < maxIndex; ++i)
			{
				m_checkBoxes[i]->setText(m_options[i]);
				m_checkBoxes[i]->setChecked(false);
			}
			for (int i : m_selectedIndexes)
			{
				if (i >= 0 && i < m_checkBoxes.size())
					m_checkBoxes[i]->setChecked(true);
			}
		}
	}



	QWidget* CheckBoxList::createEditorWidget(QWidget* parent) const
	{
		if (m_editorWidget)
			return m_editorWidget;

		m_editorWidget = new QWidget(parent);

		// Add Layout
		QVBoxLayout* layout = new QVBoxLayout(m_editorWidget);
		layout->setContentsMargins(5, 5, 5, 5);
		m_editorWidget->setLayout(layout);

		// Add Radio Buttons
		for (const QString& option : m_options)
		{
			QCheckBox* button = new QCheckBox(option, m_editorWidget);
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
			m_checkBoxes[i]->setText(m_options[i]);
			m_checkBoxes[i]->setChecked(false);
		}
		for (int i : m_selectedIndexes)
		{
			if (i >= 0 && i < m_checkBoxes.size())
				m_checkBoxes[i]->setChecked(true);
		}
		return m_editorWidget;
	}
	
	QString CheckBoxList::getToolTip() const
	{
		QString text;
		for (int i=0; i< m_options.size(); ++i)
		{
			text += (m_selectedIndexes.contains(i) ? "[X] " : "[  ] ") + m_options[i] + "\n";
		}
		text.chop(1); // Remove the last newline
		return text;
	}
	void CheckBoxList::editorWidgetDestroyed() const
	{
		m_editorWidget = nullptr;
		m_checkBoxes.clear();
	}
	void CheckBoxList::updateIcon()
	{
		setEditorPlaceholderIcon(IconManager::getIcon(s_checkedIcon));
	}
	void CheckBoxList::updateText()
	{
		QString text;
		for (int i : m_selectedIndexes)
		{
			if (i >= 0 && i < m_options.size())
				text += m_options[i] + ", ";
		}
		if (!text.isEmpty())
			text.chop(2); // Remove the last comma and space
		//else
		//	text = "Nothing selected";
		setEditorPlaceholderText(text);
	}
}