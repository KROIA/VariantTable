#pragma once

#include "VariantTable_base.h"
#include <QObject>
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
		static std::shared_ptr<TypeName> create(Args&&... args) \
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
		static TypeName* create(Args&&... args) \
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
		friend class Delegate;
		public:
		CellDataBase() = default;
		virtual ~CellDataBase() = default;

		virtual CellDataBasePtr clone() const = 0;

		virtual void setData(const QVariant& data) = 0;
		virtual void setData(QWidget* editor) = 0;
		virtual QVariant getData() const = 0;
		virtual void getData(QWidget* editor) = 0;

		virtual void setColor(const QColor& color);
		virtual const QColor &getColor() const { return m_color; }


		virtual void setEditable(bool editable);
		virtual bool isEditable() const { return m_isEditable; }
		virtual QSize getSizeHint(const QStyleOptionViewItem& option);
		
		virtual void drawEditorPlaceholder(QPainter* painter, const QStyleOptionViewItem& option) const;
		virtual QString getToolTip() const = 0;

		protected:

		void setEditorPlaceholderText(const QString& text) { m_editorPlaceholderText = text; }
		const QString& getEditorPlaceholderText() const {
			return m_editorPlaceholderText;
		}
		void setEditorPlaceholderIcon(const QIcon* icon) { m_editorPlaceholderIcon = icon; }
		const QIcon* getEditorPlaceholderIcon() const {
			return m_editorPlaceholderIcon;
		}
		
		virtual QWidget* createEditorWidget(QWidget* parent) const = 0;
		virtual void editorWidgetDestroyed() const = 0;

		void applyColor(QWidget* editor) const;

		virtual void drawEditorPlaceholderColorOverlay(QPainter* painter, const QStyleOptionViewItem& option) const;
		virtual void drawEditorPlaceholderIcon(QPainter* painter, const QStyleOptionViewItem& option) const;
		virtual void drawEditorPlaceholderText(QPainter* painter, const QStyleOptionViewItem& option) const;

		private:
		QWidget* createEditorWidget_internal(QWidget* parent) const;

		QColor m_color = QColor(255,255,255);
		QString m_editorPlaceholderText;
		const QIcon* m_editorPlaceholderIcon = nullptr;
		bool m_isEditable = true;
		mutable QWidget* m_mainEditorWidget = nullptr;
	};

	template<typename T>
	static constexpr CellDataTypeID getTypeID()
	{
		return typeid(T).hash_code();
	}

}



template <typename Sender, typename Signal, typename Receiver, typename Slot>
QMetaObject::Connection connect(
	const std::shared_ptr<Sender>& sender,
	Signal signal,
	const std::shared_ptr<Receiver>& receiver,
	Slot slot)
{
	if (!sender || !receiver) {
		// Handle the case where one or both shared pointers are null
		qWarning("Failed to connect: sender or receiver is null");
		return QMetaObject::Connection();
	}

	// Use raw pointers to connect, since QObject::connect only accepts raw QObject pointers
	return QObject::connect(sender.get(), signal, receiver.get(), slot);
}

template <typename Sender, typename Signal, typename Receiver, typename Slot>
QMetaObject::Connection connect(
	const Sender* sender,
	Signal signal,
	const std::shared_ptr<Receiver>& receiver,
	Slot slot)
{
	if (!sender || !receiver) {
		// Handle the case where one or both shared pointers are null
		qWarning("Failed to connect: sender or receiver is null");
		return QMetaObject::Connection();
	}

	// Use raw pointers to connect, since QObject::connect only accepts raw QObject pointers
	return QObject::connect(sender, signal, receiver.get(), slot);
}
template <typename Sender, typename Signal, typename Receiver, typename Slot>
QMetaObject::Connection connect(
	const std::shared_ptr<Sender>& sender,
	Signal signal,
	const Receiver* receiver,
	Slot slot)
{
	if (!sender || !receiver) {
		// Handle the case where one or both shared pointers are null
		qWarning("Failed to connect: sender or receiver is null");
		return QMetaObject::Connection();
	}

	// Use raw pointers to connect, since QObject::connect only accepts raw QObject pointers
	return QObject::connect(sender.get(), signal, receiver, slot);
}

// Overload for connecting to lambdas
template <typename Sender, typename Signal, typename Func>
QMetaObject::Connection connect(
	const std::shared_ptr<Sender>& sender,
	Signal signal,
	Func&& func)
{
	if (!sender) {
		qWarning("Failed to connect: sender is null");
		return QMetaObject::Connection();
	}
	// Connect using a lambda function as the slot
	return QObject::connect(sender.get(), signal, std::forward<Func>(func));
}


template <typename Sender, typename Signal, typename Receiver, typename Slot>
QMetaObject::Connection connect(
	const Sender* sender,
	Signal signal,
	const Receiver* receiver,
	Slot slot)
{
	if (!sender || !receiver) {
		// Handle the case where one or both shared pointers are null
		qWarning("Failed to connect: sender or receiver is null");
		return QMetaObject::Connection();
	}

	// Use raw pointers to connect, since QObject::connect only accepts raw QObject pointers
	return QObject::connect(sender, signal, receiver, slot);
}


// Overload for connecting to lambdas
template <typename Sender, typename Signal, typename Func>
QMetaObject::Connection connect(
	const Sender* sender,
	Signal signal,
	Func&& func)
{
	if (!sender) {
		qWarning("Failed to connect: sender is null");
		return QMetaObject::Connection();
	}
	// Connect using a lambda function as the slot
	return QObject::connect(sender, signal, std::forward<Func>(func));
}
