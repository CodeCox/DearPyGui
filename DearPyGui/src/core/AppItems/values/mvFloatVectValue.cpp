#include "mvFloatVectValue.h"
#include <utility>
#include "mvContext.h"
#include "mvModule_DearPyGui.h"
#include <string>
#include "mvPythonExceptions.h"

namespace Marvel {

    void mvFloatVectValue::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
		std::vector<mvPythonDataElement> args;

        AddCommonArgs(args,(CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SOURCE)
        );

		args.push_back({ mvPyDataType::FloatList, "default_value", mvArgType::KEYWORD_ARG, "()" });
		args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_3", "Parent to add this item to. (runtime adding)" });
        
		mvPythonParserSetup setup;
		setup.about = "Adds a float vect value.";
		setup.category = { "Widgets", "Values" };
		setup.returnType = mvPyDataType::UUID;
		
		mvPythonParser parser = FinalizeParser(setup, args);

        parsers->insert({ s_command, parser });
    }

    mvFloatVectValue::mvFloatVectValue(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

	PyObject* mvFloatVectValue::getPyValue()
	{
		return ToPyList(*_value);
	}

	void mvFloatVectValue::setPyValue(PyObject* value)
	{
		*_value = ToFloatVect(value);
	}

	void mvFloatVectValue::setDataSource(mvUUID dataSource)
	{
		if (dataSource == _source) return;
		_source = dataSource;

		mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
		if (!item)
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
				"Source item not found: " + std::to_string(dataSource), this);
			return;
		}
		if (item->getValueType() != getValueType())
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<std::vector<float>>*>(item->getValue());
	}

}
