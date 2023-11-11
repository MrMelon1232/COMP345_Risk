#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

class Deck;
class Hand;

class Card {
	public:
		Card(); //default constructor
		Card(string s); //constructor
		Card(Card& c);
		
		void play(string s, Hand* h, Deck* d); //play card from hand, put back card to deck

		//fixed list of cards
		//string getCardType(int n);
		string card;
		string const cardList[5] = 
			{"BOMB", "REINFORCEMENT", "BLOCKADE", "AIRLIFT", "DIPLOMACY"};

	private:
};

class Hand {
public:
	Hand(); //default constructor
	Hand(int n); //constructor
	Hand(Hand& h);
	vector <Card*> currentHand;

	//extra operators
	void addHand(Card* c); //add card to hand
	void removeHand(int i); //remove card from hand
	int handSize();

	friend ostream& operator << (ostream& os, const Hand& hand);
	virtual string toString() const;
private:
};

class Deck {
	public:
		Deck(); //default constructor
		Deck(int size); //constructor
		Deck(Deck& d);
		void draw(Hand* hand); //draw cards from a deck

		vector <Card*> deckList;
		vector <int> test;

		//extra operators
		void addDeck(Card* c); //add card to deck
		void removeDeck(int i); //remove card from deck

		friend ostream& operator << (ostream& os, const Deck& deck);
		virtual string toString() const;
	private:
};

void testCards();