/********************************************************************************************************************************************************
    Maria Hernandez 
    Casey Collins
    COSC202
    12/06/2022
    poker.h
    This program allows X amount of user (greater than 2) to play one hand (preflop, flop, turn, river, and showdown) of fixed-limit Texas Hold'em.
***********************************************************************************************************************************************************/

#ifndef _POKER_H_
#define _POKER_H_

#include "player.h"
#include "player.cpp"

// Set the fields and methods for the Poker class. 
class Poker {
    int pot, big_blind, small_blind, dealer, highest_bet;
    bool first_bet;
    string dealer_name;  
public: 
    vector<Player> players;
    map <string, int> card_deck_map; 
    vector <string> card_deck_vec; 
    vector <string> community_cards; 
    vector <int> player_points; 

    // Methods to deal a card to a player or to the middle, suffle the deck and print the deck.
    void deal(vector<string> &);
    void shuffle();
    void print(vector <string> &); 

    // Constructor.
    Poker (int);

    // Functions that deal with the betting rounds. 
    void distributeBlinds(); 
    void firstRound(); 
    void otherRounds();
    int getPot() { return pot; } 
    void setPot(int); 
    int getBigBlind() { return big_blind; }
    void setBigBlind(int bB) { big_blind = bB; }
    int getSmallBlind() { return small_blind; }
    void setSmallBlind(int sB) { small_blind = sB; }
    int getDealer() { return dealer; }
    void setDealer(int d) { dealer = d; }
    string getDealerName() { return dealer_name; }
    void setDealerName(string name) { dealer_name = name; }
    bool getFirstBet() { return first_bet; } 
    void setFirstBet(bool fb) { first_bet = fb; }
    int getIndex(vector<Player> v, string name);

    // Functions to calculate the hand ranking of each player. 
    int traverseHandRankings(vector <string>&, map<string, int>&);
    void sortHand(vector<string>&, vector<int>&);
    void sortPoints(vector <int>&, vector<Player>&players);
    bool royalFlush(vector <string>&, map<string, int>&);
    bool straightFlush(vector <string>&, map<string, int>&);
    bool fourOfAKind(vector <string>&, map<string, int>&);
    bool fullHouse(vector <string>&, map<string, int>&);
    bool flush(vector <string>&, map<string, int>&);
    bool straight(vector <string>&, map<string, int>&);
    bool threeOfAKind(vector <string>&, map<string, int>&);
    bool twoPair(vector <string>&, map<string, int>&);
    bool onePair(vector <string>&, map<string, int>&);
};

#endif