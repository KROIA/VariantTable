#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"


class QRadioButton;

namespace VariantTable
{
	class VARIANT_TABLE_API RadioButton : public CellDataBase
	{
		VT_CELL_DATA_OBJ(RadioButton);
	public:
		enum CopyPastePolicy : int
		{
			Text = 0x1,
			SelectedIndex = 0x2,
		};
		RadioButton();
		RadioButton(const RadioButton& other);
		RadioButton(const QStringList& options);



		void setOptions(const QStringList& text);
		const QStringList& getOptions() const;

		void setSelectedIndex(int index);
		int getSelectedIndex() const;


		bool setData(const QVariant& data) override;
		void setData(QWidget* editor) override;
		QVariant getData() const override;
		void getData(QWidget* editor) override;

		QWidget* createEditorWidget(QWidget* parent) override;
		QString getToolTip() const override;
		void editorWidgetDestroyed() override;
		void updateIcon() const override;
		void updateEditorPlaceholderText() const override;

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

	private slots:
		void onSelectionChanged();
	private:
		
		QStringList m_options;
		int m_selectedIndex = -1;

		QWidget* m_editorWidget = nullptr;
		QVector<QRadioButton*> m_editorButtons;

		int m_copyPolicy = CopyPastePolicy::Text | CopyPastePolicy::SelectedIndex;
		int m_pastePolicy = CopyPastePolicy::Text | CopyPastePolicy::SelectedIndex;

		static QString s_radioIcon;
	};
}