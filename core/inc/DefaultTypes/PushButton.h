#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"
#include <QObject>


class QPushButton;

namespace VariantTable
{
	class VARIANT_TABLE_API PushButton : public QObject, public CellDataBase
	{
		VT_CELL_DATA_OBJ(PushButton);
		Q_OBJECT
		public:
		PushButton();
		PushButton(const PushButton& other);
		PushButton(const QString& text);



		void setText(const QString& text);
		const QString& getText() const;

		void setData(const QVariant& data) override;
		void setData(QWidget* editor) override;
		QVariant getData() const override;
		void getData(QWidget* editor) override;

		QWidget* createEditorWidget(QWidget* parent) const override;
		QString getToolTip() const override;
		void editorWidgetDestroyed() const override;
		void updateIcon() override;

		signals:
		void clicked();

		private slots:
		void onButtonClickedInternal();
		private:
		QString m_text;
		mutable QPushButton* m_editor = nullptr;

		static QString s_pushButtonIcon;
	};
}