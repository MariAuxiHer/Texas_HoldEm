/********************************************************************************************************************************************************
    Maria Hernandez 
    Casey Collins
    COSC202
    12/06/2022
    player.h
    This program allows X amount of user (greater than 2) to play one hand (preflop, flop, turn, river, and showdown) of fixed-limit Texas Hold'em.
***********************************************************************************************************************************************************/

// fixed-limit hold'em

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <iostream> 
#include <map>
#include <string>
#include <vector>
#include <set>
#include <iterator>
#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine

using namespace std; 

// Set the fields and methods for the Player class. 
class Player {
    int chips; 
    string player_name; 
public: 
    vector <string> player_hand; 
    Player (); 
    string getName() { return player_name; }
    void setName(string name) { player_name = name; }
    int getChips(); 
    void setChips(int); 
    int call(int); 
    int raise(int); 
    int bet();
};

#endif