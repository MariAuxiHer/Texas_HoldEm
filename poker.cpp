/********************************************************************************************************************************************************
    Maria Hernandez 
    Casey Collins
    COSC202
    12/06/2022
    poker.cpp
    This program allows X amount of user (greater than 2) to play one hand (preflop, flop, turn, river, and showdown) of fixed-limit Texas Hold'em.
***********************************************************************************************************************************************************/

#include "poker.h"

Poker::Poker(int num_players) {
    pot = 0;  
    first_bet = false; 
    string name; 

    for (size_t i = 0; i < num_players; i++) {
        players.push_back(Player());
        cout << "Enter the name of Player# " << i << ". Enter an unique name for each player: ";
        cin >> name; 
        players.at(i).setName(name); 
    }

    // All suits
    string CARD_SUITS [] = { 
        "H", "C", "D", "S" 
    };

    // All values. 
    string CARD_VALUES [] = { 
        "2", "3", "4", "5", "6", "7", 
        "8", "9", "10", "J", "Q", "K", "A" 
    };

    // Map (Key -> Card, Value -> Value of the card). We are considering that A can only be the highest card. 
    for (size_t i = 0; i < 13; i++) {
        for (size_t j = 0; j < 4; j++) {
            card_deck_map.insert(pair<string, int>((CARD_VALUES[i] + CARD_SUITS[j]), i+2)); 
        }
    }

    // We also create a vector of the cards to facilitate shuffling. 
    for (size_t i = 0; i < 13; i++) {
        for (size_t j = 0; j < 4; j++) {
            card_deck_vec.push_back(CARD_VALUES[i] + CARD_SUITS[j]);
        }
    }
    shuffle();
}

void Poker::shuffle() {
    std::random_shuffle(&card_deck_vec[0], &card_deck_vec[card_deck_vec.size()]); 
}

void Poker::deal(vector<string> &hand) {
    hand.push_back(card_deck_vec.at(card_deck_vec.size()-1));
    card_deck_vec.pop_back();
}

// Distribute the blinds. We set a fixed amount for the small blind and big blind (SB = 2, BB = 4).
void Poker::distributeBlinds() { 
    int counter = 1; 
    while (counter <= 2) {    
        int player_blind = dealer+counter; 
        int player_chips;
        // The following if/else-if accounts for the scenarios when dealer+1 or dealer+2 is equal or greater than the size of the vector.
        if (dealer+counter == players.size()) player_blind = 0;  
        else if (dealer+counter == players.size()+1) player_blind = 1;  
        if (counter == 1) {
            player_chips = 2; 
            small_blind = player_blind;
            cout << "\n" << players.at(player_blind).getName() << ", you bet 2 chips as the small blind.\n"; 
        }
        else {
            player_chips = 4; 
            big_blind = player_blind; // This is so that in the next round I know who acts first (the one after big_blind)
            cout << players.at(player_blind).getName() << ", you bet 4 chips as the big blind.\n"; 
        }

        // Setting the highest bet as the bet placed by the big Blind (4).
        if (counter == 2) highest_bet = player_chips; 
        counter++; 
    }
    // We don't set the pot yet because we are giving the small and big blind the possibility of calling/raising the highest bet or folding.
    cout << "\nPot: " << getPot() << "\n\n"; 
}

// Method for the first betting round (PreFlop).
void Poker::firstRound() {
    int player_num = big_blind + 1;
    int player_move; 
    int counter = 1;
    int original_number_of_players = players.size();

    do {
        if (player_num >= players.size()) player_num = 0;
       
       // If player enters an option different to 1, 2, or 3, we keep asking for a selection.
        do {
            cout << "These are your options " << players.at(player_num).getName() << " Player #" << player_num 
                << ", \n(1) call, \n(2) raise, \n(3) fold. \nEnter either 1,2,or 3: ";
            cin >> player_move; 
        } while (!(player_move >= 1 && player_move <= 3));

        // If the dealer folds, we set up a new dealer. 
        if (players.at(player_num).getName() == getDealerName() && player_move == 3) {
            if ((getIndex(players, getDealerName()) + 1) != players.size()) dealer = (getIndex(players, getDealerName()) + 1);
            else dealer = 0;
            dealer_name = players.at(dealer).getName();
            cout << "New Dealer: " << getDealerName() << endl; 
        }

        switch(player_move) {
            case 1: 
                // Increasing the pot.
                setPot(players.at(player_num).call(highest_bet));
                cout << "Option: Call. Highest bet: " << highest_bet << ". Players chips: " << players.at(player_num).getChips() << endl; 
                cout << "Updated Pot: " << getPot() << endl; 
                player_num++; 
            break; 
            case 2:
                // Setting the new highest bet and increasing the pot.
                highest_bet = players.at(player_num).raise(highest_bet);
                setPot(highest_bet);
                cout << "Option: Raise. Highest bet: " << highest_bet << ". Players chips: " << players.at(player_num).getChips() << endl; 
                cout << "Updated Pot: " << getPot() << endl; 
                player_num++; 
            break; 
            case 3: 
                cout << "Option: Fold." << endl;  
                // The player is now out of this hand. We use the funtion erase to remove the player from this hand.
                players.erase(players.begin() + player_num); 
                if (players.size() == 1) return; 
            break; 
        }
        counter++;
    }   while (counter <= original_number_of_players); 
     
    // Setting up the new index for the dealer, to account for those players that folded. 
    cout << "Dealer is now at index: " << getIndex(players, getDealerName()) << endl; 
    dealer = getIndex(players, getDealerName());
}

