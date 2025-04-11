// GOAL:  a text-based pong clone

// ball movement (ball is letter 'o')
// paddle movement / control


// CONSTANTS


#include <string.h>
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
	bool dirX;  // 1 is right
	bool dirY;	// 1 is up
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

void update_paddles(gamestate_t *obj, WINDOW *win) {
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
		break;
	}

	
}

void update_ball(gamestate_t *obj, WINDOW *win) {
	int step = 1;

	// ball movement
	if (obj->ballpos.dirX) { // if direction is right(1)
		obj->ballpos.x+=step;
	} else {
		obj->ballpos.x-=step;
	}

	if (obj->ballpos.dirY) { // if direction is up(1)
		obj->ballpos.y-=step;
	} else {
		obj->ballpos.y+=step;
	}

	wmove(win, 2, 2);
}


// draw - output to screen

int main(int argc, char **argv) {
	
	// window/screen buffer initialization

	initscr(); // Initialize ncurses
	cbreak();
	nodelay(stdscr, TRUE);
	noecho();
	curs_set(0);

	char player = '|';
	char ball = 'o';

    // Attempt to resize the terminal window
    // if (resizeterm(rows, cols) == OK) {
    //     // Resizing was successful, or the terminal was already the requested size
    //     printw("Terminal resized to %d rows and %d columns.\n", rows, cols);
    // } else {
    //     printw("Failed to resize terminal.\n");
    // }

    // wresize(stdscr, rows, cols);


    int yMax, xMax;

    
    getmaxyx(stdscr, yMax, xMax);
    int nlines = 20,
    	ncols = 50,
    	beginY = yMax/2 - 10,
    	beginX = 10,
    	Lchannel = 1,
    	Rchannel = ncols - 2;

    // game object initialization
	gamestate_t thisgame = {.lpadpos = nlines / 2,
							.rpadpos = nlines / 2,
							.ballpos = {.x = ncols / 2, .y = nlines / 2, .dirX = 1, .dirY = 1}, 
							.game = 1
						   };

	

    WINDOW *playwin = newwin(nlines, ncols, beginY, beginX);
    box(playwin, 0, 0);


    // draw paddles in init positions
    mvwaddch(playwin, thisgame.rpadpos, Rchannel, player);
    mvwaddch(playwin, thisgame.lpadpos, Lchannel, player);
    mvwaddch(playwin, thisgame.rpadpos+1, Rchannel, player);
    mvwaddch(playwin, thisgame.lpadpos-1, Lchannel, player);
    mvwaddch(playwin, thisgame.rpadpos-1, Rchannel, player);
    mvwaddch(playwin, thisgame.lpadpos+1, Lchannel, player);

    // ball init pos
    mvwaddch(playwin, thisgame.ballpos.y, thisgame.ballpos.x, ball);

    

	int timer = 0;
	while(1) {
		if (thisgame.game == 0) {

		}
		
		int prevl = thisgame.lpadpos;
		int prevr = thisgame.rpadpos;
		int prevBx = thisgame.ballpos.x;
		int prevBy = thisgame.ballpos.y;

		update_paddles(&thisgame, playwin);  // update game state variables
		// update_ball(&thisgame, playwin);
		if (timer == 200000) {
			update_ball(&thisgame, playwin);
			//timer = 0;
		}
		
		// shorten the typing while getting updated paddle positions
		int *newl = &thisgame.lpadpos;
		int *newr = &thisgame.rpadpos;

		// paddles collisions with walls
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

		// ball movement and collision with paddles and top and bottom walls

		// pseudocode
		// if ballpos  = x(==Rchannel or Lchannel) && y(lpadpos +/- 1 || rpadpos +/- 1)
		// 		flip xdir
		// if ballpos = 0 || nlines
		//		flip ydir
		
		// collision: ball and top and bottom walls
		if (thisgame.ballpos.y >= nlines - 2) {
			thisgame.ballpos.dirY = 1;
			thisgame.ballpos.y = nlines - 2;
		}
		if (thisgame.ballpos.y <= 1) {
			thisgame.ballpos.dirY = 0;
			thisgame.ballpos.y = 1;
		}

		// collision: ball with paddles
		if (thisgame.ballpos.x <= Lchannel+1 && (thisgame.ballpos.y == thisgame.lpadpos \
			|| thisgame.ballpos.y == thisgame.lpadpos + 1 || thisgame.ballpos.y == thisgame.lpadpos - 1)) {
			thisgame.ballpos.dirX = 1;
			//thisgame.ballpos.x += 1;
		} else if (thisgame.ballpos.x <= Lchannel) {
			mvwaddstr(playwin, nlines / 2, (ncols / 2) - 5, "Game Over!");
			mvwaddstr(playwin, (nlines / 2) + 1, (ncols / 2) - 9, "Press any key to quit");
			wrefresh(playwin);
			break;
		}
		if (thisgame.ballpos.x >= Rchannel-1 && (thisgame.ballpos.y == thisgame.rpadpos \
			|| thisgame.ballpos.y == thisgame.rpadpos + 1 || thisgame.ballpos.y == thisgame.rpadpos - 1)) {
			thisgame.ballpos.dirX = 0;
			//thisgame.ballpos.x -= 1;
		} else if (thisgame.ballpos.x >= Rchannel) {
			mvwaddstr(playwin, nlines / 2, (ncols / 2) - 5, "Game Over!");
			mvwaddstr(playwin, (nlines / 2) + 1, (ncols / 2) - 10, "Press any key to quit");
			wrefresh(playwin);
			break;
		}
		
		if (timer == 200000) {
			mvwaddch(playwin, thisgame.ballpos.y, thisgame.ballpos.x, ball);
			mvwaddch(playwin, prevBy, prevBx, ' ');
			timer = 0;
		}

				

		// left: erase char in previous pos before drawing new pos
		if (*newl > prevl) {
			mvwaddch(playwin, *newl - 2, Lchannel, ' ');
		} else if (*newl < prevl) {
			mvwaddch(playwin, *newl + 2, Lchannel, ' ');
		}	


		// right: erase char in previous pos before drawing new pos
		if (*newr > prevr) {
			mvwaddch(playwin, *newr - 2, Rchannel, ' ');
		} else if (*newr < prevr) {
			mvwaddch(playwin, *newr + 2, Rchannel, ' ');
		}		
		
		// draw paddles in new pos
		mvwaddch(playwin, *newl, Lchannel, player);
    	mvwaddch(playwin, *newl+1, Lchannel, player);
    	mvwaddch(playwin, *newl-1, Lchannel, player);
		mvwaddch(playwin, *newr, Rchannel, player);
		mvwaddch(playwin, *newr+1, Rchannel, player);
    	mvwaddch(playwin, *newr-1, Rchannel, player);


    	//refresh();
		wrefresh(playwin);
		usleep(10000);
		timer+=10000;
	}
	nodelay(stdscr, FALSE);
	getch();	
	endwin();
	return 0;
}


/* 
main game logic:
	get user input & update game state
	redraw screen
	
*/ 