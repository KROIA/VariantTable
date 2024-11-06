#pragma once

#include "VariantTable_base.h"
#include <QVariant>
#include <QSize>
#include <QStyleOptionViewItem>
#ifdef VT_USE_SHARED_PTR
#include <memory>
#endif

class QWidget;
class QPainter;

namespace VariantTable
{
	class CellDataBase;
	typedef size_t CellDataTypeID;


#ifdef VT_USE_SHARED_PTR
	typedef std::shared_ptr<CellDataBase> CellDataBasePtr;

	#define VT_CELL_DATA_CLONE_FUNC(Typename) \
		CellDataBasePtr clone() const override \
		{ \
			return std::make_shared<Typename>(*this); \
		}
	#define VT_CELL_DATA_CREATOR(TypeName) \
        template <typename... Args> \
		static CellDataBasePtr create(Args&&... args) \
		{ \
			return std::make_shared<TypeName>(std::forward<Args>(args)...); \
		}

#else
	typedef CellDataBase* CellDataBasePtr;

	#define VT_CELL_DATA_CLONE_FUNC(Typename) \
		CellDataBasePtr clone() const override \
		{ \
			return new Typename(*this); \
		}
	#define VT_CELL_DATA_CREATOR(TypeName) \
		template <typename... Args> \
		static CellDataBasePtr create(Args&&... args) \
		{ \
			return new TypeName(std::forward<Args>(args)...); \
		}
#endif



#define VT_CELL_DATA_OBJ(Typename) \
	private: \
	public: \
		VT_CELL_DATA_CREATOR(Typename); \
		VT_CELL_DATA_CLONE_FUNC(Typename); \
    private:


	




	class CellDataBase
	{
		public:
		CellDataBase() = default;
		virtual ~CellDataBase() = default;

		virtual CellDataBasePtr clone() const = 0;

		virtual void setData(const QVariant& data) = 0;
		virtual void setData(QWidget* editor) = 0;
		virtual QVariant getData() const = 0;
		virtual void getData(QWidget* editor) = 0;

		virtual void setColor(const QColor& color) { m_color = color; }
		virtual const QColor &getColor() const { return m_color; }


		virtual void setEditable(bool editable) { m_isEditable = editable; }
		virtual bool isEditable() const { return m_isEditable; }
		virtual QSize getSizeHint(const QStyleOptionViewItem& option) const = 0;
		virtual QWidget* createEditorWidget(QWidget *parent) const = 0;
		virtual void drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const = 0;
		virtual QString getToolTip() const = 0;

		protected:
		void applyColor(QWidget* editor) const
		{
			if (editor)
			{
				editor->setStyleSheet("background-color: " + m_color.name() + ";");
			}
		}


		private:
		QColor m_color = QColor(255,255,255);
		bool m_isEditable = true;
	};

	template<typename T>
	static constexpr CellDataTypeID getTypeID()
	{
		return typeid(T).hash_code();
	}
}