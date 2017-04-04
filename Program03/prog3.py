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
        y=random.randrange(0,grid_h)
        x=random.randrange(0,grid_w)
        screen.addstr(y,x,char)
        char_list[t_num]=char
        coord_list[t_num]=(y,x)
        time.sleep(random.randrange(pop_dur))
        screen.addstr(y,x,' ')
        char_list[t_num]=' '
        coord_list[t_num]=' '
        semaphore.release()

    print('Thread '+str(t_num+1)+' ending')


def board_refresh():
    while SIGINT:
        screen.refresh()
        time.sleep(0.01)


def main(stdscr,grid_w,grid_h,moles,max_moles,pop_dur,hide_dur):
    global screen
    screen=stdscr

    global semaphore
    semaphore=threading.BoundedSemaphore(max_moles)

    # signal.signal(signal.SIGINT,signal_handler)

    global key
    key=' '

    screen.clear()
    screen.addstr(grid_h+1,0,'Welcome to Whack-a-Mole! (Press esc to exit)')
    screen.refresh()
    curses.curs_set(0)

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
    while key != 27: # esc char
        key=screen.getch()
        screen.addstr(grid_h+2,0,'Key Pressed: '+chr(key))

        # if chr(key) in char_list:
        #     score=score+1
        #     screen.addstr(grid_h+3,0,'Score: '+str(score))
        #     try:
        #         char_list=[i.replace(key,' ') for i in char_list]
        #     except:
        #         pass

        if chr(key) in char_list:
            index=char_list.index(chr(key))
            score=score+1
            screen.addstr(grid_h+3,0,'Score: '+str(score))
            screen.addstr(int(coord_list[index][0]),int(coord_list[index][1]),' ')
            char_list[index]=' '
            coord_list[index]=' '

    global SIGINT
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
