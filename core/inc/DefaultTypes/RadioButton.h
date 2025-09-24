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
		RadioButton();
		RadioButton(const RadioButton& other);
		RadioButton(const QStringList& options);



		void setOptions(const QStringList& text);
		const QStringList& getOptions() const;

		void setSelectedIndex(int index);
		int getSelectedIndex() const;


		void setData(const QVariant& data) override;
		void setData(QWidget* editor) override;
		QVariant getData() const override;
		void getData(QWidget* editor) override;

		QWidget* createEditorWidget(QWidget* parent) const override;
		QString getToolTip() const override;
		void editorWidgetDestroyed() const override;
		void updateIcon() override;
	private:
		void updateText();
		QStringList m_options;
		int m_selectedIndex = -1;

		mutable QWidget* m_editorWidget = nullptr;
		mutable QVector<QRadioButton*> m_editorButtons;

		static QString s_radioIcon;
	};
}