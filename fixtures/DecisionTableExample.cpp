#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "SlimList.hpp"
#include "Fixtures.hpp"
#include "compatibility.hpp"


class Division : public InstanceNode
{
public:
	double numerator;
	double denominator;
	char result[32];

	Division(std::shared_ptr<SlimList> args) {}


	std::string setNumerator(std::shared_ptr<SlimList> args) {
		numerator = std::stof(args->GetStringAt(0));
		return "";
	}

	std::string setDenominator(std::shared_ptr<SlimList> args) {
		denominator = std::stof(args->GetStringAt(0));
		if (denominator == 0.0)
			return SLIM_EXCEPTION("You shouldn't divide by zero now should ya?");
		return "";
	}

	std::string Quotient(std::shared_ptr<SlimList> args) {
		double quotient = numerator / denominator;
		snprintf(result, 32, "%g", quotient);
		return result;
	}

	std::string execute(std::shared_ptr<SlimList> args) {
		return "";
	}

	std::string reset(std::shared_ptr<SlimList> args) {
		denominator = 0.0f;
		numerator = 0.0f;
		return "";
	}

	std::string table(std::shared_ptr<SlimList> args) {
		return "";
	}

	static std::shared_ptr<Division> Create(std::shared_ptr<SlimList> args) {
		return std::make_shared<Division>(args);
	}

	BEGIN_REGISTER_METHOD()
		REGISTER(Division, setNumerator)
		REGISTER(Division, setDenominator)
		REGISTER(Division, Quotient)
		REGISTER(Division, execute)
		REGISTER(Division, reset)
		REGISTER(Division, table)
	END_REGISTER_METHOD()
};


SLIM_CREATE_FIXTURE(Division)
SLIM_END
