#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"
#include "CellWidgetBase.h"


class QLineEdit;

namespace VariantTable
{
	class VARIANT_TABLE_API LineEditCellWidget : public CellWidgetBase
	{
		Q_OBJECT
	public:
		explicit LineEditCellWidget(QWidget* parent = nullptr);

		void setData(const QVariant& data) override;
		QVariant getData() const override;

		QLineEdit* lineEdit() const { return m_lineEdit; }

	protected:
		void onAlignmentChanged(Qt::Alignment alignment) override;

	private:
		QLineEdit* m_lineEdit = nullptr;
	};

	class VARIANT_TABLE_API LineEdit : public CellDataBase
	{
		Q_OBJECT
		VT_CELL_DATA_OBJ(LineEdit);
	public:
		enum CopyPastePolicy : int
		{
			Text = 0x1,
		};

		LineEdit();
		LineEdit(const LineEdit& other);
		LineEdit(const QString& text);



		void setText(const QString& text);
		QString getText() const;

		bool setData(const QVariant& data) override;
		void setData(CellWidgetBase* editor) override;
		QVariant getData() const override;
		void getData(CellWidgetBase* editor) override;

		void setRegularExpression(const QRegExp& regExp);
		void setRegularExpression(const QString& regExp);

		CellWidgetBase* createEditorWidget(QWidget* parent) override;
		QString getToolTip() const override;
		void editorWidgetDestroyed() override;
		void drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const override;

		/**
		* CopyPastePolicy enum values can be combined using bitwise OR to specify what data should be included when copying.
		*/
		void setCopyPolicy(int policy) { m_copyPolicy = policy; }
		void addCopyPolicy(CopyPastePolicy policy) { m_copyPolicy |= policy; }
		void removeCopyPolicy(CopyPastePolicy policy) { m_copyPolicy &= ~policy; }
		int getCopyPolicy() const { return m_copyPolicy; }
		bool hasCopyPolicy(CopyPastePolicy policy) const { return (m_copyPolicy & policy) != 0; }

		/**
		* CopyPastePolicy enum values can be combined using bitwise OR to specify what data should be included when pasting.
		*/
		void setPastePolicy(int policy) { m_pastePolicy = policy; }
		void addPastePolicy(CopyPastePolicy policy) { m_pastePolicy |= policy; }
		void removePastePolicy(CopyPastePolicy policy) { m_pastePolicy &= ~policy; }
		int getPastePolicy() const { return m_pastePolicy; }
		bool hasPastePolicy(CopyPastePolicy policy) const { return (m_pastePolicy & policy) != 0; }

		std::shared_ptr<ClipboardData> copyAction() const override;
		bool pasteAction(std::shared_ptr<ClipboardData> pasteData) override;

	private slots:
		void onTextChanged(const QString& newText);
	private:
		QString m_text;
		QRegExp m_validatorRegExp;
		LineEditCellWidget* m_editor = nullptr;

		int m_copyPolicy = CopyPastePolicy::Text;
		int m_pastePolicy = CopyPastePolicy::Text;

		static QString s_lineEditIcon;
	};
}
