# Alexander Vansteel
# CIS 452
# Whack-a-Mole
# Python version with Multiprocessing

import curses, random, signal, string, sys, time, threading

def main(stdscr,grid_w,grid_h,moles,max_moles,pop_dur,hide_dur):
    global screen
    screen=stdscr

    global semaphore
    semaphore=threading.BoundedSemaphore(max_moles)

    # set up board
    screen.clear()


if __name__=='__main__':
    # grid_width=int(input('Enter Grid Width: '))
    # grid_height=int(input('Enter Grid Hight: '))
    # max_moles=int(input('Enter Maximum Number of Moles: '))
    # pop_dur=int(input('Enter Pop Duration in ms: '))
    # hide_dur=int(input('Enter Hide Duration in ms: '))

	grid_w=int(sys.argv[1])
	grid_h=int(sys.argv[2])
	moles=int(sys.argv[3])
	max_moles=int(sys.argv[4])
	pop_dur=int(sys.argv[5])
	hide_dur=int(sys.argv[6])

    if len(sys.argv)<7:
		print('Incorrect number of arguments')
		print('Usage: '+sys.argv[0]+' [width] [height] [number of moles] [max moles] '+
			'[pop duration(ms)] [hide duration(ms)]')
		sys.exit()

    if (grid_w*grid_h)<3 or (grid_w*grid_h)<49:
        print('Incorrect grid size: Must be between 3x1 and 7x7')

    try:
        curses.wrapper(main,grid_w,grid_h,moles,max_moles,pop_dur,hide_dur)
    except KeyboardInterrupt:
        print('Received KeyboardInterrupt Exception: Exiting...')
        sys.exit()
