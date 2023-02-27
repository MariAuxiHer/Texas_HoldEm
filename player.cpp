/********************************************************************************************************************************************************
    Maria Hernandez 
    Casey Collins
    COSC202
    12/06/2022
    player.cpp
    This program allows X amount of user (greater than 2) to play one hand (preflop, flop, turn, river, and showdown) of fixed-limit Texas Hold'em.
***********************************************************************************************************************************************************/

#include "player.h"

// Call, matches highest bet.
int Player::call(int highest_bet) {
    // cout << this->player_name << endl; 
    int chips_to_pot = highest_bet; 
    chips -= chips_to_pot; 
    return chips_to_pot;
}

// Fixed-limit hold'em -> doubling highest_bet.
int Player::raise(int highest_bet) {
    int chips_to_pot = 2*highest_bet; 
    chips -= chips_to_pot;
    return chips_to_pot;
}

// Check well the betting rules here. I'm not sure... 
int Player::bet() {
    int bet; 
    cout << "How much do you want to bet? "; 
    cin >> bet; 
    chips -= bet;
    return bet; 
}

// Player constructor. Set the chips of all player to 1000. 
Player::Player() {
    chips = 1000;   
}

// getter method to get the chips.
int Player::getChips() {
    return chips; 
}

// setter method to set the chips.
void Player::setChips(int c) {
    chips = c; 
}