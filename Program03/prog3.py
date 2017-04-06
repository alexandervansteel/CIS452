# Alexander Vansteel
# CIS 452
# Whack-a-Mole
# Python version with Multiprocessing

import curses, random, signal, string, sys, time, threading


SIGINT=1


def mole_t(t_num,pop_dur,hide_dur,grid_w,grid_h):
    global semaphore
    global screen
    global char_list
    global coord_list

    while SIGINT:
        time.sleep(random.randrange(0,hide_dur))
        semaphore.acquire()
        char=' '
        while char in char_list: char=random.choice(string.ascii_lowercase)
        y=(random.randrange(0,grid_h)*2)+1
        x=(random.randrange(0,grid_w)*4)+2
        screen.addstr(y,x,char)
        char_list[t_num]=char
        coord_list[t_num]=(y,x)
        time.sleep(random.randrange(pop_dur))
        screen.addstr(y,x,' ')
        char_list[t_num]=' '
        coord_list[t_num]=' '
        semaphore.release()

    time.sleep(6)
    print('Thread '+str(t_num+1)+' ending')


def board_refresh():
    while SIGINT:
        screen.refresh()
        time.sleep(0.01)


def create_board(grid_h, grid_w):
   global screen
   str1 = '+---'
   str2 = '|   '
   top_bottom= ''
   side = ''

   for w in range(grid_w):
       top_bottom += str1
       side += str2

   top_bottom += '+'
   side += '|'

   for h in range(grid_h):
       screen.addstr(h*2, 0, top_bottom)
       screen.addstr((h*2)+1, 0, side)
       screen.addstr((h*2)+2, 0, top_bottom)


def main(stdscr,grid_w,grid_h,moles,max_moles,pop_dur,hide_dur):
    global SIGINT

    global screen
    screen=stdscr

    global semaphore
    semaphore=threading.BoundedSemaphore(max_moles)

    global key
    key=' '

    screen.clear()
    screen.addstr(2*grid_h+2,0,'Welcome to Whack-a-Mole! (Press esc to exit)')
    screen.refresh()
    curses.curs_set(0)

    create_board(grid_h,grid_w)

    global char_list
    char_list=[' ']*moles
    global coord_list
    coord_list=[' ']*moles

    try:
        for t_num in range(moles):
            t=threading.Thread(target=mole_t,args=(t_num,pop_dur,hide_dur,grid_w,grid_h,))
            t.start()
    except:
        raise Exception("Error: Failed to start threads.")

    t=threading.Thread(target=board_refresh,args=())
    t.start()

    score=0
    misses=0
    while key != 27: # esc char
        key=screen.getch()
        screen.addstr(2*grid_h+3,0,'Key Pressed: '+chr(key))

        if chr(key) in char_list:
            index=char_list.index(chr(key))
            score=score+1
            screen.addstr(2*grid_h+4,0,'Score: '+str(score))
            screen.addstr(int(coord_list[index][0]),int(coord_list[index][1]),' ')
            char_list[index]=' '
            coord_list[index]=' '
        else:
            misses+=1
            screen.addstr(2*grid_h+5,0,'Misses: '+str(misses))

        if misses > 9:
            SIGINT=0
            curses.flash()
            curses.flushinp()
            screen.clear()
            screen.refresh()
            screen.addstr(0,0,'Oh no! You have missed the mole too many times and lost the game...',curses.A_BLINK)
            screen.addstr(1,0,'Closing game in 5 sec...',curses.A_BLINK)
            screen.refresh()
            time.sleep(5)
            key=27

    SIGINT=0
    curses.curs_set(2)


if __name__=='__main__':
    grid_w=int(sys.argv[1])
    grid_h=int(sys.argv[2])
    moles=int(sys.argv[3])
    max_moles=int(sys.argv[4])
    pop_dur=int(sys.argv[5])
    hide_dur=int(sys.argv[6])

    if (len(sys.argv) < 7):
        print('Incorrect number of arguments')
        print('Usage: '+sys.argv[0]+' [width] [height] [number of moles] [max moles] [pop duration(ms)] [hide duration(ms)]')
        sys.exit()

    try:
        curses.wrapper(main,grid_w,grid_h,moles,max_moles,pop_dur,hide_dur)
    except KeyboardInterrupt:
        print('Received KeyboardInterrupt Exception: Exiting...')
        sys.exit()
