#include "Cards.h"
using namespace std;

Card::Card(){
    //initialize card to a random card type
    int n = rand() % 5;
    card = cardList[n];
}

Card::Card(string s){
    bool found = false;
    //search and set card type
    for(int i = 0; i < cardList->size(); i++){
        if(cardList[i] == s){
            card = cardList[i];
            found = true;
        }
    }
    //no card matching
    if(!found){
        cout << "No card of name \'" << s << "\' exists." << endl;
    }
    found = false;
}

Card::Card(Card& c){
    card = c.card;
}


void Card::play(string s, Hand* h, Deck* d) {
    int n = -1;
    //look if specified card is in hand
    for (int i = 0; i < h->currentHand.size(); i++) {
        if (s == h->currentHand[i]->card) {
            n = i;
            break;
        }
    }
    //immediately exit if card is not in hand
    if (n == -1) {
        cout << "Card not in hand." << endl;
        return;
    }
    //add card to deck and remove from hand to simulate playing a card
    d->addDeck(h->currentHand[n]);
    cout << "Played card " << s << " from deck." << endl;
    h->removeHand(n);
}



Deck::Deck(){
    //assume the deck size to be 25
    for(int i = 0; i < 25; i++){
        deckList.push_back(new Card());
    }
}

Deck::Deck(int n){
    //build a deck of n sizes
    for(int i = 0; i < n; i++){
        deckList.push_back(new Card());
        test.push_back(1);
    }
}

Deck::Deck(Deck& d)
{
    for (int i = 0; i < deckList.size(); i++)
    {
        Card* c = new Card(*d.deckList[i]);
        d.deckList.push_back(c);
    }
}

void Deck::draw(Hand* hand) {
    //draw a random card from deck
    int card = rand() % deckList.size();
    Card* c = deckList[card];
    //add from deck to hand
    hand->addHand(c);
    cout << "Drawn card " << c->card << " from deck." << endl;
    removeDeck(card);
}

void Deck::addDeck(Card* c)
{
    deckList.push_back(c);
}

void Deck::removeDeck(int i)
{
    deckList.erase(deckList.begin() + i);
}

ostream& operator << (ostream& os, const Deck& deck){
    int a = 0, b = 0, c = 0, d = 0, e = 0;
    Card* card = new Card();
    for (int i = 0; i < deck.deckList.size(); i++) {
        if (deck.deckList[i]->card == card->cardList[0]) {
            a += 1;
        } else if (deck.deckList[i]->card == card->cardList[1]) {
            b += 1;
        } else if (deck.deckList[i]->card == card->cardList[2]) {
            c += 1;
        } else if (deck.deckList[i]->card == card->cardList[3]) {
            d += 1;
        } else if (deck.deckList[i]->card == card->cardList[4]) {
            e += 1;
        }
    }
    return (os << "\nDisplaying current deck:\n Deck Size: " << deck.deckList.size() <<"\n\t" << card->cardList[0] << ": " << a 
        << "\n\t" << card->cardList[1] << ": " << b
        << "\n\t" << card->cardList[2] << ": " << c
        << "\n\t" << card->cardList[3] << ": " << d
        << "\n\t" << card->cardList[4] << ": " << e << endl);
}
string Deck::toString() const{
    stringstream ss;
    ss << (*this);
    return ss.str();
}


Hand::Hand()
{
    //assume the hand size to be 5
    for (int i = 0; i < 5; i++) {
        currentHand.push_back(new Card());
    }
}

Hand::Hand(int n)
{
    //assume create a hand of n size
    for (int i = 0; i < n; i++) {
        currentHand.push_back(new Card());
    }
}

Hand::Hand(Hand& h)
{
    for (int i = 0; i < currentHand.size(); i++)
    {
        Card* c = new Card(*h.currentHand[i]);
        h.currentHand.push_back(c);
    }
}

void Hand::addHand(Card* c){
    currentHand.push_back(c);
}

void Hand::removeHand(int i){
    currentHand.erase(currentHand.begin() + i);
}

ostream& operator << (ostream& os, const Hand& d) {
    string str;
    for (int i = 0; i < d.currentHand.size(); i++) {
        str = str + d.currentHand.at(i)->card + "\n";
    }
    return (os << "\nDisplaying current hand:\n" << str);
}
string Hand::toString() const {
    stringstream ss;
    ss << (*this);
    return ss.str();
}


//test methods
void testCards();