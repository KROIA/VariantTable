#include "DefaultTypes/ComboBox.h"
#include "IconManager.h"

#include <QComboBox>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{
	QString ComboBox::s_comboBoxIcon = "comboBox.png";

	ComboBox::ComboBox()
		: CellDataBase()
		, m_options({ "Option 1", "Option 2", "Option 3" })
	{
		updateIcon();
		updateText();
	}
	ComboBox::ComboBox(const ComboBox& other)
		: CellDataBase(other)
		, m_options(other.m_options)
	{

	}
	ComboBox::ComboBox(const QStringList& options)
		: CellDataBase()
		, m_options(options)
	{
		updateIcon();
		updateText();
	}


	void ComboBox::setOptions(const QStringList& options)
	{
		m_options = options;
		m_selectedIndex = -1;
		updateText();
		dataChanged();
	}
	const QStringList& ComboBox::getOptions() const
	{
		return m_options;
	}
	void ComboBox::setCurrentIndex(int index)
	{
		if (m_combo)
		{
			m_combo->setCurrentIndex(index);
		}
		m_selectedIndex = index;
		dataChanged();
	}
	int ComboBox::getCurrentIndex() const
	{
		if(m_combo)
			return m_combo->currentIndex();
		return m_selectedIndex;
	}


	void ComboBox::setData(const QVariant& data)
	{
		m_options = data.toStringList();
		dataChanged();
	}
	void ComboBox::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
		//QComboBox* ComboBox = qobject_cast<QComboBox*>(editor);
		if (m_combo)
		{
			m_selectedIndex = m_combo->currentIndex();		
			updateText();
		}
	}
	QVariant ComboBox::getData() const
	{
		return QVariant(m_options);
	}

	void ComboBox::getData(QWidget* editor)
	{
		VT_UNUSED(editor);
		//QComboBox* ComboBox = qobject_cast<QComboBox*>(editor);
		if (m_combo)
		{
			m_combo->clear();
			m_combo->addItems(m_options);
			m_combo->setCurrentIndex(m_selectedIndex);
		}
	}

	QWidget* ComboBox::createEditorWidget(QWidget* parent) const
	{
		if (m_combo)
			return m_combo;
		
		m_combo = new QComboBox(parent);

		// Set options
		m_combo->addItems(m_options);
		m_combo->setCurrentIndex(m_selectedIndex);

		return m_combo;
	}
	
	QString ComboBox::getToolTip() const
	{
		QString text;
		for (int i = 0; i < m_options.size(); ++i)
		{
			if (i == m_selectedIndex)
				text += "- "+m_options[i] + "\n";
			else
				text += "   "+m_options[i] + "\n";
		}
		text.chop(1); // Remove the last newline
		return text;
	}
	void ComboBox::updateIcon()
	{
		setEditorPlaceholderIcon(IconManager::getIcon(s_comboBoxIcon));
	}
	void ComboBox::editorWidgetDestroyed() const
	{
		m_combo = nullptr;
	}
	void ComboBox::updateText()
	{
		QString text;
		if (m_selectedIndex >= 0 && m_selectedIndex < m_options.size())
		{
			text = m_options[m_selectedIndex];
		}
		setEditorPlaceholderText(text);
	}
}