void Poker::otherRounds() {
    first_bet = false; 
    highest_bet = 0; 

    // First player to act is the one at the left of the dealer. 
    int player_num = dealer + 1;
    int player_move; 

    // last_act will be the last person to act in case that there is a betting in the round 
    // (because if there's a betting in the round, then we need to loop again through the players we had already looped).
    string last_act; 
    bool finish_loop = false; 
    int counter = 0; 
    int original_number_of_players = players.size();

    do {
        // cout << "counter " << counter << endl; 
        if (player_num >= players.size()) player_num = 0;

        // If player enters an option different to 1, 2, or 3, we keep asking for a selection.
        do {
            cout << "These are your options " << players.at(player_num).getName() << " Player #" << player_num 
                << ", \n(1) call, \n(2) raise, \n(3) fold, \n(4) bet, \n(5) check. \nEnter either 1,2,3,4,or 5: " 
                << "\nNote: You can't check or bet if someone has already bet: ";
            cin >> player_move; 
        } while (!(player_move >= 1 && player_move <= 5));
        
        // If the dealer folds, we set up a new dealer. 
        if (players.at(player_num).getName() == getDealerName() && player_move == 3) {
            if ((getIndex(players, getDealerName()) + 1) != players.size()) dealer = (getIndex(players, getDealerName()) + 1);
            else dealer = 0;
            dealer_name = players.at(dealer).getName();
            cout << "New Dealer: " << getDealerName() << endl; 
        }

        // Setting the first_bet and the last person who will act. There's only one person betting per round. 
        if (player_move == 4) {
            if (!first_bet) {
                setFirstBet(true); 
                if (player_num-1 >= 0) last_act = players.at(player_num-1).getName();
                else last_act = players.at(players.size()-1).getName();
            }
            else {
                do {
                    cout << "You can't bet beause the first bet was already placed. Enter either 1,2, or 3: ";
                    cin >> player_move; 
                } while (!(player_move == 1 || player_move == 2 || player_move == 3));
            } 
        }
        
        // Check whether the player can check or not. They can check if the first bet has not been placed.
        if (player_move == 5) {
            if (!getFirstBet()) {
                player_num++; 
                counter++; 
                if (counter == original_number_of_players && !first_bet) return; 
                continue;
            }
            else {
                do {
                    cout << "You can't check beause the first bet was already placed. Enter either 1,2, or 3: ";
                    cin >> player_move; 
                } while (!(player_move == 1 || player_move == 2 || player_move == 3));
            }
        }

        // We finish the loop when we go back to the player at the right of the player who placed the bet.
        if (players.at(player_num).getName() == last_act) {
            finish_loop = true; 
        }

        switch (player_move) {
            case 1: 
                // Increasing the pot.
                setPot(players.at(player_num).call(highest_bet));
                cout << "Option: Call. Highest bet: " << highest_bet << ". Players chips: " << players.at(player_num).getChips() << endl; 
                cout << "Updated Pot: " << getPot() << endl; 
                player_num++; 
                counter++; 
            break; 
            case 2:
                // Setting the new highest bet and increasing the pot.
                highest_bet = players.at(player_num).raise(highest_bet);
                setPot(highest_bet);
                cout << "Option: Raise. Highest bet: " << highest_bet << ". Players chips: " << players.at(player_num).getChips() << endl; 
                cout << "Updated Pot: " << getPot() << endl; 
                player_num++; 
                counter++; 
            break; 
            case 3:  
                // The player is now out of this hand. We use the funtion erase to remove the player from this hand.
                cout << "Option: Fold." << endl; 
                players.erase(players.begin() + player_num);  
                counter++;
                if (players.size() == 1) return; 
            break; 
            case 4: 
                cout << "Option: Bet." << endl; 
                // Setting the new highest bet and increasing the pot.
                highest_bet = players.at(player_num).bet();
                setPot(highest_bet);
                 cout << "Option: Bet. Highest bet: " << highest_bet << ". Players chips: " << players.at(player_num).getChips() << endl; 
                cout << "Updated Pot: " << getPot() << endl; 
                player_num++; 
            break; 
        }

        if (counter == original_number_of_players && !first_bet) {
            cout << "Dealer is now at index: " << getIndex(players, getDealerName()) << endl; 
            dealer = getIndex(players, getDealerName());
            return; 
        }
    }   while (!finish_loop); 

    // Setting up the new index for the dealer, to account for those players that folded. 
    cout << "Dealer is now at index: " << getIndex(players, getDealerName()) << endl; 
    dealer = getIndex(players, getDealerName());
}

