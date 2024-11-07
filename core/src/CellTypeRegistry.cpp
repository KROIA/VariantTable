#include "CellTypeRegistry.h"
#include "DefaultTypes/DefaultTypes.h"

#ifdef _DEBUG
#include <QDebug>
#endif

namespace VariantTable
{
	void CellTypeRegistry::registerDefaultTypes()
	{
		registerType_internal<CheckBox>();
		registerType_internal<LineEdit>();
		registerType_internal<RadioButton>();
		registerType_internal<ComboBox>();
		registerType_internal<CheckBoxList>();
		registerType_internal<PushButton>();
		registerType_internal<TextEdit>();
		registerType_internal<TimeEdit>();
		registerType_internal<DateEdit>();
		registerType_internal<DateTimeEdit>();
		registerType_internal<ProgressBar>();
	}
	const CellDataBasePtr CellTypeRegistry::getDefaultType()
	{
		return getRegisteredType<CheckBoxList>();
	}



	CellTypeRegistry::CellTypeRegistry()
	{
		registerDefaultTypes();
	}
	CellTypeRegistry::~CellTypeRegistry()
	{

	}

	CellTypeRegistry& CellTypeRegistry::getInstance()
	{
		static CellTypeRegistry instance;
		return instance;
	}


	const CellDataBasePtr CellTypeRegistry::getRegisteredTypeFromID(CellDataTypeID typeId)
	{
		CellTypeRegistry& instance = getInstance();
		if (instance.m_cellFactories.find(typeId) == instance.m_cellFactories.end())
		{
#ifdef _DEBUG
			qWarning() << "Type with id " << typeId << " not found";
#endif
			return nullptr;
		}
		return instance.m_cellFactories[typeId];
	}

	CellDataBasePtr CellTypeRegistry::create(const CellDataTypeID typeId)
	{
		const CellDataBasePtr cellData = getRegisteredTypeFromID(typeId);
		if (!cellData)
		{
#ifdef _DEBUG
			qWarning() << "Type with id " << typeId << " not found";
#endif
			return nullptr;
		}
		return cellData->clone();
	}
	CellDataBasePtr CellTypeRegistry::createDefaultType()
	{
		return getDefaultType()->clone();
	}

	



}