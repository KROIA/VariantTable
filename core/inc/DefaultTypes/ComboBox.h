#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"
#include <QVector>
#include <QPair>
#include <QMetaType>
#include <QComboBox>



namespace VariantTable
{
	class VARIANT_TABLE_API ComboBox : public CellDataBase
	{
		VT_CELL_DATA_OBJ(ComboBox);
	public:
		typedef QVector<QPair<QString, QVariant>> OptionsType;
		enum CopyPastePolicy : int
		{
			Text = 0x1,
			SelectedIndex = 0x2,
		};
		ComboBox();
		ComboBox(const ComboBox& other);
		ComboBox(const QStringList& options);
		ComboBox(const OptionsType& options);



		void setOptions(const QStringList& text);
		void setOptions(const QVector<QPair<QString, QVariant>> &data);
		const QVector<QPair<QString, QVariant>>& getOptions() const;

		void setCurrentIndex(int index);
	    int getCurrentIndex() const;


		bool setData(const QVariant& data) override;
		void setData(QWidget* editor) override;
		QVariant getData() const override;
		void getData(QWidget* editor) override;

		QWidget* createEditorWidget(QWidget* parent) override;
		QString getToolTip() const override;
		void editorWidgetDestroyed() override;
		void updateIcon() const override;
		void drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const override;


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

		std::shared_ptr<ClipboardData> copyAction() const override;
		bool pasteAction(std::shared_ptr<ClipboardData> pasteData) override;

	private slots:
		void onIndexChanged(int index);
	private:
		QVector<QPair<QString, QVariant>> m_options; // Text and associated data for each option
		int m_selectedIndex = -1;

		QComboBox* m_combo = nullptr;

		int m_copyPolicy = CopyPastePolicy::Text | CopyPastePolicy::SelectedIndex;
		int m_pastePolicy = CopyPastePolicy::Text | CopyPastePolicy::SelectedIndex;

		static QString s_comboBoxIcon;
	};
}

