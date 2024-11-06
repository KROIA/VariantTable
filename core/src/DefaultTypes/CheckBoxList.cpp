#include "DefaultTypes/CheckBoxList.h"

#include <QCheckBox>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{
	CheckBoxList::CheckBoxList()
		: CellDataBase()
		, m_options({ "Option 1", "Option 2", "Option 3" })
	{

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

	}


	void CheckBoxList::setOptions(const QStringList& options)
	{
		m_options = options;
		m_selectedIndexes.clear();
	}
	const QStringList& CheckBoxList::getOptions() const
	{
		return m_options;
	}


	void CheckBoxList::setData(const QVariant& data)
	{
		m_options = data.toStringList();
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

	QSize CheckBoxList::getSizeHint(const QStyleOptionViewItem& option) const
	{
		if (m_editorWidget)
		{
			QSize size = m_editorWidget->sizeHint();
			return size;
		}
		return QSize(option.rect.width(), option.rect.height());
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
		/*if (m_selectedIndex < 0 || m_selectedIndex >= m_editorButtons.size())
		{
			QPalette palette = m_editorWidget->palette();
			palette.setColor(QPalette::Window, Qt::red);
			m_editorWidget->setAutoFillBackground(true);  // Required to apply the color
			m_editorWidget->setPalette(palette);
		}
		else
		{
			QPalette palette = m_editorWidget->palette();
			palette.setColor(QPalette::Window, Qt::lightGray);
			m_editorWidget->setAutoFillBackground(true);  // Required to apply the color
			m_editorWidget->setPalette(palette);
		}*/



		// Destroy event
		QObject::connect(m_editorWidget, &QWidget::destroyed, parent, [this]()
			{
				m_editorWidget = nullptr;
				m_checkBoxes.clear();
			});
		return m_editorWidget;
	}
	void CheckBoxList::drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const
	{


		QRect rect = option.rect;
		QPoint TL = rect.topLeft();
		float xPos = 5;
		float height = rect.height();
		float size = 20;
		float yOffset = (height - size) / 2;

		// Draw the checkboxes which are checked in a comumn
		/*for (int i = 0; i < m_selectedIndexes.size(); ++i)
		{
			QRect checkBoxRect = QRect(TL.x() + xPos, TL.y() + yOffset, size, size);

			QStyleOptionButton checkboxOption;
			checkboxOption.rect = checkBoxRect;
			checkboxOption.state = QStyle::State_On | QStyle::State_Enabled; // Set the state based on the checkbox

			// Draw the checkbox using the application's style
			QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkboxOption, painter);

			QStyleOptionButton checkboxOptionLabel;
			checkboxOptionLabel.rect = QRect(checkBoxRect.x() + checkBoxRect.width(), checkBoxRect.y(), rect.width() - size, size);
			checkboxOptionLabel.text = m_options[m_selectedIndexes[i]];
			QApplication::style()->drawControl(QStyle::CE_CheckBoxLabel, &checkboxOptionLabel, painter);

			yOffset += size + 5; // Adjust the position for the next checkbox
		}*/

		QString text;
		for (int i : m_selectedIndexes)
		{
			if (i >= 0 && i < m_options.size())
				text += m_options[i] + ", ";
		}
		if (!text.isEmpty())
			text.chop(2); // Remove the last comma and space
		else
			text = "Nothing selected";

		QRect textRect = QRect(xPos + TL.x(), yOffset + TL.y(), rect.width() - size, size);
		painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);	

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
}