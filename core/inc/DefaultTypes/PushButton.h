#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"
#include <QObject>


class QPushButton;

namespace VariantTable
{
	class VARIANT_TABLE_API PushButton : public CellDataBase
	{
		VT_CELL_DATA_OBJ(PushButton);
		Q_OBJECT
		public:
		enum CopyPastePolicy : int
		{
			Text = 0x1,
		};
		PushButton();
		PushButton(const PushButton& other);
		PushButton(const QString& text);



		void setText(const QString& text);
		const QString& getText() const;

		bool setData(const QVariant& data) override;
		void setData(QWidget* editor) override;
		QVariant getData() const override;
		void getData(QWidget* editor) override;

		QWidget* createEditorWidget(QWidget* parent) override;
		QString getToolTip() const override;
		void editorWidgetDestroyed() override;
		void updateIcon() const override;


		/**
		* CopyPastePolicy enum values can be combined using bitwise OR to specify what data should be included when copying.
		* Defines which aspects of the ComboBox should be included when copying to the clipboard.
		* For example, you can choose to include only the text list, only the selected index, or both.
		*/
		void setCopyPolicy(int policy) { m_copyPolicy = policy; }
		void addCopyPolicy(CopyPastePolicy policy) { m_copyPolicy |= policy; }
		void removeCopyPolicy(CopyPastePolicy policy) { m_copyPolicy &= ~policy; }
		int getCopyPolicy() const { return m_copyPolicy; }
		bool hasCopyPolicy(CopyPastePolicy policy) const { return (m_copyPolicy & policy) != 0; }

		/**
		* CopyPastePolicy enum values can be combined using bitwise OR to specify what data should be included when pasting.
		* Defines which aspects of the ComboBox can be applied when pasting from the clipboard.
		* For example, you can choose to include only the text list, only the selected index, or both.
		*/
		void setPastePolicy(int policy) { m_pastePolicy = policy; }
		void addPastePolicy(CopyPastePolicy policy) { m_pastePolicy |= policy; }
		void removePastePolicy(CopyPastePolicy policy) { m_pastePolicy &= ~policy; }
		int getPastePolicy() const { return m_pastePolicy; }
		bool hasPastePolicy(CopyPastePolicy policy) const { return (m_pastePolicy & policy) != 0; }

		std::shared_ptr<ClipboardData> createClipboadData() const override;
		bool onPaste(std::shared_ptr<ClipboardData> pasteData) override;

		signals:
		void clicked();

		private slots:
		void onButtonClickedInternal();
		private:
		QString m_text;
		QPushButton* m_editor = nullptr;

		int m_copyPolicy = CopyPastePolicy::Text;
		int m_pastePolicy = CopyPastePolicy::Text;

		static QString s_pushButtonIcon;
	};
}