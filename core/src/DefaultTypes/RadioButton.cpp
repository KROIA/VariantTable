#include "DefaultTypes/RadioButton.h"
#include "IconManager.h"

#include <QRadioButton>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{

	QString RadioButton::s_radioIcon = "radioButton.png";


	RadioButton::RadioButton()
		: CellDataBase()
		, m_options({"Option 1", "Option 2", "Option 3"})
	{
		updateIcon();
	}
	RadioButton::RadioButton(const RadioButton& other)
		: CellDataBase(other)
		, m_options(other.m_options)
	{

	}
	RadioButton::RadioButton(const QStringList& options)
		: CellDataBase()
		, m_options(options)
	{
		updateIcon();
		updateText();
	}


	void RadioButton::setOptions(const QStringList& options)
	{
		m_options = options;
		m_selectedIndex = -1;
		updateText();
	}
	const QStringList& RadioButton::getOptions() const
	{
		return m_options;
	}

	void RadioButton::setSelectedIndex(int index)
	{
		m_selectedIndex = index;
		updateText();
		if (m_editorWidget)
		{
			if (index >= 0 && index < m_editorButtons.size())
			{
				for (int i = 0; i < m_editorButtons.size(); ++i)
				{
					if (i == index)
						m_editorButtons[i]->setChecked(true);
					else
						m_editorButtons[i]->setChecked(false);
				}
			}
		}
		dataChanged();
	}

	int RadioButton::getSelectedIndex() const
	{
		if (m_editorWidget)
		{
			for (int i = 0; i < m_editorButtons.size(); ++i)
			{
				if (m_editorButtons[i]->isChecked())
				{
					return i;
				}
			}
		}
		return m_selectedIndex;
	}



	void RadioButton::setData(const QVariant& data)
	{
		m_options = data.toStringList();
		dataChanged();
	}
	void RadioButton::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
		if (m_editorWidget)
		{
			m_selectedIndex = -1;
			for (int i = 0; i < m_editorButtons.size(); ++i)
			{
				if (m_editorButtons[i]->isChecked())
				{
					m_selectedIndex = i;
					break;
				}
			}
		}
		updateText();
	}
	QVariant RadioButton::getData() const
	{
		return QVariant(m_options);
	}
	void RadioButton::getData(QWidget* editor)
	{
		VT_UNUSED(editor);
		if (m_editorWidget)
		{
			int maxIndex = std::min(m_editorButtons.size(), m_options.size());
			if (m_editorButtons.size() != m_options.size())
			{
				qWarning("RadioButton::getData: The number of options and buttons do not match");
			}
			for (int i = 0; i < maxIndex; ++i)
			{
				m_editorButtons[i]->setText(m_options[i]);
				if (i == m_selectedIndex)
					m_editorButtons[i]->setChecked(true);
				else
					m_editorButtons[i]->setChecked(false);
			}
		}
	}


	QWidget* RadioButton::createEditorWidget(QWidget* parent) const
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
			QRadioButton* button = new QRadioButton(option, m_editorWidget);
			layout->addWidget(button);
			m_editorButtons.push_back(button);
		}

		// Set data
		int maxIndex = std::min(m_editorButtons.size(), m_options.size());
		if (m_editorButtons.size() != m_options.size())
		{
			qWarning("RadioButton::getData: The number of options and buttons do not match");
		}
		for (int i = 0; i < maxIndex; ++i)
		{
			m_editorButtons[i]->setText(m_options[i]);
			if (i == m_selectedIndex)
				m_editorButtons[i]->setChecked(true);
			else
				m_editorButtons[i]->setChecked(false);
		}

	
		return m_editorWidget;
	}

	QString RadioButton::getToolTip() const
	{
		QString text;
		for (int i = 0; i < m_options.size(); ++i)
		{
			if (i == m_selectedIndex)
				text += "- " + m_options[i] + "\n";
			else
				text += "  " + m_options[i] + "\n";
		}
		text.chop(1); // Remove the last newline
		return text;
	}
	void RadioButton::editorWidgetDestroyed() const
	{
		m_editorWidget = nullptr;
		m_editorButtons.clear();
	}
	void RadioButton::updateIcon()
	{
		setEditorPlaceholderIcon(IconManager::getIcon(s_radioIcon));
	}
	void RadioButton::updateText()
	{
		QString text = "Nothing selected";
		if (m_selectedIndex >= 0 && m_selectedIndex < m_options.size())
			text = m_options[m_selectedIndex];
		setEditorPlaceholderText(text);
	}
}