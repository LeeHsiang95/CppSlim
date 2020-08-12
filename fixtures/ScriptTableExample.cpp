#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Fixtures.hpp"
#include "SlimList.hpp"
#include "compatibility.hpp"

class Count : public InstanceNode {
	int count_{0};
	char result[32];

public:
	static std::shared_ptr<Count> Create(std::shared_ptr<SlimList> args)
	{
		return std::make_shared<Count>(args);
	}

	Count(std::shared_ptr<SlimList> args) {}

	std::string count(std::shared_ptr<SlimList> args) {
		++count_;
		return "";
	}

	std::string counter(std::shared_ptr<SlimList> args) {
		snprintf(result, 32, "%d", count_);
		return result;
	}

	BEGIN_REGISTER_METHOD()
		REGISTER(Count, count)
		REGISTER(Count, counter)
	END_REGISTER_METHOD()
};

SLIM_CREATE_FIXTURE(Count)
SLIM_END

