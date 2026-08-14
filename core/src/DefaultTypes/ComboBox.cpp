#include "DefaultTypes/ComboBox.h"
#include "ClipboardData/ComboBoxClipboardData.h"
#include "IconManager.h"

#include <QComboBox>
#include <QLineEdit>
#include <QApplication>
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace VariantTable
{
	// ComboBoxCellWidget ----------------------------------------------------
	ComboBoxCellWidget::ComboBoxCellWidget(QWidget* parent)
		: CellWidgetBase(parent)
		, m_comboBox(new QComboBox(this))
	{
		auto* l = new QHBoxLayout(this);
		l->setContentsMargins(0, 0, 0, 0);
		l->addWidget(m_comboBox);
		setFocusProxy(m_comboBox);
	}
	void ComboBoxCellWidget::setData(const QVariant& data)
	{
		// Best-effort: select index if int, else find by text
		if (data.userType() == QMetaType::Int)
			m_comboBox->setCurrentIndex(data.toInt());
		else
		{
			int idx = m_comboBox->findText(data.toString());
			if (idx >= 0)
				m_comboBox->setCurrentIndex(idx);
		}
	}
	QVariant ComboBoxCellWidget::getData() const
	{
		return m_comboBox->currentIndex();
	}
	void ComboBoxCellWidget::onAlignmentChanged(Qt::Alignment alignment)
	{
		if (layout())
			layout()->setAlignment(m_comboBox, alignment);
		if (m_comboBox->lineEdit())
			m_comboBox->lineEdit()->setAlignment(alignment);
	}

	// ComboBox --------------------------------------------------------------
	QString ComboBox::s_comboBoxIcon = "comboBox.png";

	ComboBox::ComboBox()
		: CellDataBase()
		, m_options({
			{"Option 1",QVariant()},
			{"Option 2",QVariant()},
			{"Option 3",QVariant()} })
	{
		updateIcon();
		updateEditorPlaceholderText();
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
		updateEditorPlaceholderText();
	}
	ComboBox::ComboBox(const OptionsType& options)
		: CellDataBase()
		, m_options(options)
	{
		updateIcon();
		updateEditorPlaceholderText();
	}


	void ComboBox::setOptions(const QStringList& options)
	{
		m_options.clear();
		for (const auto& option : options)
		{
			m_options.push_back({ option, QVariant() });
		}
		m_selectedIndex = -1;
		updateEditorPlaceholderText();
		dataChanged();
	}
	void ComboBox::setOptions(const QVector<QPair<QString, QVariant>>& data)
	{
		m_options = data;
		m_selectedIndex = -1;
		updateEditorPlaceholderText();
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
			m_combo->comboBox()->setCurrentIndex(index);
		}
		m_selectedIndex = index;
		dataChanged();
	}
	int ComboBox::getCurrentIndex() const
	{
		if(m_combo)
			return m_combo->comboBox()->currentIndex();
		return m_selectedIndex;
	}


	bool ComboBox::setData(const QVariant& data)
	{
		if (data.isValid() && data.canConvert<OptionsType>())
		{
			m_options = data.value<OptionsType>();
			updateEditorPlaceholderText();
			dataChanged();
			return true;
		}
		return false;
	}
	void ComboBox::setData(CellWidgetBase* editor)
	{
		VT_UNUSED(editor);
		if (m_combo)
		{
			m_selectedIndex = m_combo->comboBox()->currentIndex();
			updateEditorPlaceholderText();
		}
	}
	QVariant ComboBox::getData() const
	{
		return QVariant::fromValue(m_options);
	}

	void ComboBox::getData(CellWidgetBase* editor)
	{
		VT_UNUSED(editor);
		if (m_combo)
		{
			IgnoreSignalsContext context(this);
			QComboBox* cb = m_combo->comboBox();
			cb->clear();
			for (const auto& option : m_options)
			{
				cb->addItem(option.first, option.second);
			}
			cb->setCurrentIndex(m_selectedIndex);
		}
	}

	CellWidgetBase* ComboBox::createEditorWidget(QWidget* parent)
	{
		if (m_combo)
			return m_combo;

		IgnoreSignalsContext context(this);
		m_combo = new ComboBoxCellWidget(parent);
		QComboBox* cb = m_combo->comboBox();

		// Set options
		for(const auto& option : m_options)
		{
			cb->addItem(option.first, option.second);
		}
		cb->setCurrentIndex(m_selectedIndex);
		connect(cb, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ComboBox::onIndexChanged);

		return m_combo;
	}

	QString ComboBox::getToolTip() const
	{
		QString text;
		for (int i = 0; i < m_options.size(); ++i)
		{
			if (i == m_selectedIndex)
				text += "-> "+m_options[i].first + "\n";
			else
				text += "   "+m_options[i].first + "\n";
		}
		text.chop(1); // Remove the last newline
		return text;
	}
	void ComboBox::updateIcon() const
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

	std::shared_ptr<ClipboardData> ComboBox::copyAction() const
	{
		std::shared_ptr<ComboBoxClipboardData> data = std::make_shared<ComboBoxClipboardData>();
		if (hasCopyPolicy(CopyPastePolicy::Text))
		{
			QStringList textList;
			for (const auto& option : m_options)
			{
				textList.push_back(option.first);
			}
			data->setText(textList);
		}
		if (hasCopyPolicy(CopyPastePolicy::SelectedIndex))
			data->setSelectedIndex(m_selectedIndex);
		return data;
	}
	bool ComboBox::pasteAction(std::shared_ptr<ClipboardData> pasteData)
	{
		auto comboBoxData = std::dynamic_pointer_cast<ComboBoxClipboardData>(pasteData);
		if (comboBoxData)
		{
			if (comboBoxData->hasText() && hasPastePolicy(CopyPastePolicy::Text))
				setOptions(comboBoxData->getText());
			if (comboBoxData->hasSelectedIndex() && hasPastePolicy(CopyPastePolicy::SelectedIndex))
				setCurrentIndex(comboBoxData->getSelectedIndex());
			return true;
		}
		return false;
	}

	void ComboBox::onIndexChanged(int index)
	{
		if (doIgnoreSignals())
			return;
		m_selectedIndex = index;
		dataChanged();
	}

	void ComboBox::editorWidgetDestroyed()
	{
		m_combo = nullptr;
	}
}


