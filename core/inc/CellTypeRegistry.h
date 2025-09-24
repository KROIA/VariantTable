#pragma once

#include "VariantTable_base.h"
#include "CellDataBase.h"
#include <unordered_map>

#ifdef _DEBUG
#include <QDebug>
#endif

namespace VariantTable
{

	class VARIANT_TABLE_API CellTypeRegistry
	{
		CellTypeRegistry();
		~CellTypeRegistry();

		static CellTypeRegistry& getInstance();

	public:

		template<typename CellDataType>
		static void registerType()
		{
			CellTypeRegistry& instance = getInstance();
			instance.registerType_internal<CellDataType>();
		}

#ifdef VT_USE_SHARED_PTR
		template<typename CellDataType>
		static const CellDataBasePtr getRegisteredType()
		{
			// Check base type
			static_assert(std::is_base_of<CellDataBase, CellDataType>::value,
				"CelllDataType must be derived from CellDataBase");
			CellDataTypeID typeId = getTypeID<CellDataType>();
			return getRegisteredTypeFromID(typeId);
			//return std::dynamic_pointer_cast<const CellDataType>();
		}
#else
		template<typename CellDataType>
		static const CellDataBasePtr getRegisteredType()
		{
			// Check base type
			static_assert(std::is_base_of<CellDataBase, CellDataType>::value,
				"CelllDataType must be derived from CellDataBase");
			CellDataTypeID typeId = getTypeID<CellDataType>();
			return getRegisteredTypeFromID(typeId);
		}
#endif


		
		static const std::unordered_map<CellDataTypeID, CellDataBasePtr>& getRegisteredTypes()
		{
			return getInstance().m_cellFactories;
		}

		static const CellDataBasePtr getRegisteredTypeFromID(CellDataTypeID typeId);
		static const CellDataBasePtr getDefaultType();
		static CellDataBasePtr create(const CellDataTypeID typeId);
		static CellDataBasePtr createDefaultType();
		

	private:
		template<typename CellDataType>
		void registerType_internal()
		{

			// Check base type
			static_assert(std::is_base_of<CellDataBase, CellDataType>::value,
				"CelllDataType must be derived from CellDataBase");

			CellDataTypeID typeId = getTypeID<CellDataType>();
			if (m_cellFactories.find(typeId) != m_cellFactories.end())
			{
#ifdef _DEBUG
				qDebug() << "Type already registered";
#endif
				return;
			}
			m_cellFactories[typeId] = CellDataType::create();
		}

		void registerDefaultTypes();

		std::unordered_map<size_t, CellDataBasePtr> m_cellFactories;
	};
}