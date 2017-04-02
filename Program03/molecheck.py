# import curses
from multiprocessing import Process, Value

def mole(column,line):
    #stdscr.addstr(column,line,'a')
    print(column, line)

if __name__=='__main__':

    moles=[]
    for line in range(0,4):
        for column in range(0,4):
            p=Process(target=mole,args=(line,column))
            moles.append(p)
            p.start()


    for m in moles: m.terminate()
