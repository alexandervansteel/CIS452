# Alexander Vansteel
# CIS 452
# Whack-a-Mole
# Python version with Multiprocessing

from multiprocessing import Process, Lock, Value
from ctypes import c_char_p
import curses


def mole(line,column):
    stdscr.addstr(line,column,'a')
#     while True:
#         #curses.delay_output(hide_dur) # output delay in ms
#         stdscr.addstr(line, column, "a")

if __name__=='__main__':
    grid_width=int(input('Enter Grid Width: '))
    grid_height=int(input('Enter Grid Hight: '))
    max_moles=int(input('Enter Maximum Number of Moles: '))
    pop_dur=int(input('Enter Pop Duration in ms: '))
    hide_dur=int(input('Enter Hide Duration in ms: '))

    # this should fucking work. but it doesn't i can't figure it out
    #key=Value('c','a') #shared memory value for user input
    key=' '
    score=Value('i',0) #shared memory value for the score
    lock=Lock()

    stdscr=curses.initscr()
    curses.noecho()
    curses.cbreak()

    # spawn moles
    moles=[]
    for line in range (0, grid_height):
        for column in range (0, grid_width):
            p=Process(target=mole,args=(line,column))
            moles.append(p)
            p.start()

    stdscr.addstr(grid_height+1,0,'Welcome to Whack-a-Mole! (Press esc to quit)')

    while key != 27: # esc key
        key=stdscr.getch()
        stdscr.addstr(grid_height+2,0,'key pressed %c' % key)

    # closes processes
    for m in moles: m.terminate()

    curses.echo()
    curses.nocbreak()
    curses.endwin()
