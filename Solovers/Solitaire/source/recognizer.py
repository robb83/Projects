import numpy as np
import cv2
from operator import itemgetter, attrgetter

symbols1_threshold = 0.9
symbols1_name = ['S','H','C','D']
symbols1 = [cv2.cvtColor(cv2.imread('resources/spades.png'),cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/heart.png'),cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/club.png'),cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/diamond.png'),cv2.COLOR_BGR2GRAY)]

symbols0_threshold = 0.7
symbols_name = ['A','2','3','4','5','6','7','8','9','10','J','Q','K']
symbols_value = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14]
symbols0 = [cv2.cvtColor(cv2.imread('resources/A.png'),cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/2.png'),cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/3.png'),cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/4.png'),cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/5.png'),cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/6.png'),cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/7.png'),cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/8.png'),cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/9.png'),cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/10.png'),cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/J.png'),cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/Q.png'),cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/K.png'),cv2.COLOR_BGR2GRAY)]

def detectCardInColumn(column, cx, cy, output = None):
    cards = []
    gray = cv2.inRange(column, (200,200,200), (255,255,255))
    ret,thresh = cv2.threshold(gray, 128, 255, cv2.THRESH_BINARY)
    if thresh == None:
        return cards
    contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    for cnt in contours:
        area = cv2.contourArea(cnt)
        if area > 6000 and area < 7000:	
            [x,y,w,h] = cv2.boundingRect(cnt)
            card = column[y:(y+h), x:(x+w)]
            data = detectCard(card)
            success = 1 if data[0] != '' else 0
            if success == 1:
                cards.append((data, (cx + x, cy + y, w,h ), 0, cy + y))
                if output != None:
                    cv2.rectangle(output,(cx + x, cy + y),(cx + x + w, cy + y + h),(0,0,255),2)
        elif area > 700 and area < 1500:
            [x,y,w,h] = cv2.boundingRect(cnt)
            card = column[y:(y+h), x:(x+w)]
            data = detectCard(card)
            success = 1 if data[0] != '' else 0
            if success == 1:
                cards.append((data, (cx + x, cy + y, w,h ), 1, cy + y))
                if output != None:
                    cv2.rectangle(output,(cx + x, cy + y),(cx + x + w, cy + y + h),(0,0,255),2)
    return sorted(cards, key=itemgetter(3))

def detectCard(card):
    crop = card[0:50, 0:20]
    gray = cv2.cvtColor(crop, cv2.COLOR_BGR2GRAY)
    black = cv2.inRange(crop, (0,0,0), (50, 50, 50))
    color = 0
    if (np.sum(black) > 5000):
        color = 0
    else:
        color = 1
    symbol0 = ''
    symbol0v = 0
    symbol1 = ''
    max_value = 0
    results = []
    for symbol in symbols1:
        res = cv2.matchTemplate(gray, symbol, cv2.TM_CCOEFF_NORMED)
        minv, maxv, minloc, maxloc = cv2.minMaxLoc(res)
        results.append(maxv)
    max_value = max(results)
    if max_value >= symbols1_threshold:
        symbol1 = symbols1_name[results.index(max_value)]
    results = []
    for symbol in symbols0:
        res = cv2.matchTemplate(gray, symbol, cv2.TM_CCOEFF_NORMED)
        minv, maxv, minloc, maxloc = cv2.minMaxLoc(res)
        results.append(maxv)
    max_value = max(results)
    if max_value >= symbols0_threshold:
        symbol0 = symbols_name[results.index(max_value)]
        symbol0v = symbols_value[results.index(max_value)]
#    if color == 0 and (symbol1 == 'D' or symbol1 == 'H'):
#        symbol1 = ''
#    elif color == 1 and (symbol1 == 'S' or symbol1 == 'C'):
#        symbol1 = ''
    return symbol0, symbol1, color, symbol0v

def getCurrentScreenState(screen, columns, output = None):
    cardsInScreen = []
    for c in columns:
        cardsInColumn = detectCardInColumn(screen[c[0]:c[1], c[2]:c[3]], c[2], c[0], output)
        cardsInScreen.append(cardsInColumn)
    return cardsInScreen
