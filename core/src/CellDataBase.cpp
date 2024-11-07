#include "CellDataBase.h"

#include <QSize>
#include <QPainter>

namespace VariantTable
{


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

	QSize CellDataBase::getSizeHint(const QStyleOptionViewItem& option)
	{
		if (m_mainEditorWidget)
		{
			QSize size = m_mainEditorWidget->sizeHint();
			return size;
		}
		return QSize(option.rect.width(), option.rect.height());
	}

	void CellDataBase::drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const
	{
		drawEditorPlaceholderColorOverlay(painter, option);
		drawEditorPlaceholderIcon(painter, option);
		drawEditorPlaceholderText(painter, option);
	}


	void CellDataBase::applyColor(QWidget* editor) const
	{
		if (editor)
		{
			editor->setStyleSheet("background-color: " + m_color.name() + ";");
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
		if (!m_editorPlaceholderIcon)
			return;

		QRect rect = option.rect;
		QPoint TL = rect.topLeft();
		float xPos = 5;
		float height = rect.height();
		float size = 20;
		float yOffset = (height - size) / 2;

		

		QSize iconSize = m_editorPlaceholderIcon->availableSizes().first();
		float aspectRatio = iconSize.width() / (float)iconSize.height();
		// Draw icon
		QRect iconRect = QRect(xPos + TL.x(), yOffset + TL.y(), aspectRatio * size, size);
		painter->drawPixmap(iconRect, m_editorPlaceholderIcon->pixmap(aspectRatio * size, size));

	}
	void CellDataBase::drawEditorPlaceholderText(QPainter* painter, const QStyleOptionViewItem& option) const
	{
		QRect rect = option.rect;
		QPoint TL = rect.topLeft();
		float xPos = 5;
		float height = rect.height();
		float size = 20;
		float yOffset = (height - size) / 2;

		
		float aspectRatio = 1;
		if (m_editorPlaceholderIcon)
		{
			QSize iconSize = m_editorPlaceholderIcon->availableSizes().first();
			aspectRatio = iconSize.width() / (float)iconSize.height();
			xPos += size * aspectRatio;
		}
		QRect textRect = QRect(xPos + TL.x(), yOffset + TL.y(), rect.width() - size, size);
		painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, m_editorPlaceholderText);
	}


	QWidget* CellDataBase::createEditorWidget_internal(QWidget* parent) const
	{
		QWidget* editor = createEditorWidget(parent);
		m_mainEditorWidget = editor;
		applyColor(editor);
		editor->setEnabled(m_isEditable);
		QObject::connect(editor, &QObject::destroyed, [this]()
						 {
							 m_mainEditorWidget = nullptr;
							 editorWidgetDestroyed();
						 });
		return editor;
	}
}