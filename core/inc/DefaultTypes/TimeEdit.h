#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"
#include <QTime>


class QTimeEdit;

namespace VariantTable
{
	class VARIANT_TABLE_API TimeEdit : public CellDataBase
	{
		VT_CELL_DATA_OBJ(TimeEdit);
		public:
		static void setFormat(const QString& format);
		static const QString& getFormat();

		enum CopyPastePolicy : int
		{
			Time = 0x2,
		};

		TimeEdit();
		TimeEdit(const TimeEdit& other);
		TimeEdit(const QTime& time);



		void setTime(const QTime& time);
		QTime getTime() const;

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

		std::shared_ptr<ClipboardData> createClipboadData() const override;
		bool onPaste(std::shared_ptr<ClipboardData> pasteData) override;

	private slots:
		void onTimeChanged(const QTime& newTime);
		private:
		
		QTime m_time;
		QTimeEdit* m_editor = nullptr;
		
		int m_copyPolicy = CopyPastePolicy::Time;
		int m_pastePolicy = CopyPastePolicy::Time;

		static QString s_format;
		static QString s_timeIcon;
	};
}