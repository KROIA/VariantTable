#include "DefaultTypes/LineEdit.h"
#include "ClipboardData/QVariantClipboardData.h"
#include "IconManager.h"

#include <QLineEdit>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>
#include <QRegExpValidator>

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


	bool LineEdit::setData(const QVariant& data)
	{
		if(data.isValid() && data.type() == QVariant::String)
		{
			m_text = data.toString();
			setEditorPlaceholderText(m_text);
			dataChanged();
			return true;
		}
		return false;
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
			IgnoreSignalsContext context(this);
			m_editor->setText(m_text);
		}
	}

	QWidget* LineEdit::createEditorWidget(QWidget* parent)
	{
		if (m_editor)
			return m_editor;

		IgnoreSignalsContext context(this);
		m_editor = new QLineEdit(parent);
		m_editor->setValidator(new QRegExpValidator(m_validatorRegExp, m_editor));
		m_editor->setText(m_text);
		connect(m_editor, &QLineEdit::textChanged, this, &LineEdit::onTextChanged);

		return m_editor;
	}
	void LineEdit::setRegularExpression(const QRegExp& regExp)
	{ 
		m_validatorRegExp = regExp; 
		if (m_editor)
		{
			m_editor->setValidator(new QRegExpValidator(m_validatorRegExp, m_editor));
		}
	}
	void LineEdit::setRegularExpression(const QString& regExp) 
	{ 
		setRegularExpression(QRegExp(regExp));
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

	std::shared_ptr<ClipboardData> LineEdit::createClipboadData() const
	{
		std::shared_ptr<QVariantClipboardData> data = std::make_shared<QVariantClipboardData>();
		if (hasCopyPolicy(CopyPastePolicy::Text))
			data->setData(QVariant(m_text));
		return data;
	}
	bool LineEdit::onPaste(std::shared_ptr<ClipboardData> pasteData)
	{
		auto variantData = std::dynamic_pointer_cast<QVariantClipboardData>(pasteData);
		if (!variantData)
			return false;

		if (hasPastePolicy(CopyPastePolicy::Text))
			return setData(variantData->getData());

		return true;
	}

	void LineEdit::onTextChanged(const QString& newText)
	{
		if (doIgnoreSignals())
			return;
		VT_UNUSED(newText);
		dataChanged();
	}
	void LineEdit::editorWidgetDestroyed()
	{
		m_editor = nullptr;
	}
}