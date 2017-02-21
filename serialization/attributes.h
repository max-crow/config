#pragma once


#define ___L(x) L ## x
#define __L(x) ___L(x)
#define _L(x) __L(x)


namespace config {

struct archive;

class serializable {
public:
    virtual serializable& serialize(config_ptr cfg) = 0;
};

}

#include "serializator.h"
#include "aggregation.h"
#include "archive_name.h"
//#include "archive_safe_reference.h"
#include "nvp.h"
//#include "archive.h"
#include "xml_config_reader.h"


#define BEGIN_CONFIG_SERIALIZATION_MAP()				config::serializable& serialize(config::config_ptr cfg) {
#define CONFIG_SERIALIZATION_NAME(ref_to_variable)			*cfg & CONFIG_SERIALIZATION_NAME_(ref_to_variable);
#define CONFIG_SERIALIZATION_NVP(name)						*cfg & CONFIG_SERIALIZATION_NVP_(name);
#define CONFIG_SERIALIZATION_AGGR(variable)					*cfg & CONFIG_SERIALIZATION_AGGR_(variable);
#define CONFIG_SERIALIZATION_AGGR_IV_COLLECTION(name)		*cfg & CONFIG_SERIALIZATION_AGGR_IV_COLLECTION_(name);
#define CONFIG_SERIALIZATION_AGGR_CLASS_COLLECTION(name)	*cfg & CONFIG_SERIALIZATION_AGGR_CLASS_COLLECTION_(name);
#define CONFIG_SERIALIZATION_AGGR_NAMED_MAP(name)			*cfg & CONFIG_SERIALIZATION_AGGR_NAMED_MAP_(name);
#define END_CONFIG_SERIALIZATION_MAP()						return *this;									\
														}

