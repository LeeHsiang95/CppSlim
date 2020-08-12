#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "Fixtures.hpp"
#include "SlimList.hpp"

class EchoLoudly : public InstanceNode {
    std::string result;

public:
    static std::shared_ptr<EchoLoudly> Create(std::shared_ptr<SlimList> args)
    {
        return std::make_shared<EchoLoudly>(args);
    }

    EchoLoudly(std::shared_ptr<SlimList> args) {}

    std::string echo(std::shared_ptr<SlimList> args)
    {
        if (args->GetLength() != 1)
        {
            return SLIM_EXCEPTION("Exactly 1 argument expected");
        }

        auto stringToEcho = args->GetStringAt(0);

        result = stringToEcho;

        for (int i = 0; i <  result.size(); ++i) {
            result[i] = std::toupper(result[i]);
        }

        return result;
    }

    std::string repeat(std::shared_ptr<SlimList> args)
    {
        if (args->GetLength() != 0)
        {
            return SLIM_EXCEPTION("Exactly 0 arguments expected");
        }

        return result;
    }

    BEGIN_REGISTER_METHOD()
        REGISTER(EchoLoudly, echo)
        REGISTER(EchoLoudly, repeat)
    END_REGISTER_METHOD()
};

SLIM_CREATE_FIXTURE(EchoLoudly)
SLIM_END
