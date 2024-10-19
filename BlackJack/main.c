//This program runs a game of Blackjack.

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//Global variables. Number of decks, number of cards per deck (should always be 52), the shuffle value, and how many cards are allowed in each "hand". Shuffle value is how thoroughly the deck is shuffled.
int NUMDECKS = 4;
int NUMCARDS = 52;
int SHUFFLEVALUE = 10000;
int NUMHAND = 7;

//Structure of a card. Suit is just a character, rank is two characters (to fit 10, the rest lead with a space), value (11 default for Ace), and a bool to show if the card is facedown.
struct card {
	char suit;
	char rank[2];
	short value;
	short flipped;
};

//Prototype functions.
void fillDeck(struct card deck[]);							//Populate deck.
void shuffleDeck(struct card deck[]);						//Shuffle deck.
void printCards(struct card deck[], int numElements);		//Print out all cards. Used for hands and testing the deck.
int dealCard(struct card deck[], struct card hand[]);		//Deals a card from the deck to a player.
int handValue(struct card hand[]);							//Counts value of hand. Doesn't count facedown cards.
void emptyCard(struct card deck[], int index);				//Empties a card.
void scrolldown(int lines);									//Scrolls down by using next line.

int main(int argc, char *argv[]) {
	
	srand(time(0));
	
	struct card deck[NUMCARDS * NUMDECKS];
	struct card player[NUMHAND];
	struct card dealer[NUMHAND];
	
	int i;
	for(i = 0; i < NUMHAND; i++) {
		emptyCard(player, i);
		emptyCard(dealer, i);
	}
	
	scrolldown(2);
	
	fillDeck(deck);
	shuffleDeck(deck);
	printCards(deck, NUMCARDS * NUMDECKS);
	
	scrolldown(2);
	
	dealCard(deck, dealer);
	dealCard(deck, player);
	dealCard(deck, dealer);
	dealCard(deck, player);
	dealer[1].flipped = 1;
	
	printCards(deck, NUMCARDS * NUMDECKS);
	scrolldown(2);
	printCards(dealer, NUMHAND);
	scrolldown(1);
	printCards(player, NUMHAND);
	scrolldown(1);
	printf("Value of dealer's hand is: %d\nValue of player's hand is: %d\n", handValue(dealer), handValue(player));
	
	dealer[1].flipped = 0;
	
	scrolldown(2);
	printCards(dealer, NUMHAND);
	scrolldown(1);
	printCards(player, NUMHAND);
	scrolldown(1);
	printf("Value of dealer's hand is: %d\nValue of player's hand is: %d\n", handValue(dealer), handValue(player));
	
	char anyKey;
	scanf("%c", anyKey);
	
	return 0;
	
}

//Populate deck.
//Input: An array of card structures.
//Output: None. Array of cards is edited via pointers.
void fillDeck(struct card deck[]) {
	
	int temp;						//Temporary integer for populating suits.
	
	//For loop goes through the deck, populating.
	int i;
	for(i = 0; i < NUMCARDS * NUMDECKS; i++) {
		
		//Goes through every 13 cards and gives it the same suit. Temp's result will be 0 13 times, 1 13 times, 2 13 times, and 3 13 times. Then repeat.
		temp = (i / 13) % 4;
		switch(temp) {
			case 0:
				deck[i].suit = 'S';
				break;
			case 1:
				deck[i].suit = 'C';
				break;
			case 2:
				deck[i].suit = 'H';
				break;
			case 3:
				deck[i].suit = 'D';
				break;
		}
		
		//As we go through every suit we give it a value from 1 to 13. The switch-case assigns all the rank characters and changes values of Ace to 11 and face cards to 10.
		deck[i].value = (i % 13) + 1;
		switch(deck[i].value) {
			case 1:
				deck[i].value = 11;
				deck[i].rank[0] = ' ';
				deck[i].rank[1] = 'A';
				break;
			case 10:
				deck[i].rank[0] = '1';
				deck[i].rank[1] = '0';
				break;
			case 11:
				deck[i].value = 10;
				deck[i].rank[0] = ' ';
				deck[i].rank[1] = 'J';
				break;
			case 12:
				deck[i].value = 10;
				deck[i].rank[0] = ' ';
				deck[i].rank[1] = 'Q';
				break;
			case 13:
				deck[i].value = 10;
				deck[i].rank[0] = ' ';
				deck[i].rank[1] = 'K';
				break;
			default:
				deck[i].rank[0] = ' ';
				deck[i].rank[1] = deck[i].value + 48;		//The +48 makes the integer match its ASCII value so that the proper character shows.
		}
		
		deck[i].flipped = 0;								//All the cards are unflipped by default since the user won't see it, anyways. Will only be flipped for dealer's first card.
	}
}

