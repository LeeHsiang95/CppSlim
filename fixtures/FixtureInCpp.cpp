#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "SlimList.hpp"
#include "Fixtures.hpp"
#include "compatibility.hpp"

class cMultiplication
{
public:
	cMultiplication(){};
	~cMultiplication(){};
	double product()
	{
		return m1*m2;
	}
	double m1;
	double m2;
	char result[32];
};


class Multiplication : public InstanceNode {
	cMultiplication multiplication;
	char result[32];

public:

	static std::shared_ptr<Multiplication> Create(std::shared_ptr<SlimList> args)
	{
		return std::make_shared<Multiplication>(args);
	}

	Multiplication(std::shared_ptr<SlimList> args) {}

	std::string setMultiplicand1(std::shared_ptr<SlimList> args) {
		multiplication.m1 = std::stof(args->GetStringAt(0));
		return result;
	}

	std::string setMultiplicand2(std::shared_ptr<SlimList> args) {
		multiplication.m2 = std::stof(args->GetStringAt(0));
		return result;
	}

	std::string Product(std::shared_ptr<SlimList> args) {
		double product = multiplication.product();
		snprintf(result, 32, "%g", product);
		return result;
	}

	BEGIN_REGISTER_METHOD()
		REGISTER(Multiplication, setMultiplicand1)
		REGISTER(Multiplication, setMultiplicand2)
		REGISTER(Multiplication, Product)
	END_REGISTER_METHOD()
};


SLIM_CREATE_FIXTURE(Multiplication)
SLIM_END
