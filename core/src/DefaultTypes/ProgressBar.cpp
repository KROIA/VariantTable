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
		dataChanged();
	}

	void ProgressBar::setMaximum(int max)
	{
		if (max < m_min)
			max = m_min;
		if (m_bar)
			m_bar->setMaximum(max);
		m_max = max;
		dataChanged();
	}

	void ProgressBar::setValue(int progress)
	{
		if (m_bar)
		{
			m_bar->setValue(progress);
		}
		m_progress = progress;
		dataChanged();
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
		dataChanged();
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
		setEditorPlaceholderIcon(IconManager::getIcon(s_ProgressBarIcon));
	}
	void ProgressBar::editorWidgetDestroyed() const
	{
		m_bar = nullptr;
	}
	void ProgressBar::updateText()
	{
		setEditorPlaceholderText(getToolTip());
	}
}