// Each time a player call/raise/bet we update the Pot.
void Poker::setPot(int chips_to_pot) {
    pot += chips_to_pot;
}

// Method to get the index of the player.
int Poker::getIndex(vector<Player> v, string name) {
    for (size_t i = 0; i < v.size(); i++) {
        if (v.at(i).getName() == name) {
            return i;
        }
    }
    return -1; 
}

// Method to print the card deck.
void Poker::print(vector <string> &cardDeck) {
    cout << endl; 
    for (size_t i = 0; i != cardDeck.size(); i++) {
        cout << cardDeck[i] << endl; 
    }
    cout << endl; 
}

// This method will call all the possible hand ranking functions and pass the hand of the player (sorted) to obtain their highest hand. 
int Poker::traverseHandRankings(vector <string>&player_hand, map<string, int>&deck_map) {
    map<string, int>::iterator itr1;
    vector<int> sorting_vector; 

    // Creating a vector with the values of each card to sort this vector so that we can be able to calculate each hand ranking. 
    for (size_t i = 0; i < player_hand.size(); i++) {
        itr1 = deck_map.find(player_hand.at(i));
        sorting_vector.push_back(itr1->second);
    }
 
    // We sort both the vector of integers and the vector of strings (based on the sorting of the vector of integers).
    sortHand(player_hand, sorting_vector);

    // Highest ranking hands return higest value. The player with the highest value will win. 
    // We are not accounting for the situation when there's a tie. That's code in progress. 
    if (royalFlush(player_hand, deck_map)) { 
        return 10;
    }
    else if (straightFlush(player_hand, deck_map)) {
        return 9;
    }
    else if (fourOfAKind(player_hand, deck_map)) {
        return 8;
    }
    else if (fullHouse(player_hand, deck_map)) {
        return 7;
    }
    else if (flush(player_hand, deck_map)) {
        return 6;
    }
    else if (straight(player_hand, deck_map)) {
        return 5;
    }
    else if (threeOfAKind(player_hand, deck_map)) {
        return 4;
    }
    else if (twoPair(player_hand, deck_map)) {
        return 3;
    }
    else if (onePair(player_hand, deck_map)) {
        return 2;
    }
    else {
        return 1;
    }
}

// Method to sort the vector containing all the points the players got in ascending order
// so that we set the winner as the player in the last index.  
void Poker::sortPoints(vector<int> &sorting_vector, vector<Player>&players) {
    int minIndex;
    int temp1;
    Player temp2; 

    for (size_t i = 0; i < sorting_vector.size()-1; i++) {
        minIndex = i; 
        for (int j = i+1; j < sorting_vector.size(); j++) {
            if (sorting_vector.at(j) < sorting_vector.at(minIndex)) {
                minIndex = j; 
            }
        }
        if (minIndex != i) {
            temp1 = sorting_vector.at(i); 
            temp2 = players.at(i); 
            sorting_vector.at(i) = sorting_vector.at(minIndex);
            players.at(i) = players.at(minIndex);
            sorting_vector.at(minIndex) = temp1; 
            players.at(minIndex) = temp2;
        }
    }
}

