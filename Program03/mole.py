import sys, time, threading, random, signal
import curses
from curses import wrapper

MOLEONBOARD=[]
THREAD_LIST=[]
RAND_LINE=[]
RAND_COL=[]
SIGINT = 1
GAMESTART = 1

def main(stdscr):
	global MAX_MOLE
	global semaphore
	global BOARD

	BOARD = stdscr
	signal.signal(signal.SIGINT, signal_handler)
	semaphore = threading.BoundedSemaphore(MAX_MOLE)
	createBoard()
	createThreads()
	gameEngine()

def gameEngine():
	global LINE
	global GAMESTART
	global MOLEONBOARD
	global BOARD

	molesHit = 0

	while GAMESTART:
		read = BOARD.getch()
		if read == ord('s'):
			BOARD.addstr(5+(LINE*3),0,"Moles hit:              ")
			BOARD.addstr(5+(LINE*3),12,str(molesHit))
			GAMESTART = 0

	while True:
		read = BOARD.getch()
		if chr(read) in MOLEONBOARD:
			molesHit=molesHit+1
			BOARD.addstr(5+(LINE*3),12,str(molesHit))
			try:
				MOLEONBOARD.remove(chr(read))
			except:
				pass

def boardRefresh():
	while GAMESTART:
		time.sleep(0.1)
	while SIGINT:
		BOARD.refresh()
		time.sleep(0.01)

def waitingRoom(moleChar, threadNum):
	global UP_DURA
	global SIGINT

	while(GAMESTART):
		time.sleep(.1)

	while(SIGINT):
		time.sleep(random.randrange(0,DOWN_DURA))
		popUp(moleChar, threadNum)

	print("Thread ["+str(threadNum+1)+"] ending")

def popUp(moleChar, threadNum):
	global semaphore
	global UP_DURA
	global MOLEONBOARD
	global BOARD
	global LINE
	global COL
	global RAND_LINE
	global RAND_COL

	semaphore.acquire()
	RAND_LINE[threadNum] = random.randrange(0,LINE)
	RAND_COL[threadNum] = random.randrange(0,COL)
	BOARD.addch(3+(RAND_LINE[threadNum]*3),3+(RAND_COL[threadNum]*6),moleChar)
	MOLEONBOARD.append(moleChar)
	time.sleep(random.randrange(0,UP_DURA))
	try:
		MOLEONBOARD.remove(moleChar)
	except:
		pass
	BOARD.addch(3+(RAND_LINE[threadNum]*3),3+(RAND_COL[threadNum]*6),' ')
	semaphore.release()

def createThreads():
	global THREADS

	moleChar = [
		"a","b","c","d","e",
		"f","g","h","i","j",
		"k","l","m","n","o",
		"p","q","r","s","t",
		"u","v","w","x","y",
		"z","0","1","2","3",
		"4","5","6","7","8","9"]
	try:
		for x in range(THREADS):
			t = threading.Thread(target=waitingRoom, args=(moleChar[x],x,))
			THREAD_LIST.append(t)
			t.start()
	except:
		raise Exception("Error: unable to start threads")

	t = threading.Thread(target=boardRefresh, args=())
	t.start()

def createBoard():
	global LINE
	global COL
	global BOARD

	BOARD.clear()
	for i in range(LINE+1):
		RAND_LINE.append(0)
	for j in range(COL+1):
		RAND_COL.append(0)
	BOARD.addstr(3+(LINE*3),0,"Hit the keys [a-z] or [0-1] to hit the moles! (Press Ctrl-C to quit)")
	BOARD.addstr(5+(LINE*3),0,"Press the s key to start")
	BOARD.refresh()

def signal_handler(signal, frame):
	global SIGINT
	SIGINT = 0
	GAMESTART = 0
	sys.exit()

#Startup Code
if __name__ == "__main__":
	global LINE
	global COL
	global THREADS
	global MAX_MOLE
	global UP_DURA
	global DOWN_DURA

	#Checks arg amount
	if len(sys.argv) < 7:
		print("Incorrect number of arguments")
		print("Usage: "+sys.argv[0]+" [height] [width] [number of moles] [max #of moles] "+
			"[mole pop-up duration(milisec)] [mole hide duration(milisec)]")
		sys.exit()

	#Sets global and checks for special conditions
	LINE = int(sys.argv[1])
	COL = int(sys.argv[2])
	THREADS = int(sys.argv[3])
	MAX_MOLE = int(sys.argv[4])
	UP_DURA = int(sys.argv[5])
	DOWN_DURA = int(sys.argv[6])

	#Checks to make sure max board size <= 36
	if (LINE*COL) < 4 or (LINE*COL) > 36:
		print("Boardsize is incorrect, boardsize(height*width) must be: >= 4 and <= 36")
		sys.exit()

	#Checks to make sure max number of moles doesnt exceed the max boardsize
	if MAX_MOLE > (LINE*COL):
		print("Max number of moles is larger then the boardsize")
		sys.exit()

	wrapper(main)	
