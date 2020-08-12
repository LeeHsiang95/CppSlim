#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "SlimList.hpp"
#include "Fixtures.hpp"

class ExceptionsExample : public InstanceNode {
public:
	static std::shared_ptr<ExceptionsExample> Create(std::shared_ptr<SlimList> args)
	{
		if (args->GetLength() < 1) {
			return nullptr;
		}

		return std::make_shared<ExceptionsExample>(args);
	}

	ExceptionsExample(std::shared_ptr<SlimList> args) {}

	std::string setTrouble(std::shared_ptr<SlimList> args) {
		return SLIM_EXCEPTION("You stink");
	}

	BEGIN_REGISTER_METHOD()
		REGISTER(ExceptionsExample, setTrouble)
	END_REGISTER_METHOD()
};

SLIM_CREATE_FIXTURE(ExceptionsExample) 
SLIM_END

