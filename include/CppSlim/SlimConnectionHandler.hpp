#pragma once
#include <string>
#include <memory>

class ISlim;
class ComLink;

class SlimConnectionHandler {
public:
	SlimConnectionHandler(std::shared_ptr<ComLink> comLink);
	
	int Run();
	void RegisterSlimMessageHandler(std::shared_ptr<ISlim> slim);

protected:
	int read_size();

	std::shared_ptr<ComLink> com_link_;
	std::shared_ptr<ISlim> slim_;
};
