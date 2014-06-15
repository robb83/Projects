import os

def array2str(value, width):
    index = 0
    length = len(value)
    result = ''
    while index + width < length:
        result = result + str(value[index: index + width]) + "\n"
        index = index + width
    if index < length:
        result = result + str(value[index:]) + "\n"
    return result

def writeToFile(file, content):
    f = open(file, 'w')
    f.write(content)
    f.close()

def readLineFromFile(file, defaultValue):
    if not os.path.exists(file):
        return defaultValue
    f = open(file, 'r')
    value = f.readline()
    f.close()
    return value