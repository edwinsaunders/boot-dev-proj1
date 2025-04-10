// GOAL:  a text-based pong clone

// ball movement (ball is letter 'o')
// paddle movement / control


// CONSTANTS



#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct Paddle {
	int x;
	int y;
} paddle_t;

typedef struct Ball {
	int x;
	int y;
} ball_t;


typedef struct GameState {
	int lpadpos;
	int rpadpos;
	ball_t ballpos;
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
		obj->lpadpos--;
		break;
	case 's':
		obj->lpadpos++;
		break;
	case 'i':
		obj->rpadpos--;
		break;
	case 'k':
		obj->rpadpos++;
		break;
	case 't':
		endwin();
		exit(1);
	default:
		return;
	}
}


// draw - output to screen

int main(int argc, char **argv) {
	
	// window/screen buffer initialization

	initscr(); // Initialize ncurses
	cbreak();
	//nodelay(stdscr, TRUE);
	noecho();

	

	char player = 'o';

	int rows = 41;
    int cols = 81;
    int coloffset = 2;


    // Attempt to resize the terminal window
    // if (resizeterm(rows, cols) == OK) {
    //     // Resizing was successful, or the terminal was already the requested size
    //     printw("Terminal resized to %d rows and %d columns.\n", rows, cols);
    // } else {
    //     printw("Failed to resize terminal.\n");
    // }

    //wresize(stdscr, rows, cols);

    //box(stdscr, 0, 0);

    int yMax, xMax;

    
    getmaxyx(stdscr, yMax, xMax);
    int nlines = 20,
    	ncols = 50,
    	beginY = yMax/2 - 10,
    	beginX = 10,
    	Lchannel = 1,
    	Rchannel = ncols - 2;

    // game object initialization
	gamestate_t thisgame = {.lpadpos = beginY + (nlines / 2),
							.rpadpos = beginY + (nlines / 2),
							.ballpos = {.x = 0, .y = 0}, 
							.game = 1
						   };

	

    WINDOW *playwin = newwin(nlines, ncols, beginY, beginX);
    box(playwin, 0, 0);


    // draw paddles in init positions
    mvwaddch(playwin, thisgame.lpadpos, Lchannel, player);
    mvwaddch(playwin, thisgame.rpadpos, Rchannel, player);
	refresh();
    wrefresh(playwin);

	while(1) {
				
		int prevl = thisgame.lpadpos;
		int prevr = thisgame.rpadpos;

		update(&thisgame);  // update game state variables
		
		int *newl = &thisgame.lpadpos;
		int *newr = &thisgame.rpadpos;

		// check if paddles and ball are in bounds after
		// 	update
		if (*newl >= nlines - 1) {
			*newl = nlines - 2;
		}

		if (*newl <= 0) {
			*newl = 1;
		}

		if (*newr >= nlines - 1) {
			*newr = nlines - 2;
		}

		if (*newr <= 0) {
			*newr = 1;
		}


		

		// left: erase char in previous pos before drawing new pos
		if (*newl > prevl) {
			mvwaddch(playwin, *newl - 1, Lchannel, ' ');
		} else if (*newl < prevl) {
			mvwaddch(playwin, *newl + 1, Lchannel, ' ');
		}	

		//mvwaddch(playwin, prevl, Lchannel, ' ');
		//mvwaddch(playwin, prevr, Rchannel, ' ');

		// right: erase char in previous pos before drawing new pos
		if (*newr > prevr) {
			mvwaddch(playwin, *newr - 1, Rchannel, ' ');
		} else if (*newr < prevr) {
			mvwaddch(playwin, *newr + 1, Rchannel, ' ');
		}		
		
		// draw paddles in new pos and erase prev pos
		mvwaddch(playwin, *newl, Lchannel, player);
		mvwaddch(playwin, *newr, Rchannel, player);
		// mvwaddch(playwin, prevl, Lchannel, ' ');
		// mvwaddch(playwin, prevr, Rchannel, ' ');


		// printf("%d\n", cols - coloffset);

		wrefresh(playwin);
		//refresh();
		// box(stdscr, rows, cols);
	}
	endwin();
	return 0;
}


/* 
main game logic:
	get user input & update game state
	redraw screen
	
*/ 