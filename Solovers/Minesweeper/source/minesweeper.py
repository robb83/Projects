import random

class Minesweeper:
    width = 0
    height = 0
    cells = None
    values = None

    def __init__(self, cells, width, height):
        self.cells = cells
        self.width = width
        self.height = height
        self.values = [0] * (self.width * self.height)
        
    def loadState(self, values):
        self.values = values

    def getNeighbors(self, pos):
        x = pos[0]
        y = pos[1]
        results = []
        if y > 0:
            if x > 0:
                results.append((x - 1, y - 1))
            results.append((x ,y - 1))
            if x < self.width - 1:
                results.append((x + 1,y - 1))   

        if x > 0:
            results.append((x - 1, y))
        if x < self.width - 1:
            results.append((x + 1, y))

        if y < self.height - 1:
            if x > 0:
                results.append((x - 1, y + 1))
            results.append((x,y + 1))
            if x < self.width - 1:
                results.append((x + 1, y + 1))   
        return results
    
    def hiddenOrMarkedNeighbors(self, pos):
        value = 0
        ns = self.getNeighbors(pos)
        for n in ns:
            v = self.values[n[0] + n[1] * self.width]
            if v == 254:
                value = value + 1
            if v == 255:
                value = value + 1
        return value

    def markedNeighbors(self, pos):
        value = 0
        ns = self.getNeighbors(pos)
        for n in ns:
            v = self.values[n[0] + n[1] * self.width]
            if v == 254:
                value = value + 1
        return value

    def isPossibleMine(self, c):
        ns = self.getNeighbors(c[1])
        for n in ns:
            value = self.hiddenOrMarkedNeighbors(n)
            v = self.values[n[0] + n[1] * self.width]
            if value == v:
                return 1
        return 0

    def isPossibleNoMine(self, c):
        ns = self.getNeighbors(c[1])
        for n in ns:
            value = self.markedNeighbors(n)
            v = self.values[n[0] + n[1] * self.width]
            if value == v:
                return 1
        return 0

    def domarkmine(self):
        for c in self.cells:
            if self.values[c[2]] == 255:
                if self.isPossibleMine(c):
                    return c
        return None

    def doclick(self):
        for c in self.cells:
            if self.values[c[2]] == 255:
                if self.isPossibleNoMine(c):
                    return c;
        return None

    def dorandomclick(self):
        unknows = []
        for c in self.cells:
            if self.values[c[2]] == 255:
                unknows.append(c)
        if len(unknows) > 0:
            return random.choice(unknows)
        return None

def generateMinesweeper(width, height, cell_size, cell_offset):
    result_info = []
    result_value = []
    offsety = cell_offset[1]
    for y in range(height):
        offsetx = cell_offset[0]
        sizey = cell_size
        if y == 4:
            sizey = sizey + 1
        for x in range(width):        
            sizex = cell_size
            if x == 4:
                sizex = sizex + 1
            pos = (x, y)
            index = x + y * width
            size = (sizex, sizey)
            offset = (offsetx, offsety)
            click = (offsetx + 5, offsety + 5)
            # value, pos, index, offset, size, click
            cellinfo = (0, pos, index, offset, size, click)
            result_info.append(cellinfo)
            result_value.append(255)
            offsetx = offsetx + sizex
        offsety = offsety + sizey
    return Minesweeper(result_info, width, height)