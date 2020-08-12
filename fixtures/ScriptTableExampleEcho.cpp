#include <stdlib.h>
#include <string.h>

#include "Fixtures.hpp"
#include "SlimList.hpp"

class Echo : public InstanceNode {
    std::string result;

public:
    static std::shared_ptr<Echo> Create(std::shared_ptr<SlimList> args)
    {
        return std::make_shared<Echo>(args);
    }

    Echo(std::shared_ptr<SlimList> args) {}

    std::string echo(std::shared_ptr<SlimList> args)
    {
        if (args->GetLength() != 1)
        {
            return SLIM_EXCEPTION("Exactly 1 argument expected");
        }

        auto stringToEcho = args->GetStringAt(0);
        result = stringToEcho;
        result[sizeof(result) - 1] = '\0';
        return result;
    }

    BEGIN_REGISTER_METHOD()
        REGISTER(Echo, echo)
    END_REGISTER_METHOD()
};

SLIM_CREATE_FIXTURE(Echo)
SLIM_END
