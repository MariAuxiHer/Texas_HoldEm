/********************************************************************************************************************************************************
    Maria Hernandez 
    Casey Collins
    COSC202
    12/09/2022
    main.cpp
    This program allows X amount of user (greater than 2) to play one hand (preflop, flop, turn, river, and showdown) of fixed-limit Texas Hold'em.
***********************************************************************************************************************************************************/

#include "poker.h"
#include "poker.cpp"

using namespace std; 

int main () {
    int number_players, d; 
    do {
        cout << "How many players? Enter 2 or more: ";
        cin >> number_players; 
    } while (number_players < 2);
 
    // Creating a new game. 
    Poker poker_game(number_players); 

    // Ask for the dealer. 
    do {
        cout << "\nWho is the dealer? Please enter the player number \n(Number between 0 and " << poker_game.players.size()-1 << "): ";
        cin >> d; 
    } while(d > (poker_game.players.size()-1) || d < 0);
    
    poker_game.setDealer(d);
    poker_game.setDealerName(poker_game.players.at(d).getName());
    cout << "The dealer is: " << poker_game.getDealerName() << endl; 

    // Distributing small and big blinds. 
    poker_game.distributeBlinds(); 

    // Distribute 2 cards to each player.  
    for (size_t i = 0; i < poker_game.players.size(); i++) {
        cout << "Player " << poker_game.players.at(i).getName() << " cards: " << endl; 
        for (size_t j = 0; j < 2; j++) {
            poker_game.deal(poker_game.players.at(i).player_hand); 
            cout << poker_game.players.at(i).player_hand.at(j) << " "; 
        }
        cout << "\n\n";
    }

    // First round of betting. PreFlop.
    // Player to the left of Big Blind is first act, then the others act. 
    cout << "\nPreFlop \n";
    poker_game.firstRound();

    if (poker_game.players.size() == 1) {
        cout << "The player that wins the pot of " << poker_game.getPot() << " is: " << poker_game.players.at(0).getName() << endl; 
        return 0;
    }

    // Getting the pot.
    cout << "Pot at the end of PreFlop: " << poker_game.getPot() << endl;

    // 2nd round of betting. Flop.

    // First, give 3 community cards to the middle.
    for (size_t j = 0; j < 3; j++) {
        poker_game.deal(poker_game.community_cards); 
    }

    // Flop
    if (poker_game.players.size() == 1) {
        cout << "The player that wins the pot of " << poker_game.getPot() << " is: " << poker_game.players.at(0).getName() << endl; 
        return 0;
    }
    cout << "\nFlop \n";
    poker_game.otherRounds();
    cout << "Pot at the end of Flop: " << poker_game.getPot() << endl;

    // 3rd round of betting. Turn.
    // First, give 1 community card to the middle.
    poker_game.deal(poker_game.community_cards); 

    if (poker_game.players.size() == 1) {
        cout << "The player that wins the pot of " << poker_game.getPot() << " is: " << poker_game.players.at(0).getName() << endl; 
        return 0;
    }
    cout << "\nTurn \n";
    poker_game.otherRounds(); 
    cout << "Pot at the end of Turn: " << poker_game.getPot() << endl;

    // 4th round of betting. River.
    // First, give 1 community card to the middle.
    poker_game.deal(poker_game.community_cards); 

    if (poker_game.players.size() == 1) {
        cout << "The player that wins the pot of " << poker_game.getPot() << " is: " << poker_game.players.at(0).getName() << endl;
        return 0;
    }
    cout << "\nRiver \n";
    poker_game.otherRounds(); 
    cout << "Pot at the end of River: " << poker_game.getPot() << endl;

    cout << "Community Cards \n"; 
    for (size_t i = 0; i < poker_game.community_cards.size(); i++) {
        cout << poker_game.community_cards.at(i) << " "; 
    }

    // Showdown
    // We compare the the players' hands and the player with the best hand, wins the pot. 

    // Each player "gets" all 5 community cards. 
    for (size_t i = 0; i < poker_game.players.size(); i++) {
        for (int j = 0; j < 5; j++) {
            poker_game.players.at(i).player_hand.push_back(poker_game.community_cards.at(j)); 
        }
        cout << "\n";
    }

    // Each player's hand.
    for (size_t i = 0; i < poker_game.players.size(); i++) {
        cout << "Player " << poker_game.players.at(i).getName() << " cards: " << endl; 
        for (size_t j = 0; j < poker_game.players.at(i).player_hand.size(); j++) {
            cout << poker_game.players.at(i).player_hand.at(j) << " "; 
        }
        cout << "\n\n";
    }
    
    // We use the traverseHandRankings() function to get the highest possible ranking card of each player.
    for (size_t i = 0; i < poker_game.players.size(); i++) {
       poker_game.player_points.push_back(poker_game.traverseHandRankings(poker_game.players.at(i).player_hand, poker_game.card_deck_map));   
    }

    // Set a vector with the points of all players. This vector will be sorted and the last index will be the winner. 
    cout << "\nPoints of each player \n" << endl; 
    for (size_t i = 0; i < poker_game.player_points.size(); i++) {
       cout << "Player " << poker_game.players.at(i).getName() << " got: " << poker_game.player_points.at(i) << endl; 
    }

    // We call the method to sort the vector containing all the points the players got in ascending order
    // so that we set the winner as the player in the last index.  
    poker_game.sortPoints(poker_game.player_points, poker_game.players);
    cout << "The player that wins the pot of " << poker_game.getPot() << " is: " << poker_game.players.at(poker_game.player_points.size()-1).getName() << endl; 
}