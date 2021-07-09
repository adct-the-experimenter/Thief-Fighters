#ifndef PLAYER_DEATH_SYSTEM_H
#define PLAYER_DEATH_SYSTEM_H

#include <cstdint>
#include "core/system.h"

#include <array>
#include <vector>

#include <bitset>

class PlayerDeathSystem : public System
{
public:
	void Init(std::uint8_t num_players);

	void Update();
	
	bool OnePlayerWon();
	
	std::int8_t GetPlayerWhoWon();
	
	void Reset();
	
private:

	std::bitset <8> players_alive;
	bool one_player_won;
	std::int8_t winning_player;
};

#endif