// Method to sort the player's hand. We use selection sort to achieve that. 
void Poker::sortHand(vector<string>&player_hand, vector<int>&sorting_vector) {
    int minIndex;
    int temp1;
    string temp2; 

    for (size_t i = 0; i < sorting_vector.size()-1; i++) {
        minIndex = i; 
        for (int j = i+1; j < sorting_vector.size(); j++) {
            if (sorting_vector.at(j) < sorting_vector.at(minIndex)) {
                minIndex = j; 
            }
        }
        if (minIndex != i) {
            temp1 = sorting_vector.at(i); 
            temp2 = player_hand.at(i); 
            sorting_vector.at(i) = sorting_vector.at(minIndex);
            player_hand.at(i) = player_hand.at(minIndex);
            sorting_vector.at(minIndex) = temp1; 
            player_hand.at(minIndex) = temp2;
        }
    }
}

// To calculate whether the player has a royal flush, we pass a sorted vector to the function and subtract the values (obtained from the map)
// of the card at (i+1) and the card at (i), if the difference is 1 four consecutive times && the suits are the same && the cards have values of
// 10 or more, then the player has a royal Flush. 
bool Poker::royalFlush(vector <string> &player_hand, map<string, int> &deck_map) {
int counter = 0; 
map<string, int>::iterator itr1;
map<string, int>::iterator itr2;

    for (size_t i = 0; i < (player_hand.size()-1); i++) {
        itr1 = deck_map.find(player_hand.at(i));
        itr2 = deck_map.find(player_hand.at(i+1));

        if ((abs((itr2->second)-(itr1->second)) == 1) && (player_hand.at(i).at(player_hand.at(i).size()-1) == player_hand.at(i+1).at(player_hand.at(i+1).size()-1))
        && itr1->second >= 10) {
            counter++;
            if (counter == 4) return true; 
        }
        else {
            counter = 0;  
        }
    }
    return false; 
}

// To calculate whether the player has a straight flush, we pass a sorted vector to the function and subtract the values (obtained from the map)
// of the card at (i+1) and the card at (i), if the difference is 1 four consecutive times && the suits are the same 
// then the player has a straight Flush. 
bool Poker::straightFlush(vector <string> &player_hand, map<string, int> &deck_map) {
    int counter = 0; 
    map<string, int>::iterator itr1;
    map<string, int>::iterator itr2;

    for (size_t i = 0; i < (player_hand.size()-1); i++) {
        itr1 = deck_map.find(player_hand.at(i));
        itr2 = deck_map.find(player_hand.at(i+1));
        if ((abs((itr2->second)-(itr1->second)) == 1) 
            && (player_hand.at(i).at(player_hand.at(i).size()-1) == player_hand.at(i+1).at(player_hand.at(i+1).size()-1))) {  
            counter++;
            if (counter == 4) return true; 
        }
        else {
            counter = 0;
        }             
    }
    return false; 
}

// To calculate whether the player has four of a kind, we pass a sorted vector to the function and subtract the values (obtained from the map)
// of the card at (i+1) and the card at (i), if the difference is 0 three consecutive times then the player has four of a kind. 
bool Poker::fourOfAKind(vector <string> &player_hand, map<string, int> &deck_map) {
    int counter = 0; 
    map<string, int>::iterator itr1;
    map<string, int>::iterator itr2;

    for (size_t i = 0; i < (player_hand.size()-1); i++) {
        itr1 = deck_map.find(player_hand.at(i));
        itr2 = deck_map.find(player_hand.at(i+1));
        if ((abs((itr2->second)-(itr1->second)) == 0)) {  
            counter++;
            if (counter == 3) return true; 
        }
        else {
            counter = 0;
        }             
    }
    return false; 
}

// To calculate whether the player has full house, we pass a sorted vector to the function. We set two counters, one counter will 
// track whether there are 3 cards of the same value, and the other whether there are 2 other cards of the same value. 
bool Poker::fullHouse(vector <string> &player_hand, map<string, int> &deck_map) {
    int counter1 = 0, counter2 = 0; 
    bool ct2 = false; 
    map<string, int>::iterator itr1;
    map<string, int>::iterator itr2;

    for (size_t i = 0; i < (player_hand.size()-1); i++) {
        itr1 = deck_map.find(player_hand.at(i));
        itr2 = deck_map.find(player_hand.at(i+1));
        if ((abs((itr2->second)-(itr1->second)) == 0)) {  
            if (!ct2) counter1++;
            else counter2++; 
            if ((counter1 == 1 && counter2 == 2) || (counter2 == 1 && counter1 == 2)) return true; 
        }
        else {
            if (counter1 != 0) {
                if (counter2 != 0) counter2 = 0;
                else ct2 = true; 
            }
        }             
    }
    return false; 
}

