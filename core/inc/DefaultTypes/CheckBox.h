#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"


class QCheckBox;

namespace VariantTable
{
	class VARIANT_TABLE_API CheckBox : public CellDataBase
	{
		VT_CELL_DATA_OBJ(CheckBox);
		public:
			enum CopyPastePolicy : int
			{
				Text = 0x1,
				CheckBoxState = 0x2,
			};
			CheckBox();
			CheckBox(const CheckBox& other);
			CheckBox(const QString& text, bool value = false);

			

			void setText(const QString& text);
			const QString& getText() const;
			void setChecked(bool value);
			bool isChecked() const;

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
			* Defines which aspects of the CheckBox should be included when copying to the clipboard. 
			* For example, you can choose to include only the text, only the checked state, or both.
			*/
			void setCopyPolicy(int policy) { m_copyPolicy = policy; }
			void addCopyPolicy(CopyPastePolicy policy) { m_copyPolicy |= policy; }
			void removeCopyPolicy(CopyPastePolicy policy) { m_copyPolicy &= ~policy; }
			int getCopyPolicy() const { return m_copyPolicy; }
			bool hasCopyPolicy(CopyPastePolicy policy) const { return (m_copyPolicy & policy) != 0; }

			/**
			* CopyPastePolicy enum values can be combined using bitwise OR to specify what data should be included when pasting.
			* Defines which aspects of the CheckBox can be applied when pasting from the clipboard.
			* For example, you can choose to include only the text, only the checked state, or both.
			*/
			void setPastePolicy(int policy) { m_pastePolicy = policy; }
			void addPastePolicy(CopyPastePolicy policy) { m_pastePolicy |= policy; }
			void removePastePolicy(CopyPastePolicy policy) { m_pastePolicy &= ~policy; }
			int getPastePolicy() const { return m_pastePolicy; }
			bool hasPastePolicy(CopyPastePolicy policy) const { return (m_pastePolicy & policy) != 0; }

			std::shared_ptr<ClipboardData> createClipboadData() const override;
			bool onPaste(std::shared_ptr<ClipboardData> pasteData) override;

		private slots:
			void onStateChanged(int state);
		private:
			

			QString m_text;
			bool m_value;
			QCheckBox* m_editor = nullptr;

			int m_copyPolicy = CopyPastePolicy::Text | CopyPastePolicy::CheckBoxState;
			int m_pastePolicy = CopyPastePolicy::Text | CopyPastePolicy::CheckBoxState;

			static QString s_checkedIcon;
			static QString s_uncheckedIcon;
	};
}