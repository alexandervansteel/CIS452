import curses

stdscr=curses.initscr()
key=''

curses.noecho()
curses.cbreak()

stdscr.addstr(1,1,'press esc to quit')

while key != 27: # esc key
    key=stdscr.getch()
    stdscr.addstr(3,3, 'key pressed')
    stdscr.addch(5,5, key)
    stdscr.refresh()

curses.echo()
curses.nocbreak()
curses.endwin()
