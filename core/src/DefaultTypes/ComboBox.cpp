#include "DefaultTypes/ComboBox.h"

#include <QComboBox>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{
	ComboBox::ComboBox()
		: CellDataBase()
		, m_options({ "Option 1", "Option 2", "Option 3" })
	{

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

	}


	void ComboBox::setOptions(const QStringList& options)
	{
		m_options = options;
		m_selectedIndex = -1;
	}
	const QStringList& ComboBox::getOptions() const
	{
		return m_options;
	}


	void ComboBox::setData(const QVariant& data)
	{
		m_options = data.toStringList();
	}
	void ComboBox::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
		//QComboBox* ComboBox = qobject_cast<QComboBox*>(editor);
		if (m_editorWidget)
		{
			m_selectedIndex = m_combo->currentIndex();			
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
		if (m_editorWidget)
		{
			m_combo->clear();
			m_combo->addItems(m_options);
			m_combo->setCurrentIndex(m_selectedIndex);
		}
	}

	QSize ComboBox::getSizeHint(const QStyleOptionViewItem& option) const
	{
		if (m_editorWidget)
		{
			QSize size = m_editorWidget->sizeHint();
			return size;
		}
		return QSize(option.rect.width(), option.rect.height());
	}

	QWidget* ComboBox::createEditorWidget(QWidget* parent) const
	{
		if (m_editorWidget)
			return m_editorWidget;

		m_editorWidget = new QWidget(parent);

		// Add Layout
		QVBoxLayout* layout = new QVBoxLayout(m_editorWidget);
		layout->setContentsMargins(5, 5, 5, 5);
		m_editorWidget->setLayout(layout);

		
		m_combo = new QComboBox(m_editorWidget);
		layout->addWidget(m_combo);

		// Set options
		m_combo->addItems(m_options);
		m_combo->setCurrentIndex(m_selectedIndex);

		/*if (m_selectedIndex < 0 || m_selectedIndex >= m_options.size())
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
				m_combo = nullptr;
			});
		return m_editorWidget;
	}
	void ComboBox::drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const
	{


		QRect rect = option.rect;
		QPoint TL = rect.topLeft();
		float xPos = 5;
		float height = rect.height();
		float size = 20;
		float yOffset = (height - size) / 2;

		// Draw combo box 
		//QStyleOptionButton button;
		//button.rect = rect;//QRect(xPos + TL.x(), yOffset + TL.y(), size, size);
		//QApplication::style()->drawControl(QStyle::CE_PushButtonBevel, &button, painter);

		
		

		// Draw text
		// Make the rect in red
		QPen origPen = painter->pen();
		QString text = "Nothing selected";
		
		if (m_selectedIndex >= 0 && m_selectedIndex < m_options.size())
		{
			text = m_options[m_selectedIndex];
		}
		else
		{
			QPen pen(Qt::red);
			painter->setPen(pen);
		}
		
		
		QRect textRect = QRect(xPos + TL.x(), yOffset + TL.y(), rect.width() - size, size);
		painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);
		painter->setPen(origPen);

		/*
		//float width = dropDownIndicatorRect.width();
			// Draw a simple dropdown arrow (triangle)
		QPointF points[3] = {
			QPointF(xPos - size / 2,yOffset) + TL,
			QPointF(xPos + size / 2,yOffset) + TL,
			QPointF(xPos,size + yOffset) + TL,
		};

		// Create a linear gradient from gray (tip) to black (base)
		QLinearGradient gradient(points[2], points[0]); // Gradient from bottom right to top
		gradient.setColorAt(0, Qt::gray); // Tip color
		gradient.setColorAt(1, Qt::black); // Base color

		// Set the gradient as brush and draw the triangle
		painter->setBrush(gradient);
		painter->drawPolygon(points, 3); // Draw the triangle*/
		

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
}