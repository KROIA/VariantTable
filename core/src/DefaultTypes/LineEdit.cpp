#include "DefaultTypes/LineEdit.h"
#include "ClipboardData/QVariantClipboardData.h"
#include "IconManager.h"

#include <QLineEdit>
#include <QApplication>
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRegExpValidator>

namespace VariantTable
{
	// LineEditCellWidget ----------------------------------------------------
	LineEditCellWidget::LineEditCellWidget(QWidget* parent)
		: CellWidgetBase(parent)
		, m_lineEdit(new QLineEdit(this))
	{
		auto* l = new QHBoxLayout(this);
		l->setContentsMargins(0, 0, 0, 0);
		l->addWidget(m_lineEdit);
		setFocusProxy(m_lineEdit);
	}
	void LineEditCellWidget::setData(const QVariant& data)
	{
		m_lineEdit->setText(data.toString());
	}
	QVariant LineEditCellWidget::getData() const
	{
		return m_lineEdit->text();
	}
	void LineEditCellWidget::onAlignmentChanged(Qt::Alignment alignment)
	{
		m_lineEdit->setAlignment(alignment);
	}

	// LineEdit --------------------------------------------------------------
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
			m_editor->lineEdit()->setText(text);
		setEditorPlaceholderText(m_text);
		dataChanged();
	}
	QString LineEdit::getText() const
	{
		if (m_editor)
			return m_editor->lineEdit()->text();
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
	void LineEdit::setData(CellWidgetBase* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			m_text = m_editor->lineEdit()->text();
			setEditorPlaceholderText(m_text);
		}
	}
	QVariant LineEdit::getData() const
	{
		return QVariant(m_text);
	}
	void LineEdit::getData(CellWidgetBase* editor)
	{
		VT_UNUSED(editor);
		if (m_editor)
		{
			IgnoreSignalsContext context(this);
			m_editor->lineEdit()->setText(m_text);
		}
	}

	CellWidgetBase* LineEdit::createEditorWidget(QWidget* parent)
	{
		if (m_editor)
			return m_editor;

		IgnoreSignalsContext context(this);
		m_editor = new LineEditCellWidget(parent);
		QLineEdit* le = m_editor->lineEdit();
		le->setValidator(new QRegExpValidator(m_validatorRegExp, le));
		le->setText(m_text);
		connect(le, &QLineEdit::textChanged, this, &LineEdit::onTextChanged);

		return m_editor;
	}
	void LineEdit::setRegularExpression(const QRegExp& regExp)
	{
		m_validatorRegExp = regExp;
		if (m_editor)
		{
			QLineEdit* le = m_editor->lineEdit();
			le->setValidator(new QRegExpValidator(m_validatorRegExp, le));
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

	std::shared_ptr<ClipboardData> LineEdit::copyAction() const
	{
		std::shared_ptr<QVariantClipboardData> data = std::make_shared<QVariantClipboardData>();
		if (hasCopyPolicy(CopyPastePolicy::Text))
			data->setData(QVariant(m_text));
		return data;
	}
	bool LineEdit::pasteAction(std::shared_ptr<ClipboardData> pasteData)
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
		m_text = newText;
		dataChanged();
	}
	void LineEdit::editorWidgetDestroyed()
	{
		m_editor = nullptr;
	}
}
