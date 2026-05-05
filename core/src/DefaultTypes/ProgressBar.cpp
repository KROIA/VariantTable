#include "DefaultTypes/ProgressBar.h"
#include "ClipboardData/QVariantClipboardData.h"
#include "IconManager.h"

#include <QProgressBar>
#include <QApplication>
#include <QPainter>
#include <QHBoxLayout>

namespace VariantTable
{
	ProgressBarCellWidget::ProgressBarCellWidget(QWidget* parent)
		: CellWidgetBase(parent)
		, m_progressBar(new QProgressBar(this))
	{
		auto* l = new QHBoxLayout(this);
		l->setContentsMargins(0, 0, 0, 0);
		l->addWidget(m_progressBar);
	}

	void ProgressBarCellWidget::setData(const QVariant& data)
	{
		if (m_progressBar && data.isValid() && data.canConvert<int>())
			m_progressBar->setValue(data.toInt());
	}

	QVariant ProgressBarCellWidget::getData() const
	{
		if (m_progressBar)
			return QVariant(m_progressBar->value());
		return QVariant();
	}

	void ProgressBarCellWidget::onAlignmentChanged(Qt::Alignment a)
	{
		if (m_progressBar)
			m_progressBar->setAlignment(a);
	}


	QString ProgressBar::s_ProgressBarIcon = "ProgressBar.png";

	ProgressBar::ProgressBar()
		: CellDataBase()
	{
		updateIcon();
		updateEditorPlaceholderText();
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
		updateEditorPlaceholderText();
	}

	void ProgressBar::setMinimum(int min)
	{
		if(min > m_max)
			min = m_max;
		if (m_editor && m_editor->progressBar())
			m_editor->progressBar()->setMinimum(min);
		m_min = min;
		updateEditorPlaceholderText();
	}

	void ProgressBar::setMaximum(int max)
	{
		if (max < m_min)
			max = m_min;
		if (m_editor && m_editor->progressBar())
			m_editor->progressBar()->setMaximum(max);
		m_max = max;
		updateEditorPlaceholderText();
	}

	void ProgressBar::setValue(int progress)
	{
		if (m_editor && m_editor->progressBar())
		{
			m_editor->progressBar()->setValue(progress);
		}
		m_progress = progress;
		updateEditorPlaceholderText();
	}
	int ProgressBar::getValue() const
	{
		if (m_editor && m_editor->progressBar())
			return m_editor->progressBar()->value();
		return m_progress;
	}

	void ProgressBar::setOrientation(Qt::Orientation orientation)
	{
		if (m_editor && m_editor->progressBar())
		{
			m_editor->progressBar()->setOrientation(orientation);
		}
		m_orientation = orientation;
	}

	bool ProgressBar::setData(const QVariant& data)
	{
		if (data.isValid() && data.canConvert<int>())
		{
			m_progress = data.toInt();
			updateEditorPlaceholderText();
			return true;
		}
		return false;
	}
	void ProgressBar::setData(CellWidgetBase* editor)
	{
		VT_UNUSED(editor);
		if (m_editor && m_editor->progressBar())
		{
			m_progress = m_editor->progressBar()->value();
			updateEditorPlaceholderText();
		}
	}
	QVariant ProgressBar::getData() const
	{
		return QVariant(m_progress);
	}

	void ProgressBar::getData(CellWidgetBase* editor)
	{
		VT_UNUSED(editor);
		if (m_editor && m_editor->progressBar())
		{
			IgnoreSignalsContext context(this);
			m_editor->progressBar()->setValue(m_progress);
		}
	}

	CellWidgetBase* ProgressBar::createEditorWidget(QWidget* parent)
	{
		if (m_editor)
			return m_editor;

		IgnoreSignalsContext context(this);
		m_editor = new ProgressBarCellWidget(parent);

		QProgressBar* bar = m_editor->progressBar();
		// Set options
		bar->setOrientation(m_orientation);
		bar->setMinimum(m_min);
		bar->setMaximum(m_max);
		bar->setValue(m_progress);

		return m_editor;
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
	void ProgressBar::editorWidgetDestroyed()
	{
		m_editor = nullptr;
	}
	void ProgressBar::drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const
	{
		float marginX = 5;
		float marginY = 10;
		QRect rect(option.rect.x() + marginX, option.rect.y() + marginY, option.rect.width() - 2 * marginX, option.rect.height() - 2 * marginY);
		QPixmap bar = IconManager::getIcon(s_ProgressBarIcon).pixmap(10, 20);

		drawLoadingBar(painter, rect, m_progress, bar);
	}
	void ProgressBar::updateEditorPlaceholderText() const
	{
		setEditorPlaceholderText(getToolTip());
		dataChanged();
	}

	std::shared_ptr<ClipboardData> ProgressBar::copyAction() const
	{
		std::shared_ptr<QVariantClipboardData> data = std::make_shared<QVariantClipboardData>();
		if (hasCopyPolicy(CopyPastePolicy::ProgressValue))
		{
			data->setData(getData());
		}
		return data;
	}
	bool ProgressBar::pasteAction(std::shared_ptr<ClipboardData> pasteData)
	{
		std::shared_ptr<QVariantClipboardData> variantData = std::dynamic_pointer_cast<QVariantClipboardData>(pasteData);
		if (variantData)
		{
			if(hasPastePolicy(CopyPastePolicy::ProgressValue))
			{
				return setData(variantData->getData());
			}
		}
		return false;
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
