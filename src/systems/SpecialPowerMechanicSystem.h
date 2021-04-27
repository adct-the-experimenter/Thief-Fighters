
#ifndef SPECIAL_POWER_MECHANIC_SYSTEM_H
#define SPECIAL_POWER_MECHANIC_SYSTEM_H

#include "core/system.h"

#include <queue>

struct PowerTransferTransaction
{
	std::uint8_t receivingPlayer = 0;
	std::bitset <8> powerTransfered = 0;
};

class SpecialPowerMechanicSystem : public System
{
public:
	void Init();

	void Update(float& dt);

private:
	std::queue <PowerTransferTransaction> power_transfer_transaction_queue;
};

#endif
