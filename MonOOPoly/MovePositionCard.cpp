#include "MovePositionCard.h"
#include "Board.h"

MovePositionCard::MovePositionCard(int tiles)
{
    this->tiles = tiles;
}

Card* MovePositionCard::clone() const
{
    return new MovePositionCard(*this);
}

void MovePositionCard::applyEffect(Player& player, const Vector<Player*>& allPlayers) const
{
    Board::setId(player, player.getId() + tiles);
    if(tiles> 0)
        std::cout << "Player " << Tokens[(size_t)player.getUsername()] << " moves " << tiles << " fields forward." << std::endl;
	else
		std::cout << "Player " << Tokens[(size_t)player.getUsername()] << " moves " << -tiles << " fields backward." << std::endl;
}


void MovePositionCard::reverseEffect(Player& player, const Vector<Player*>& allPlayers) const {
    Board::setId(player, player.getId() - tiles);
}

bool MovePositionCard::isMovementCard() const
{
    return true;
}

int MovePositionCard::getTiles() const
{
    return tiles;
}
