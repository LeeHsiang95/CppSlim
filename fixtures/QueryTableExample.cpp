#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Fixtures.hpp"
#include "SlimList.hpp"
#include "StatementExecutor.hpp"


class EmployeePayRecordsRow : public InstanceNode {
	std::string result;

public:
	static std::shared_ptr<EmployeePayRecordsRow> Create(std::shared_ptr<SlimList> args)
	{
		return std::make_shared<EmployeePayRecordsRow>(args);
	}

	EmployeePayRecordsRow(std::shared_ptr<SlimList> args) {}

	std::string query(std::shared_ptr<SlimList> args) {
		std::shared_ptr<SlimList> id = std::make_shared<SlimList>();
		id->AddString("id");
		id->AddString("1");

		std::shared_ptr<SlimList> pay = std::make_shared<SlimList>();
		pay->AddString("pay");
		pay->AddString("1000");

		std::shared_ptr<SlimList> record1 = std::make_shared<SlimList>();
		record1->AddList(id);
		record1->AddList(pay);

		std::shared_ptr<SlimList> records = std::make_shared<SlimList>();
		records->AddList(record1);

		result = records->Serialize();
		return result;
	}

	BEGIN_REGISTER_METHOD()
		REGISTER(EmployeePayRecordsRow, query)
	END_REGISTER_METHOD()
};

SLIM_CREATE_FIXTURE(EmployeePayRecordsRow)
SLIM_END

