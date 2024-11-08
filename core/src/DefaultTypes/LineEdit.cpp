#include "DefaultTypes/LineEdit.h"
#include "IconManager.h"

#include <QLineEdit>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{
	QString LineEdit::s_lineEditIcon = "lineEdit.png";

	LineEdit::LineEdit()
		: CellDataBase()
		, m_text("")
	{
		updateIcon();
	}
	LineEdit::LineEdit(const LineEdit& other)
		: CellDataBase(other)
		, m_text(other.m_text)
	{

	}
	LineEdit::LineEdit(const QString& text)
		: CellDataBase()
		, m_text(text)
	{
		updateIcon();
		setEditorPlaceholderText(m_text);
	}


	void LineEdit::setText(const QString& text)
	{
		m_text = text;
		if (m_editor)
			m_editor->setText(text);
		setEditorPlaceholderText(m_text);
		dataChanged();
	}
	QString LineEdit::getText() const
	{
		if(m_editor)
			return m_editor->text();
		return m_text;
	}


	void LineEdit::setData(const QVariant& data)
	{
		m_text = data.toString();
		setEditorPlaceholderText(m_text);
		dataChanged();
	}
	void LineEdit::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			m_text = m_editor->text();
			setEditorPlaceholderText(m_text);
		}
	}
	QVariant LineEdit::getData() const
	{
		return QVariant(m_text);
	}
	void LineEdit::getData(QWidget* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			m_editor->setText(m_text);
		}
	}

	QWidget* LineEdit::createEditorWidget(QWidget* parent) const
	{
		if (m_editor)
			return m_editor;

		m_editor = new QLineEdit(parent);
		m_editor->setText(m_text);

		return m_editor;
	}

	QString LineEdit::getToolTip() const
	{
		return m_text;
	}
	void LineEdit::drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const
	{
		const QIcon& icon = IconManager::getIcon(s_lineEditIcon);
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

		QRect textRect = QRect(margin + TL.x(), yOffset + TL.y(), rect.width() - iconRect.width() - 3 * margin, size);
		painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, m_text);
	}
	void LineEdit::updateIcon()
	{
		//setEditorPlaceholderIcon(IconManager::getIcon(s_lineEditIcon));
	}
	void LineEdit::editorWidgetDestroyed() const
	{
		m_editor = nullptr;
	}
}