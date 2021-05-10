
#ifndef ATTACK_POWER_MECHANIC_SYSTEM_H
#define ATTACK_POWER_MECHANIC_SYSTEM_H

#include "core/system.h"

#include <queue>

struct PowerTransferTransaction
{
	std::uint8_t receivingPlayer = 0;
	std::bitset <8> powerTransfered = 0;
};

struct AttackEvent
{
	bool attack = false;
	std::uint8_t player_num_victim = 0;
	std::uint8_t player_num_attacker = 0;
	
};

class AttackPowerMechanicSystem : public System
{
public:
	void Init(std::uint8_t num_players);
	
	void HandlePowerActivation(float& dt);
	
	void MoveAttackBoxesWithPlayer(float& dt);
	
	void PerformNeededPowerTransactions();
	
	void CollisionDetectionBetweenPlayers();
	
	void ReactToCollisions(float& dt);
	
	void DebugRender();
	
private:
	std::queue <PowerTransferTransaction> power_transfer_transaction_queue;
	
	//pointers for use with collision detection and health management
	std::array <AttackBox*,8> player_attack_boxes_ptrs;
	std::array <AttackBox*,8> player_special_attack_boxes_ptrs;
	std::array <std::int16_t*,8> player_health_ptrs;
	std::array <Vector2*,8> player_position_ptrs;
	std::array <std::uint8_t*,8> player_last_hit_by_ptrs;
	std::array <bool*,8> player_alive_ptrs;
	std::array <bool*,8> player_taking_damage_state_ptrs;
	
	//collision handling functions to determine if players hit each other
	bool AreBothPlayersAlive(int& player_a_num, int& player_b_num);
	AttackEvent CheckCollisionBetween2Players(int& player_a_num, int& player_b_num);
	void HandlePossibleCollisionBetweenPlayers(int& player_a_num, int& player_b_num);
	
	
	
	std::uint8_t m_num_players;
};

#endif
