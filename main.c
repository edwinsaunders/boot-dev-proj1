// GOAL:  a text-based pong clone

// ball movement (ball is letter 'o')
// paddle movement / control


// CONSTANTS



#include <ncurses.h>
#include <unistd.h>

typedef struct GameState {
	int lpadpos;
	int rpadpos;
	int ballpos;
	bool game;
} gamestate_t;


// update - handle game logic
//	take input from user, update game state variables
//	game state variables:  lpaddlepos, rpaddlepos, 
//		ballpos, gamestate

void update(gamestate_t *obj) {
	int ch = getch(); //get input

	switch (ch) {
	case 'w':
		obj->lpadpos++;
		break;
	case 's':
		obj->lpadpos--;
		break;
	default:
		return;
	}
}


// draw - output to screen

int main(int argc, char **argv) {

	initscr(); // Initialize ncurses
	cbreak();
	nodelay(stdscr, TRUE);
	noecho();

	gamestate_t thisgame = {.lpadpos = 0, .rpadpos = 0, .ballpos = 0, .game = 1};

	int timer = 10;
	int newX = 0;
	int newY = 0;
	char player = 'o';
	
	while(timer) {
		
		addch(player);
		//thisgame.lpadpos = 65;
		
		// addch(thisgame.lpadpos);
		// move(newX, newY);
		// addch('o');
		// refresh();
		
		// usleep(500000);

		

		// printw("%d\n", LINES);
		// printw("%d", COLS);
		int prev = thisgame.lpadpos;
		update(&thisgame);
		if (thisgame.lpadpos > prev) {
			move(0, thisgame.lpadpos - 1);
		}
		//} else if (thisgame.lpadpos < prev) {
			//move(0, thisgame.lpadpos + 1);
		//}
		
		addch(' ');
		
		move(0, thisgame.lpadpos);
		addch(player);
		// refresh();
		// usleep(500000);


		//print 'o' in different places with an impulse 
		//at 45 degrees or so
		
		// move (20, 20);
		// refresh();
	    // usleep(1000000);

	    // refresh(); // Update the screen

	    //move(3, 3);
	    //int x = getch(); // Wait for user input
	    //printw("%d", x);
	    //if (x == 'q') {
	    //	endwin(); // End ncurses mode
	    //	return 0;
	    //}
	    // timer--;
	}
	endwin();
	return 0;
}