#include "Cards.h"
#include <iostream>

using namespace std;

void testCards() {
    cout << "testing for Cards" << endl;

    //initialize
    Deck *d = new Deck();
    Card *c = new Card();
    Hand *h = new Hand();

    //display
    cout << d->toString();
    cout << h->toString();
    //draw method called
    cout << "---------Draw method called---------" << endl;
    for (int i = 0; i < 5; i++) {
        d->draw(h);
    }
    //display after draw
    cout << d->toString();
    cout << h->toString();

    //play called
    cout << "---------Play method called---------" << endl;
    for (int i = 0; i < 5; i++) {
        c->play("BOMB", h, d);
    }
    //display after play
    cout << d->toString();
    cout << h->toString();

    delete d;
    d = NULL;
    delete c;
    c = NULL;
    delete h;
    h = NULL;
}

int main(){
    testCards();
}