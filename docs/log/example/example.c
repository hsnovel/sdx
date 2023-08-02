#define LOG_IMPLEMENTATION
#include "../../../log.h"

int main()
{
	log_error("Error log");
	log_warn("Warning log, something happening");
	log_info("This is just an info");
	return 0;
}
