#include "DefaultTypes/TimeEdit.h"
#include "IconManager.h"

#include <QTimeEdit>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>

namespace VariantTable
{

	QString TimeEdit::s_format = "hh:mm:ss";

	


	TimeEdit::TimeEdit()
		: CellDataBase()
	{

	}
	TimeEdit::TimeEdit(const TimeEdit& other)
		: CellDataBase(other)
	{

	}
	TimeEdit::TimeEdit(const QTime& time)
		: CellDataBase()
		, m_time(time)
	{

	}

	void TimeEdit::setFormat(const QString& format)
	{
		s_format = format;
	}
	const QString& TimeEdit::getFormat()
	{
		return s_format;
	}


	void TimeEdit::setTime(const QTime& time)
	{
		m_time = time;
		if (m_editor)
			m_editor->setTime(m_time);
	}
	QTime TimeEdit::getTime() const
	{
		if (m_editor)
			return m_editor->time();
		return m_time;
	}


	void TimeEdit::setData(const QVariant& data)
	{
		m_time = data.toTime();
	}
	void TimeEdit::setData(QWidget* editor)
	{
		VT_UNUSED(editor);
		//QTimeEdit* TimeEdit = qobject_cast<QTimeEdit*>(editor);
		if (m_editor)
		{
			m_time = m_editor->time();
		}
	}
	QVariant TimeEdit::getData() const
	{
		return QVariant(m_time);
	}
	void TimeEdit::getData(QWidget* editor)
	{
		VT_UNUSED(editor);
		//QTimeEdit* TimeEdit = qobject_cast<QTimeEdit*>(editor);
		if (m_editor)
		{
			m_editor->setTime(m_time);
		}
	}
	void TimeEdit::setColor(const QColor& color)
	{
		CellDataBase::setColor(color);
		CellDataBase::applyColor(m_editor);
	}
	QSize TimeEdit::getSizeHint(const QStyleOptionViewItem& option) const
	{
		if (m_editor)
		{
			QSize size = m_editor->sizeHint();
			return size;
		}
		return QSize(option.rect.width(), option.rect.height());
	}

	QWidget* TimeEdit::createEditorWidget(QWidget* parent) const
	{
		if (m_editor)
			return m_editor;
		//QWidget* editor = new QWidget(parent);
		// Add Layout
		//QVBoxLayout* layout = new QVBoxLayout(parent);
		//layout->setContentsMargins(5, 5, 5, 5);
		//editor->setLayout(layout);
		m_editor = new QTimeEdit(parent);
		m_editor->setDisplayFormat(s_format);
		m_editor->setTime(m_time);
		//layout->addWidget(m_editor);

		CellDataBase::applyColor(m_editor);

		// Destroy event
		QObject::connect(m_editor, &QWidget::destroyed, parent, [this]()
						 {
							 m_editor = nullptr;
						 });
		return m_editor;
	}
	void TimeEdit::drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const
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

		// Draw text
		QString text = m_time.toString(s_format);
		QRect textRect = QRect(xPos + TL.x() + size, yOffset + TL.y(), rect.width() - size, size);
		painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);

		// Draw icon
		QRect iconRect = QRect(xPos + TL.x(), yOffset + TL.y(), size, size);
		painter->drawPixmap(iconRect, IconManager::getIcon("clock.png").pixmap(size, size));
	}
	QString TimeEdit::getToolTip() const
	{
		return m_time.toString(s_format);
	}
}