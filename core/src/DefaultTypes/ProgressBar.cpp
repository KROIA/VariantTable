#include "DefaultTypes/ProgressBar.h"
#include "IconManager.h"

#include <QProgressBar>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{
	QString ProgressBar::s_ProgressBarIcon = "ProgressBar.png";

	ProgressBar::ProgressBar()
		: CellDataBase()
	{
		updateIcon();
		updateText();
	}
	ProgressBar::ProgressBar(const ProgressBar& other)
		: CellDataBase(other)
		, m_min(other.m_min)
		, m_max(other.m_max)
		, m_progress(other.m_progress)
	{

	}
	ProgressBar::ProgressBar(int min, int max, int value)
		: CellDataBase()
		, m_min(min)
		, m_max(max)
		, m_progress(value)
	{
		updateIcon();
		updateText();
	}

	void ProgressBar::setMinimum(int min)
	{
		if(min > m_max)
			min = m_max;
		if (m_bar)
			m_bar->setMinimum(min);
		m_min = min; 
		updateText();
	}

	void ProgressBar::setMaximum(int max)
	{
		if (max < m_min)
			max = m_min;
		if (m_bar)
			m_bar->setMaximum(max);
		m_max = max;
		updateText();
	}

	void ProgressBar::setValue(int progress)
	{
		if (m_bar)
		{
			m_bar->setValue(progress);
		}
		m_progress = progress;
		updateText();
	}
	int ProgressBar::getValue() const
	{
		if (m_bar)
			return m_bar->value();
		return m_progress;
	}

	void ProgressBar::setOrientation(Qt::Orientation orientation)
	{
		if (m_bar)
		{
			m_bar->setOrientation(orientation);
		}
		m_orientation = orientation;
	}

	void ProgressBar::setData(const QVariant& data)
	{
		m_progress = data.toInt();
		updateText();
	}
	void ProgressBar::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
		if (m_bar)
		{
			m_progress = m_bar->value();
			updateText();
		}
	}
	QVariant ProgressBar::getData() const
	{
		return QVariant(m_progress);
	}

	void ProgressBar::getData(QWidget* editor)
	{
		VT_UNUSED(editor);
		if (m_bar)
		{
			m_bar->setValue(m_progress);
		}
	}

	QWidget* ProgressBar::createEditorWidget(QWidget* parent) const
	{
		if (m_bar)
			return m_bar;

		m_bar = new QProgressBar(parent);


		// Set options
		m_bar->setOrientation(m_orientation);
		m_bar->setMinimum(m_min);
		m_bar->setMaximum(m_max);
		m_bar->setValue(m_progress);

		return m_bar;
	}

	QString ProgressBar::getToolTip() const
	{
		int percentage = 100;
		if (m_max != m_min)
		{
			percentage = ((m_progress - m_min) * 100) / (m_max - m_min);
		}		
		return QString::number(percentage) + "%";
	}
	void ProgressBar::updateIcon()
	{
		//setEditorPlaceholderIcon(IconManager::getIcon(s_ProgressBarIconLeftSection));
	}
	void ProgressBar::editorWidgetDestroyed() const
	{
		m_bar = nullptr;
	}
	void ProgressBar::drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const
	{
		float marginX = 5;
		float marginY = 10;
		QRect rect(option.rect.x() + marginX, option.rect.y() + marginY, option.rect.width() - 2 * marginX, option.rect.height() - 2 * marginY);
		QPixmap bar = IconManager::getIcon(s_ProgressBarIcon).pixmap(10, 20);

		drawLoadingBar(painter, rect, m_progress, bar);
	}
	void ProgressBar::updateText()
	{
		setEditorPlaceholderText(getToolTip());
		dataChanged();
	}

	void ProgressBar::drawLoadingBar(QPainter* painter, const QRect& rect, int percentage,
						const QPixmap& bar) const
	{
		if (percentage < 0) percentage = 0;
		if (percentage > 100) percentage = 100;


		// Calculate the width for the middle section based on the percentage
		int totalMiddleWidth = rect.width();
		int middleWidth = (totalMiddleWidth * percentage) / 100;

		painter->drawPixmap(rect.x() , rect.y(), middleWidth, rect.height(), bar);
	}
}