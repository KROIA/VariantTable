#include "DefaultTypes/PushButton.h"
#include "IconManager.h"

#include <QPushButton>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{
	PushButton::PushButton()
		: CellDataBase()
		, m_text("QPushButton")
	{

	}
	PushButton::PushButton(const PushButton& other)
		: CellDataBase(other)
		, m_text(other.m_text)
	{

	}
	PushButton::PushButton(const QString& text)
		: CellDataBase()
		, m_text(text)
	{

	}


	void PushButton::setText(const QString& text)
	{
		m_text = text;
	}
	const QString& PushButton::getText() const
	{
		return m_text;
	}


	void PushButton::setData(const QVariant& data)
	{
		VT_UNUSED(data);
	}
	void PushButton::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
		//QPushButton* checkBox = qobject_cast<QPushButton*>(editor);
		if (m_editor)
		{
			//m_value = m_editor->isChecked();
		}
	}
	QVariant PushButton::getData() const
	{
		return QVariant(m_text);
	}
	void PushButton::getData(QWidget* editor)
	{
		VT_UNUSED(editor);
		//QPushButton* checkBox = qobject_cast<QPushButton*>(editor);
		if (m_editor)
		{
			m_editor->setText(m_text);
			//checkBox->setMinimumSize(QSize(250, 100));
		}
	}


	QWidget* PushButton::createEditorWidget(QWidget* parent) const
	{
		if (m_editor)
			return m_editor->parentWidget();
		QWidget* editor = new QWidget(parent);
		// Add Layout
		QVBoxLayout* layout = new QVBoxLayout(editor);
		layout->setContentsMargins(5, 5, 5, 5);
		editor->setLayout(layout);
		m_editor = new QPushButton(editor);
		m_editor->setText(m_text);
		layout->addWidget(m_editor);

		connect(m_editor, &QPushButton::clicked, this, &PushButton::onButtonClickedInternal);

		return editor;
	}
	void PushButton::drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const
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

		/*QStyleOptionButton button;
		button.rect = QRect(xPos + TL.x(), yOffset + TL.y(), size, size);
		button.state = m_value ? QStyle::State_On : QStyle::State_Off;
		button.state |= QStyle::State_Enabled;

		QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);*/

		// Draw text
		QRect textRect = QRect(xPos + TL.x() + size, yOffset + TL.y(), rect.width() - size, size);
		painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, m_text);

		// Draw icon
		QRect iconRect = QRect(xPos + TL.x(), yOffset + TL.y(), size, size);
		painter->drawPixmap(iconRect, IconManager::getIcon("press-button.png").pixmap(size, size));
	}
	QString PushButton::getToolTip() const
	{
		return m_text;
	}
	void PushButton::editorWidgetDestroyed() const
	{
		m_editor = nullptr;
	}

	void PushButton::onButtonClickedInternal()
	{
		emit clicked();
	}
}