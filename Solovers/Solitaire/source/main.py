import win32
import recognizer
import sys
import time

columns = [
           (145, 480, 30, 113), 
           (145, 480, 139, 222), 
           (145, 480, 245, 328), 
           (145, 480, 353, 436), 
           (145, 480, 461, 544), 
           (145, 480, 575, 658), 
           (145, 480, 679, 766), 
           (20,116,145,300), 
           (20, 116, 361, 432), 
           (20, 116, 469, 540), 
           (20, 116, 577, 648), 
           (20, 116, 684, 775)
]

pos_next_cards = (80, 72)

window = win32.getOrCreateWindow('Solitaire', 'c:\Program Files\Microsoft Games\Solitaire\Solitaire.exe', 'unknow_states/screen_temp.bmp')
if window == None:
    print 'Solitaire window not found'
    sys.exit()
time.sleep(3)

def nextstep():
    global window, columns, pos_next_cards
    screen = window.getFramebuffer()
    #screen1 = screen.copy()
    screen1 = None
    cards = recognizer.getCurrentScreenState(screen, columns, screen1)
    index = 0
    # felpakol
    for col0 in cards[0:7]:
        for card0 in col0:
            if card0[2] == 0:
                for col1 in cards[8:12]:
                    for card1 in col1:
                        if card0[0][2] == card1[0][2] and card0[0][1] == card1[0][1] and (card0[0][3] - card1[0][3]) == 1:
                            window.click(((card0[1][0] + (card0[1][2] / 4)), (card0[1][1] + (card0[1][3] / 4))), 'left', 2)
                            print 'a'
                            return 1
    # csoportos atrendezes
    for i0 in range(7):
        col0 = cards[i0]
        if len(col0) <= 1:
            continue
        card0 = col0[0]
        if card0 == None:
            continue
        for i1 in range(7):
            if i1 == i0:
                continue
            col1 = cards[i1]
            for card1 in col1:
                if card1[2] == 0 and (card0[0][2] != card1[0][2]) and (card1[0][3] - card0[0][3]) == 1:
                    window.drag(((card0[1][0] + (card0[1][2] / 4)), (card0[1][1] + (card0[1][3] / 4))), ((card1[1][0] + (card1[1][2] / 4)), (card1[1][1] + (card1[1][3] / 4))))
                    print 'f'
                    return 1
    # kiraly es asz lapok atrendezes
    for col0 in cards[0:7]:
        for card0 in col0:
            if card0[0][3] == 1:
                window.click(((card0[1][0] + (card0[1][2] / 4)), (card0[1][1] + (card0[1][3] / 4))), 'left', 2)
                print 'h'
                return 1
            if card0[0][3] == 13 and card0[1][1] >= 150:
                index = 0
                for col12 in cards[0:7]:
                    if len(col12) == 0:
                        coords = columns[index]
                        window.drag(((card0[1][0] + (card0[1][2] / 4)), (card0[1][1] + (card0[1][3] / 4))), (coords[2], coords[0]))
                        print 'g'
                        return 1
                    index = index + 1
    # lepakolt lapok atrendezese
    for i0 in range(7):
        col0 = cards[i0]
        if len(col0) != 1:
            continue
        for card0 in col0:
            if card0[2] == 0:
                for i1 in range(7):
                    if (i1 == i0):
                        continue
                    col1 = cards[i1]
                    for card1 in col1:
                        if card1[2] == 0 and (card0[0][2] != card1[0][2]) and (card1[0][3] - card0[0][3]) == 1:
                            window.drag(((card0[1][0] + (card0[1][2] / 4)), (card0[1][1] + (card0[1][3] / 4))), ((card1[1][0] + (card1[1][2] / 4)), (card1[1][1] + (card1[1][3] / 4))))
                            print 'j'
                            return 1
    # tegyunk le egy lapot, hogy attudjuk tenni #1
    for i0 in range(7):
        col0 = cards[i0]
        if len(col0) != 1:
            continue
        for card0 in col0:
            if card0[2] == 0 and card0[1][1] >= 150:
                for col1 in cards[8:12]:
                    for card1 in col1:
                        if card0[0][2] != card1[0][2] and (card1[0][3] - card0[0][3]) == 1:
                            for i2 in range(7):
                                col2 = cards[i2]
                                for card2 in col2:
                                    if card2[2] == 0 and (card1[0][2] != card2[0][2]) and (card2[0][3] - card1[0][3]) == 1:
                                        window.drag(((card1[1][0] + (card1[1][2] / 4)), (card1[1][1] + (card1[1][3] / 4))), ((card2[1][0] + (card2[1][2] / 4)), (card2[1][1] + (card2[1][3] / 4))))
                                        window.drag(((card0[1][0] + (card0[1][2] / 4)), (card0[1][1] + (card0[1][3] / 4))), ((card2[1][0] + (card2[1][2] / 4)), (card2[1][1] + (card2[1][3] / 4))))
                                        print 'm'
                                        return 1
    # tegyunk at egy lapot, hogy feltudjuk tenni #1
    for i0 in range(7):
        col0 = cards[i0]
        col0len = len(col0)
        if col0len > 1:
            card0 = col0[col0len - 1]
            card1 = col0[col0len - 2]
            for i1 in range(7):
                if i0 == i1:
                    continue
                col1 = cards[i1]
                for card2 in col1:
                    if card2[2] == 0 and (card0[0][2] != card2[0][2]) and (card2[0][3] - card0[0][3]) == 1:
                        for col2 in cards[8:12]:
                            for card3 in col2:
                                if card3[0][2] == card1[0][2] and card3[0][1] == card1[0][1] and (card1[0][3] - card3[0][3]) == 1:
                                    window.drag(((card0[1][0] + (card0[1][2] / 4)), (card0[1][1] + (card0[1][3] / 4))), ((card2[1][0] + (card2[1][2] / 4)), (card2[1][1] + (card2[1][3] / 4))))
                                    window.click(((card0[1][0] + (card0[1][2] / 4)), (card0[1][1] + (card0[1][3] / 4))), 'left', 2)
                                    print 'n'
                                    return 1
    # osztott lapokat elpakolja
    for card0 in cards[7]:
        if card0[2] == 0:
            if card0[0][3] == 1:
                window.click(((card0[1][0] + (card0[1][2] / 4)), (card0[1][1] + (card0[1][3] / 4))), 'left', 2)
                print 'b'
                return 1
            index = 0
            if card0[0][3] == 13:
                for col12 in cards[0:7]:
                    if len(col12) == 0:
                        coords = columns[index]
                        window.drag(((card0[1][0] + (card0[1][2] / 4)), (card0[1][1] + (card0[1][3] / 4))), (coords[2], coords[0]))
                        print 'c'
                        return 1
                    index = index + 1
            for col1 in cards[8:12]:
                for card1 in col1:
                    if card0[0][2] == card1[0][2] and card0[0][1] == card1[0][1] and (card0[0][3] - card1[0][3]) == 1:
                        window.click(((card0[1][0] + (card0[1][2] / 4)), (card0[1][1] + (card0[1][3] / 4))), 'left', 2)
                        print 'd'
                        return 1
            for col12 in cards[0:7]:
               for card1 in col12:
                   if card1[2] == 0 and (card0[0][2] != card1[0][2]) and (card1[0][3] - card0[0][3]) == 1:
                       window.drag(((card0[1][0] + (card0[1][2] / 4)), (card0[1][1] + (card0[1][3] / 4))), ((card1[1][0] + (card1[1][2] / 4)), (card1[1][1] + (card1[1][3] / 4))))
                       print 'e'
                       return 1
    # osztott lapokat elpakolja
    for card0 in cards[7]:
        if card0[2] == 0:
            for col1 in cards[8:12]:
                for card1 in col1:
                    if card0[0][2] != card1[0][2] and (card1[0][3] - card0[0][3]) == 1:
                        for col2 in cards[0:7]:
                            for card2 in col2:
                                if card2[2] == 0 and (card1[0][2] != card2[0][2]) and (card2[0][3] - card1[0][3]) == 1:
                                    window.drag(((card1[1][0] + (card1[1][2] / 4)), (card1[1][1] + (card1[1][3] / 4))), ((card2[1][0] + (card2[1][2] / 4)), (card2[1][1] + (card2[1][3] / 4))))
                                    window.drag(((card0[1][0] + (card0[1][2] / 4)), (card0[1][1] + (card0[1][3] / 4))), ((card2[1][0] + (card2[1][2] / 4)), (card2[1][1] + (card2[1][3] / 4))))
                                    print 'o'
                                    return 1
    window.click(pos_next_cards)
    return 2

state = window.keyState(145)
nothingdo = 0
while 1:
    if window.isDisabled():
        nothingdo = 0
        time.sleep(1)
        if window.keyState(145) != state:
            print "User cancelled"
            break
        window.clickDialogButton("[CLASS:Button; INSTANCE:1]", (10, 10))
        print "Play Again"
        time.sleep(1)
        continue
    action = nextstep()
    if action == 0:
        break
    if action == 2:
       nothingdo = nothingdo + 1
    else:
       nothingdo = 0
    if nothingdo > 18:
        break
    if window.keyState(145) != state:
        break
    time.sleep(0.3)