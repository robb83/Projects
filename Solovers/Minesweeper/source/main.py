import win32
import sys
import time
import cv2
import numpy as np
import utils as u
import minesweeper as ms

def recognizePattern(cell_image):
    global patterns_data, patterns_value
    results = []
    for pattern in patterns_data:
        res = cv2.matchTemplate(cell_image, pattern, cv2.TM_CCOEFF_NORMED)
        minv, maxv, minloc, maxloc = cv2.minMaxLoc(res)
        results.append(maxv)
    max_value = max(results)
    if max_value > 0.6:
        return patterns_value[results.index(max_value)]
    return -1

def recognizeCell(cell_image_bgr, cell_image_th3):
    v = np.sum(cell_image_th3)
    if v >= 61000:
        v = 0
    else:
        v = recognizePattern(cell_image_th3)
        if v == -1:
            if np.sum(cv2.inRange(cell_image_bgr, (0,  0, 240), (70,  70, 255))) > 0:
                v = 254
            else:
                v = 255
    return v

def loadStateFromWindow(window, minesweeper):
    screen_bgr = window.getFramebuffer()
    screen_gray = cv2.cvtColor(screen_bgr, cv2.COLOR_BGR2GRAY)
    screen_th3 = cv2.adaptiveThreshold(screen_gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 21, 3)
    
    empty = 1
    values = []
    for c in minesweeper.cells:
        imgcell = screen_th3[c[3][1] + 3:c[3][1] + 3 + c[4][1] - 1, c[3][0] + 3:c[3][0] + 3 + c[4][0] - 1]
        imgcellbgr = screen_bgr[c[3][1] + 3:c[3][1] + 3 + c[4][1] - 1, c[3][0] + 3:c[3][0] + 3 + c[4][0] - 1]
        v = recognizeCell(imgcellbgr, imgcell)
        if v != 255:
            empty = 0
        values.append(v)
    return values, empty, (screen_bgr, screen_gray, screen_th3)

def savestate(iteration, values, screen, minesweeper):       
    cv2.imwrite('unknow_states/screen_' + str(iteration) + 'bgr.png', screen[0])
    cv2.imwrite('unknow_states/screen_' + str(iteration) + 'gray.png', screen[1])    
    cv2.imwrite('unknow_states/screen_' + str(iteration) + 'th.png', screen[2]) 
    
    strValues = u.array2str(values, minesweeper.width)
    u.writeToFile('unknow_states/screen_' + str(iteration) + '.txt', strValues)

    print strValues

def nextstep(minesweeper, window, iteration, last_selected):
    values, empty, screen = loadStateFromWindow(window, minesweeper)
    minesweeper.loadState(values)

    if empty:
        selected = minesweeper.cells[30]
        if selected == last_selected:
            savestate(iteration, values, screen, minesweeper)
            print "Repeated step"
            return None, 1
        print 'empty: ' + str(selected[5])
        window.leftClick(selected[5])
        return selected, 0

    selected = minesweeper.domarkmine()
    if selected:
        if selected == last_selected:
            savestate(iteration, values, screen, minesweeper)
            print "Repeated step"
            return None, 1
        print 'mark: ' + str(selected[5])
        window.rightClick(selected[5])
        return selected, 0

    selected = minesweeper.doclick()
    if selected:
        if selected == last_selected:
            savestate(iteration, values, screen, minesweeper)
            print "Repeated step"
            return None, 1
        print 'visit: ' + str(selected[5])
        window.leftClick(selected[5])
        return selected, 0
    
    savestate(iteration, values, screen, minesweeper)

    selected = minesweeper.dorandomclick()
    if selected:
        if selected == last_selected:
            print "Repeated step"
            return None, 1
        print 'random: ' + str(selected[5])
        window.leftClick(selected[5])
        return selected, 0

    return None, 0

# APPLICATION START

window = win32.getOrCreateWindow('Minesweeper', 'c:\Program Files\Microsoft Games\Minesweeper\MineSweeper.exe', 'unknow_states/screen_temp.bmp')
if window == None:
    print 'Minesweeper window not found'
    sys.exit()
time.sleep(3)

cell_number = 9
cell_size = 18
cell_offset = (26, 26) # x, y
patterns_data = [cv2.cvtColor(cv2.imread('resources/pattern_1.png'), cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/pattern_2.png'), cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/pattern_3.png'), cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/pattern_4.png'), cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/pattern_5.png'), cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/pattern_3b.png'), cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/pattern_1b.png'), cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/pattern_2b.png'), cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/pattern_2c.png'), cv2.COLOR_BGR2GRAY), cv2.cvtColor(cv2.imread('resources/pattern_3c.png'), cv2.COLOR_BGR2GRAY)]
patterns_value = [1, 2, 3, 4, 5, 3, 1, 2, 2, 3]

minesweeper = ms.generateMinesweeper(cell_number, cell_number, cell_size, cell_offset)

iteration = int(u.readLineFromFile('unknow_states/iteration.txt', '0'))
key_state = window.keyState(145)
nothingdo = 0
last_selected = None
while 1:
    
    if window.isDisabled():
        #press Play Again button
        nothingdo = 0
        time.sleep(1)
        if window.keyState(145) != key_state:
            print "User cancelled"
            break
        window.clickDialogButton("[CLASS:Button; INSTANCE:1]", (10, 10))
        print "Play Again"
        continue

    last_selected, repeated = nextstep(minesweeper, window, iteration, last_selected)
    if repeated:
        break
    if last_selected:
        nothingdo = 0
    else:
        time.sleep(0.5)
        nothingdo = nothingdo + 1
        if nothingdo > 2:
            print "I don't know what should I do"
            break

    if window.keyState(145) != key_state:
        print "User cancelled"
        break

    time.sleep(0.15)
    iteration = iteration + 1

u.writeToFile('unknow_states/iteration.txt', str(iteration))