// To calculate whether the player has a flush, we pass a sorted vector to the function and if the card at (i+1) and the card at (i), 
// have the same suit four consecutive times the player has a Flush. 
bool Poker::flush(vector <string> &player_hand, map<string, int> &deck_map) {
    int counter = 0; 
    map<string, int>::iterator itr1;
    map<string, int>::iterator itr2;

    for (size_t i = 0; i < (player_hand.size()-1); i++) {
        itr1 = deck_map.find(player_hand.at(i));
        itr2 = deck_map.find(player_hand.at(i+1));

        // Only checking for suits.
        if (player_hand.at(i).at(player_hand.at(i).size()-1) == player_hand.at(i+1).at(player_hand.at(i+1).size()-1)) {  
            counter++;
            if (counter == 4) return true; 
        }
        else {
            counter = 0;
        }                        
    }
    return false; 
}

// To calculate whether the player has a straight, we pass a sorted vector to the function and subtract the values (obtained from the map)
// of the card at (i+1) and the card at (i), if the difference is 1 four consecutive times then the player has a straight. 
bool Poker::straight(vector <string> &player_hand, map<string, int> &deck_map) {
    int counter = 0; 
    map<string, int>::iterator itr1;
    map<string, int>::iterator itr2;

    for (size_t i = 0; i < (player_hand.size()-1); i++) {
        itr1 = deck_map.find(player_hand.at(i));
        itr2 = deck_map.find(player_hand.at(i+1));
        if ((abs((itr2->second)-(itr1->second)) == 1)) {  
            counter++;
            if (counter == 4) return true; 
        }
        else {
            counter = 0;
        }             
    }
    return false; 
}

// To calculate whether the player has three of a kind, we pass a sorted vector to the function and subtract the values (obtained from the map)
// of the card at (i+1) and the card at (i), if the difference is 0 two consecutive times then the player has three of a kind. 
bool Poker::threeOfAKind(vector <string> &player_hand, map<string, int> &deck_map) {
    int counter = 0; 
    map<string, int>::iterator itr1;
    map<string, int>::iterator itr2;

    for (size_t i = 0; i < (player_hand.size()-1); i++) {
        itr1 = deck_map.find(player_hand.at(i));
        itr2 = deck_map.find(player_hand.at(i+1));
        if ((abs((itr2->second)-(itr1->second)) == 0)) {  
            counter++;
            if (counter == 2) return true; 
        }
        else {
            counter = 0;
        }             
    }
    return false; 
}

// To calculate whether the player has two pair, we pass a sorted vector to the function. We set two counters, one counter will 
// track whether there are 2 cards of the same value, and the other whether there are 2 other cards of the same value. 
bool Poker::twoPair(vector <string> &player_hand, map<string, int> &deck_map) {
    int counter1 = 0, counter2 = 0; 
    bool ct2 = false; 
    map<string, int>::iterator itr1;
    map<string, int>::iterator itr2;

    for (size_t i = 0; i < (player_hand.size()-1); i++) {
        itr1 = deck_map.find(player_hand.at(i));
        itr2 = deck_map.find(player_hand.at(i+1));
        if ((abs((itr2->second)-(itr1->second)) == 0)) {  
            if (!ct2) counter1++;
            else counter2++; 
            if ((counter1 == 1 && counter2 == 1) || (counter2 == 1 && counter1 == 1)) return true; 
        }
        else {
            if (counter1 != 0) ct2 = true; 
        }             
    }
    return false; 
}

// To calculate whether the player has one pair, we pass a sorted vector to the function and subtract the values (obtained from the map)
// of the card at (i+1) and the card at (i), if the difference is 0 one time then the player has one pair. 
bool Poker::onePair(vector <string> &player_hand, map<string, int> &deck_map) {
    int counter = 0; 
    map<string, int>::iterator itr1;
    map<string, int>::iterator itr2;
    for (size_t i = 0; i < (player_hand.size()-1); i++) {
        itr1 = deck_map.find(player_hand.at(i));
        itr2 = deck_map.find(player_hand.at(i+1));
        if ((abs((itr2->second)-(itr1->second)) == 0)) {  
            counter++;
            if (counter == 1) return true; 
        }
        else {
            counter = 0;
        }             
    }
    return false; 
}