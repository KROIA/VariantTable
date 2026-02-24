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
		, m_options({ 
			{"Option 1",QVariant()}, 
			{"Option 2",QVariant()},
			{"Option 3",QVariant()} })
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
		, m_options()
	{
		for(const auto& option : options)
		{
			m_options.push_back({ option, QVariant()});
		}
		updateIcon();
		updateText();
	}
	ComboBox::ComboBox(const OptionsType& options)
		: CellDataBase()
		, m_options(options)
	{
		updateIcon();
		updateText();
	}


	void ComboBox::setOptions(const QStringList& options)
	{
		m_options.clear();
		for (const auto& option : options)
		{
			m_options.push_back({ option, QVariant() });
		}
		m_selectedIndex = -1;
		updateText();
		dataChanged();
	}
	void ComboBox::setOptions(const QVector<QPair<QString, QVariant>>& data)
	{
		m_options = data;
		m_selectedIndex = -1;
		updateText();
		dataChanged();

	}
	const QVector<QPair<QString, QVariant>>& ComboBox::getOptions() const
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
		m_options = data.value<OptionsType>();
		updateText();
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
		return QVariant::fromValue(m_options);
	}

	void ComboBox::getData(QWidget* editor)
	{
		VT_UNUSED(editor);
		//QComboBox* ComboBox = qobject_cast<QComboBox*>(editor);
		if (m_combo)
		{
			m_combo->clear();
			for (const auto& option : m_options)
			{
				m_combo->addItem(option.first, option.second);
			}
			m_combo->setCurrentIndex(m_selectedIndex);
		}
	}

	QWidget* ComboBox::createEditorWidget(QWidget* parent) const
	{
		if (m_combo)
			return m_combo;
		
		m_combo = new QComboBox(parent);

		// Set options
		for(const auto& option : m_options)
		{
			m_combo->addItem(option.first, option.second);
		}
		m_combo->setCurrentIndex(m_selectedIndex);

		return m_combo;
	}
	
	QString ComboBox::getToolTip() const
	{
		QString text;
		for (int i = 0; i < m_options.size(); ++i)
		{
			if (i == m_selectedIndex)
				text += "- "+m_options[i].first + "\n";
			else
				text += "   "+m_options[i].first + "\n";
		}
		text.chop(1); // Remove the last newline
		return text;
	}
	void ComboBox::updateIcon()
	{
		//setEditorPlaceholderIcon(IconManager::getIcon(s_comboBoxIcon));
	}
	void ComboBox::drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const
	{
		const QIcon& icon = IconManager::getIcon(s_comboBoxIcon);
		/*QString text = m_text;

		// Draw the text and on the right the icon
		QRect rect = option.rect;
		QRect iconRect = rect;
		float iconMargin = 5;
		iconRect.setLeft(rect.right() - icon.availableSizes().first().width());
		iconRect.setTop(rect.top() + iconMargin);
		iconRect.setBottom(rect.bottom() - iconMargin);
		iconRect.setRight(rect.right() - iconMargin);
		rect.setRight(iconRect.left());
		painter->drawText(rect, Qt::AlignVCenter | Qt::AlignLeft, text);
		icon.paint(painter, iconRect, Qt::AlignRight, QIcon::Normal, QIcon::On);
		*/


		QRect rect = option.rect;
		QPoint TL = rect.topLeft();
		float height = rect.height();
		//const float xPos = 5;
		const float size = 15;
		const float margin = 5;
		float yOffset = (height - size) / 2;



		QSize iconSize = icon.availableSizes().first();
		float aspectRatio = iconSize.width() / (float)iconSize.height();
		// Draw icon
		QRect iconRect = QRect(rect.topRight().x() - size - margin, yOffset + TL.y(), aspectRatio * size, size);
		painter->drawPixmap(iconRect, icon.pixmap(aspectRatio * size, size));


		if (m_selectedIndex >= 0 && m_selectedIndex < m_options.size())
		{
			QString text = m_options[m_selectedIndex].first;
			QRect textRect = QRect(margin + TL.x(), yOffset + TL.y(), rect.width() - iconRect.width() - 3 * margin, size);
			painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);
		}

		
	}

	void ComboBox::editorWidgetDestroyed() const
	{
		m_combo = nullptr;
	}
	void ComboBox::updateText()
	{
		/*QString text;
		if (m_selectedIndex >= 0 && m_selectedIndex < m_options.size())
		{
			text = m_options[m_selectedIndex];
		}
		setEditorPlaceholderText(text);*/
	}
}


