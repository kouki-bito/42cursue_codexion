
#include "codexion.h"

int	check_simulation_status(t_coder *coder)
{
	int	ended;

	ended = is_simulation_ended(coder->data);
	return (ended);
}
