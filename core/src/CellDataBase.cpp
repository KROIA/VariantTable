#include "CellDataBase.h"
#include "Model.h"
#include "ClipboardData/QVariantClipboardData.h"

#include <QSize>
#include <QPainter>
#include <QEvent>
#include <QKeyEvent>

namespace VariantTable
{
	float CellDataBase::PlaceholderData::iconXPos = 5;
	float CellDataBase::PlaceholderData::iconHeight = 20;

	std::shared_ptr<ClipboardData> CellDataBase::s_clipboardData = nullptr;

	CellDataBase::CellDataBase()
	{
		m_color = QColor(255, 255, 255);
		m_isEditable = true;
		m_mainEditorWidget = nullptr;
		m_editorPlaceholderData.text = "";
		m_editorPlaceholderData.hasIcon = false;
	}
	CellDataBase::CellDataBase(const CellDataBase& other)
	{
		m_color = other.m_color;
		m_isEditable = other.m_isEditable;
		m_mainEditorWidget = nullptr;
		m_editorPlaceholderData = other.m_editorPlaceholderData;


	}
	CellDataBase::~CellDataBase()
	{

	}

	void CellDataBase::setColor(const QColor& color)
	{ 
		m_color = color; 
		applyColor(m_mainEditorWidget);
	}
	void CellDataBase::setEditable(bool editable)
	{
		m_isEditable = editable;
		if (m_mainEditorWidget)
			m_mainEditorWidget->setEnabled(editable);
	}

	QSize CellDataBase::getSizeHint(const QStyleOptionViewItem& option) const
	{
		if (m_mainEditorWidget)
		{
			if (m_mainEditorWidget->isVisible())
			{
				QSize size = m_mainEditorWidget->sizeHint();
				return size;
			}
		}

		QFontMetrics fm(option.font);
		int textWidth = fm.horizontalAdvance(m_editorPlaceholderData.text);
		int textHeight = fm.height();

		int width = textWidth + 10; // Add some padding
		if (m_editorPlaceholderData.hasIcon)
		{
			QSize iconSize = m_editorPlaceholderData.icon.availableSizes().first();
			float aspectRatio = iconSize.width() / (float)iconSize.height();
			width += aspectRatio * PlaceholderData::iconHeight + PlaceholderData::iconXPos * 2; // Add space for the icon and padding
		}
		int height = std::max(textHeight, (int)PlaceholderData::iconHeight) + 10; // Add some padding
		return QSize(width, height);
	}

	void CellDataBase::drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const
	{
		drawEditorPlaceholderColorOverlay(painter, option);
		if (m_editorPlaceholderData.hasIcon)
			drawEditorPlaceholderIcon(painter, option);
		drawEditorPlaceholderText(painter, option);
	}

	/*bool CellDataBase::eventFilter(QObject* obj, QEvent* event)
	{
		if (event->type() == QEvent::KeyPress) 
		{
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
			if (keyEvent->key() == Qt::Key_V && keyEvent->modifiers() & Qt::ControlModifier)
			{
				// Handle paste action here
				std::shared_ptr<ClipboardData> data = getClipboardData();
				if (data)
				{
					onPaste(data);
					return true; // Event handled
				}
			}
			else if (keyEvent->key() == Qt::Key_C && keyEvent->modifiers() & Qt::ControlModifier)
			{
				// Handle copy action here
				onCopy();
				return true; // Event handled
			}
		}
		return QObject::eventFilter(obj, event); // Pass unhandled events to base class
	}*/
	void CellDataBase::copyAction() const
	{
		onCopy();
	}
	bool CellDataBase::pasteAction()
	{
		std::shared_ptr<ClipboardData> data = getClipboardData();
		if (data)
		{
			return onPaste(data);
		}
		return false;
	}


	void CellDataBase::applyColor(QWidget* editor) const
	{
		if (editor)
		{
			editor->setStyleSheet("background-color: " + m_color.name() + ";");
		}
	}
	void CellDataBase::dataChanged() const
	{
		if (m_model)
		{
			IgnoreSignalsContext lock(this);
			m_model->onDataChanged(this);
		}
	}


