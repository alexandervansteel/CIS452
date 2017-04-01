# Alexander Vansteel
# CIS 452
# Whack-a-Mole
# Python version with Multiprocessing

import multiprocessing as mp
import curses

# def Mole(line,column):

    # while True:
    #     curses.delay_output(hide_dur) # output delay in ms
    #     stdscr.addstr(line, column, "a")


def GameSpace(grid_width,grid_height,max_moles):

    # for line in range (0, grid_height):
    #     for column in range (0. grid_width):
    #         #spawn moles
    #         p=Process(target=Mole,args=(line,column,))
    #         p.start()
    #         p.join()

    stdscr=curses.initscr()
    key=''

    curses.noecho()
    curses.cbreak()

    int msg=grid_height+1

    int key_msg=grid_height+2
    stdscr.addstr(msg,0,'Welcome to Whack-a-Mole! (Press esc to quit)')

    while key != 27: # esc key
        key=stdscr.getch()
        stdscr.addstr(key_msg,0,'key pressed %c' % key)
        stdscr.refresh()

    curses.echo()
    curses.nocbreak()
    curses.endwin()

if __name__=='__main__':
    grid_width=input('Enter Grid Width: ')
    grid_height=input('Enter Grid Hight: ')
    max_moles=input('Enter Maximum Number of Moles: ')
    pop_dur=input('Enter Pop Duration in ms: ')
    hide_dur=input('Enter Hide Duration in ms: ')

    # grid_width=(sys.argv[1])
    # grid_height=(sys.argv[2])
    # max_moles=(sys.argv[3])
    # global pop_dur=(sys.argv[4])
    # global hide_dur=(sys.argv[5])

    GameSpace(grid_width,grid_height,max_moles)
