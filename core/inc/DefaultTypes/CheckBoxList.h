#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"


class QCheckBox;

namespace VariantTable
{
	class VARIANT_TABLE_API CheckBoxList : public CellDataBase
	{
		VT_CELL_DATA_OBJ(CheckBoxList);
	public:
		

		CheckBoxList();
		CheckBoxList(const CheckBoxList& other);
		CheckBoxList(const QStringList& options);
		CheckBoxList(const OptionsType& options);



		void setOptions(const QStringList& text);
		void setOptions(const QVector<QPair<QString, QVariant>>& data);
		const QVector<QPair<QString, QVariant>>& getOptions() const;

		void setCheckedIndexes(const QVector<int>& indexes);
		QVector<int> getCheckedIndexes() const;


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
		QVector<QPair<QString, QVariant>> m_options;
		QVector<int> m_selectedIndexes;

		mutable QWidget* m_editorWidget = nullptr;
		mutable QVector<QCheckBox*> m_checkBoxes;

		static QString s_checkedIcon;
	};
}