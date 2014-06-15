import win32com
import win32com.client
import win32api
import win32gui
import win32ui
import win32con
import cv2
import time

autoit = win32com.client.Dispatch("AutoItX3.Control")

class Window:
    hwnd = None
    position = None
    size = None
    client_position = None
    offset = None
    screen_filepath = None
    def __init__(self, hwindow, screen_filepath):
        self.screen_filepath = screen_filepath
        self.hwnd = hwindow
        self.position = win32gui.GetWindowRect(self.hwnd)
        self.size = win32gui.GetClientRect(self.hwnd)

        width = (self.position[2] - self.position[0])
        height = (self.position[3] - self.position[1])
        self.client_position = (self.position[0] + (width - self.size[2]), self.position[1] + (height - self.size[3]))
        self.offset = ((width - self.size[2]), (height - self.size[3]))

    def show(self):
        win32gui.ShowWindow(self.hwnd, 1) # SW_SHOWNORMAL = 1
        win32gui.SetActiveWindow(self.hwnd)
        win32gui.SetForegroundWindow(self.hwnd)

    def getFramebuffer(self):
        wDC = win32gui.GetWindowDC(self.hwnd)
        dcObj=win32ui.CreateDCFromHandle(wDC)
        cDC=dcObj.CreateCompatibleDC()
        dataBitMap = win32ui.CreateBitmap()
        dataBitMap.CreateCompatibleBitmap(dcObj, self.size[2], self.size[3])
        cDC.SelectObject(dataBitMap)
        cDC.BitBlt((0,0), (self.size[2], self.size[3]) , dcObj, self.offset, win32con.SRCCOPY)
        dataBitMap.SaveBitmapFile(cDC, self.screen_filepath)
        return cv2.imread(self.screen_filepath)

    def leftClick(self, position, clicks = 1):
        autoit.MouseClick('left', self.client_position[0] + position[0], self.client_position[1] + position[1], clicks, 3)
        
    def rightClick(self, position, clicks = 1):
        autoit.MouseClick('right', self.client_position[0] + position[0], self.client_position[1] + position[1], clicks, 3)
        
    def click(self, position, clickType = 'left', clicks = 1):
        autoit.MouseClick(clickType, self.client_position[0] + position[0], self.client_position[1] + position[1], clicks, 3)

    def drag(self, position1, position2, clickType = 'left'):
        autoit.MouseMove(self.client_position[0] + position1[0], self.client_position[1] + position1[1], 3)
        autoit.MouseDown(clickType)
        autoit.MouseMove(self.client_position[0] + position2[0], self.client_position[1] + position2[1], 3)
        autoit.MouseUp(clickType)       

    def isDisabled(self):
        # GWL_STYLE, -16 
        # WS_DISABLED, 0x08000000L, 134217728
        return (win32gui.GetWindowLong(self.hwnd, -16) & 134217728) > 0
    
    def clickDialogButton(self, id, position):
        autoit.MouseClick('left', self.client_position[0] + position[0], self.client_position[1] + position[1], 1, 3)
        autoit.ControlClick('[ACTIVE]', '', id)

    def keyState(self, key):
        return win32api.GetKeyState(key)

def getOrCreateWindow(title, command, screen_filepath):
    hwindow = win32gui.FindWindow(None, title)
    if hwindow == 0:
        autoit.Run(command)
        autoit.WinWaitActive(title, "", 5)
        hwindow = win32gui.FindWindow(None, title)
        if hwindow == 0:
            return None
    window = Window(hwindow, screen_filepath)
    window.show()
    return window