//Shuffle deck.
//Input: An array of card structures.
//Output: None. Array of cards is edited via pointers.
void shuffleDeck(struct card deck[]) {
	
	//Temporary card structure to hold value while we switch. Total cards for random calculation. Two random integers.
	struct card temp;
	int totalCards = NUMCARDS * NUMDECKS;
	int rand1, rand2;
	
	//For loop that gets two random values and swaps those positions in the deck around.
	int i;
	for(i = 0; i < SHUFFLEVALUE; i++) {
		rand1 = rand() % totalCards;
		rand2 = rand() % totalCards;
		
		temp = deck[rand1];
		deck[rand1] = deck[rand2];
		deck[rand2] = temp;
	}
}

//Deals a card from the deck to a player.
//Inputs: Two arrays of card structures. First is the deck, second is the player.
//Output: Integer. 0 if deck is empty, 1 if successful, and -1 if we have an error.
int dealCard(struct card deck[], struct card hand[]) {
	
	struct card temp;
	
	//Goes through deck looking for first available card.
	int i;
	short bool = 0;
	for(i = 0; i < NUMCARDS * NUMDECKS; i++) {
		if(deck[i].suit != 'X') {
			temp = deck[i];
			emptyCard(deck, i);
			bool = 1;
			break;
		}
	}
	
	if(bool == 0) {
		return 0;			//Repopulation needed as deck is empty.
	}
	
	//Goes through hand looking for next open spot.
	for(i = 0; i < NUMHAND; i++) {
		if(hand[i].suit == 'X') {
			hand[i] = temp;
			return 1;		//Success!
		}
	}
	
	return -1;				//Error!
}

//Print out all cards. Used for hands and testing the deck.
//Inputs: An array of card structures. Integer with number of elements in array.
//Output: None. Array of cards is printed via pointers.
void printCards(struct card cards[], int numElements) {
	
	int i;
	for(i = 0; i < numElements; i++) {
		if(i % 13 == 0) {													//Keeps it to 13 cards per line. This won't affect the hands since they max out at 7 but will make deck-testing legible.
			printf("\n");
		}
		
		//If flipped, print X's. Otherwise, print out card's rank and suit.
		if(cards[i].flipped == 1) {
			printf("XXX ");
		}
		else {
			printf("%c%c%c ", cards[i].rank[0], cards[i].rank[1], cards[i].suit);
		}
	}
}

//Counts value of hand. Doesn't count facedown cards.
//Input: An array of card structures.
//Output: Integer. Value of hand.
int handValue(struct card hand[]) {
	
	int total = 0;
	
	//Count every card that's not facedown or empty.
	int i;
	for(i = 0; i < NUMHAND; i++) {
		if(hand[i].flipped == 0 && hand[i].suit != 'X') {
			total += hand[i].value;
		}
	}
	
	return total;
}

//Empties a card.
//Inputs: An array of card structures. Integer value of index.
//Output: None. Array of cards is edited via pointers.
void emptyCard(struct card deck[], int index) {
	
	deck[index].suit = 'X';
	deck[index].rank[0] = 'x';
	deck[index].rank[1] = 'x';
	deck[index].value = 0;
	deck[index].flipped = 0;
}

//Scrolls down by using next line.
//Input: Integer of how many lines we want to scroll. Will be changed to 1 if less than 1.
//Output: None.
void scrolldown(int lines) {
	
	int i;
	if(lines < 1) {lines = 1;}		//Make sure lines is at least 1. If not, make it so.
	for(i = 0; i < lines; i++) {
		printf("\n");
	}
}