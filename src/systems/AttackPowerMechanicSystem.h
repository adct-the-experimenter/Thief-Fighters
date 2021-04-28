
#ifndef ATTACK_POWER_MECHANIC_SYSTEM_H
#define ATTACK_POWER_MECHANIC_SYSTEM_H

#include "core/system.h"

#include <queue>

struct PowerTransferTransaction
{
	std::uint8_t receivingPlayer = 0;
	std::bitset <8> powerTransfered = 0;
};

class AttackPowerMechanicSystem : public System
{
public:
	void Init(std::uint8_t num_players);

	void Update(float& dt);

private:
	std::queue <PowerTransferTransaction> power_transfer_transaction_queue;
	
	std::array <AttackBox*,8> player_attack_boxes_ptrs;
	size_t current_attack_box_index;
	
	std::array <std::uint8_t*,8> player_health_ptrs;
	
	void CollisionDetectionBetweenPlayers();
	
	std::uint8_t m_num_players;
};

#endif