	void CellDataBase::drawEditorPlaceholderColorOverlay(QPainter* painter, const QStyleOptionViewItem& option) const
	{
		QPen origPen = painter->pen();
		QBrush origBrush = painter->brush();
		// Set the brush color
		painter->setBrush(getColor());
		painter->setPen(getColor());
		painter->drawRect(option.rect); // x, y, width, height
		painter->setBrush(origBrush);
		painter->setPen(origPen);
	}
	void CellDataBase::drawEditorPlaceholderIcon(QPainter* painter, const QStyleOptionViewItem& option) const
	{
		QRect rect = option.rect;
		QPoint TL = rect.topLeft();
		float height = rect.height();
		const float& xPos = PlaceholderData::iconXPos;
		const float &size = PlaceholderData::iconHeight;
		float yOffset = (height - size) / 2;

		

		QSize iconSize = m_editorPlaceholderData.icon.availableSizes().first();
		float aspectRatio = iconSize.width() / (float)iconSize.height();
		// Draw icon
		QRect iconRect = QRect(xPos + TL.x(), yOffset + TL.y(), aspectRatio * size, size);
		painter->drawPixmap(iconRect, m_editorPlaceholderData.icon.pixmap(aspectRatio * size, size));

	}
	void CellDataBase::drawEditorPlaceholderText(QPainter* painter, const QStyleOptionViewItem& option) const
	{
		QRect rect = option.rect;
		QPoint TL = rect.topLeft();
		float height = rect.height();
		float xPos = PlaceholderData::iconXPos;
		const float& size = PlaceholderData::iconHeight;
		float yOffset = (height - size) / 2;

		float aspectRatio = 1;
		if (m_editorPlaceholderData.hasIcon)
		{
			QSize iconSize = m_editorPlaceholderData.icon.availableSizes().first();
			aspectRatio = iconSize.width() / (float)iconSize.height();
			xPos += size * aspectRatio * 1.5;
		}
		
		QRect textRect = QRect(xPos + TL.x(), yOffset + TL.y(), rect.width() - size, size);
		painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, m_editorPlaceholderData.text);
	}
	std::shared_ptr<ClipboardData> CellDataBase::createClipboadData() const
	{
		std::shared_ptr<QVariantClipboardData> data = std::make_shared<QVariantClipboardData>(getData());
		return data;
	}
	bool CellDataBase::onPaste(std::shared_ptr<ClipboardData> pasteData)
	{
		std::shared_ptr<QVariantClipboardData> variantData = std::dynamic_pointer_cast<QVariantClipboardData>(pasteData);
		if (variantData)
		{
			return setData(variantData->getData());
		}
		return false;
	}
	void CellDataBase::onCopy() const
	{
		setClipboardData(createClipboadData());
	}

	void CellDataBase::onEditorWidgetDestroyed()
	{
		m_mainEditorWidget = nullptr;
		updateIcon();
		updateEditorPlaceholderText();
		editorWidgetDestroyed();
	}

	QWidget* CellDataBase::createEditorWidget_internal(QWidget* parent)
	{
		QWidget* editor = createEditorWidget(parent);
		m_mainEditorWidget = editor;
		//m_mainEditorWidget->installEventFilter(this);
		applyColor(editor);
		editor->setEnabled(m_isEditable);
		connect(editor, &QObject::destroyed, this, &CellDataBase::onEditorWidgetDestroyed);
		return editor;
	}



}

QDataStream& operator<<(QDataStream& out, const VariantTable::CellDataBase::OptionsType& t) {
	for (const auto& option : t)
	{
		out << option.first << option.second;
	}
	return out;
}

QDataStream& operator>>(QDataStream& in, VariantTable::CellDataBase::OptionsType& t) {
	QString text;
	QVariant data;
	while (!in.atEnd())
	{
		in >> text >> data;
		t.append(qMakePair(text, data));
	}
	return in;
}