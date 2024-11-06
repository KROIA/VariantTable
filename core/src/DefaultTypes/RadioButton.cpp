#include "DefaultTypes/RadioButton.h"
#include "IconManager.h"

#include <QRadioButton>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{
	RadioButton::RadioButton()
		: CellDataBase()
		, m_options({"Option 1", "Option 2", "Option 3"})
	{

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

	}


	void RadioButton::setOptions(const QStringList& options)
	{
		m_options = options;
		m_selectedIndex = -1;
	}
	const QStringList& RadioButton::getOptions() const
	{
		return m_options;
	}

	void RadioButton::setSelectedIndex(int index)
	{
		if (m_editorWidget)
		{
			if (index >= 0 && index < m_editorButtons.size())
			{
				m_selectedIndex = index;
				for (int i = 0; i < m_editorButtons.size(); ++i)
				{
					if (i == index)
						m_editorButtons[i]->setChecked(true);
					else
						m_editorButtons[i]->setChecked(false);
				}
			}
		}
		m_selectedIndex = index;
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
	}
	void RadioButton::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
		//QRadioButton* RadioButton = qobject_cast<QRadioButton*>(editor);
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
	}
	QVariant RadioButton::getData() const
	{
		return QVariant(m_options);
	}
	void RadioButton::getData(QWidget* editor)
	{
		VT_UNUSED(editor);
		//QRadioButton* RadioButton = qobject_cast<QRadioButton*>(editor);
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

	void RadioButton::setColor(const QColor& color)
	{
		CellDataBase::setColor(color);
		CellDataBase::applyColor(m_editorWidget);
	}

	QSize RadioButton::getSizeHint(const QStyleOptionViewItem& option) const
	{
		if (m_editorWidget)
		{
			QSize size = m_editorWidget->sizeHint();
			return size;
		}
		return QSize(option.rect.width(), option.rect.height());
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

		CellDataBase::applyColor(m_editorWidget);


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

		

		// Destroy event
		QObject::connect(m_editorWidget, &QWidget::destroyed, parent, [this]()
			{
				m_editorWidget = nullptr;
				m_editorButtons.clear();
			});
		return m_editorWidget;
	}
	void RadioButton::drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const
	{
		

		QRect rect = option.rect;
		QPoint TL = rect.topLeft();
		float xPos = 5;
		float height = rect.height();
		float size = 20;
		float yOffset = (height - size) / 2;	

		QPen origPen = painter->pen();
		QBrush origBrush = painter->brush();
		// Set the brush color
		painter->setBrush(getColor());
		painter->setPen(getColor());
		painter->drawRect(rect); // x, y, width, height
		painter->setBrush(origBrush);
		painter->setPen(origPen);

		// Draw radio button checked
		//QStyleOptionButton button;
		//button.rect = QRect(xPos + TL.x(), yOffset + TL.y(), size, size);
		//button.state = (m_selectedIndex >= 0 && m_selectedIndex < m_options.size())?QStyle::State_On : QStyle::State_Off;
		//button.state |= QStyle::State_Enabled;
		//QApplication::style()->drawControl(QStyle::CE_RadioButton, &button, painter);


		QString text = "Nothing selected";
		if (m_selectedIndex >= 0 && m_selectedIndex < m_options.size())
			text = m_options[m_selectedIndex];
		// Draw text
		QRect textRect = QRect(xPos + TL.x() + size, yOffset + TL.y(), rect.width() - size, size);
		painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);

		// Draw icon
		QRect iconRect = QRect(xPos + TL.x(), yOffset + TL.y(), size, size);
		painter->drawPixmap(iconRect, IconManager::getIcon("radioButton.png").pixmap(size, size));
	}
	QString RadioButton::getToolTip() const
	{
		QString text;
		for (int i = 0; i < m_options.size(); ++i)
		{
			if (i == m_selectedIndex)
				text += "- " + m_options[i] + "\n";
			else
				text += "   " + m_options[i] + "\n";
		}
		text.chop(1); // Remove the last newline
		return text;
	}
}