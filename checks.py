import re
import sys


def check_coor(e):
    pattern = re.compile(r'\(-?\d+,-?\d+\)')
    p1 = re.compile(r'\(')
    p2 = re.compile(r'\)')
    p3 = re.compile(r',')
    p4 = re.compile(r'\s')

    if not pattern.findall(e):
        print >> sys.stderr, "Error: Improper input for coordinate pairs: Use format (x,y)"
        return False
    else:
        m = pattern.findall(e)

        if len(p1.findall(e)) == len(p2.findall(e)) == len(p3.findall(e)):
            #if len(p4.findall(e)) != len(p1.findall(e)):
             #   print >> sys.stderr, "Error: Missing spaces in command"
             #   return False
            if len(p1.findall(e)) != len(m):
                print >> sys.stderr, "Error: Missing values of coordinates"
                return False
        else:
            print >> sys.stderr, "Error: Missing brackets or comma"
            return False
    return True


def check_pattern(line):
    p = re.compile(r'[a-z]\s\"*\"')
    if p.findall(line):
        return True
    return False


def check_intersect(l1, l2, t):
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y
    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y

    count = 0
    #Check if intersection point is within both line segments
    #Check for l1
    if x1 <= x2:
        low = x1
        high = x2
    else:
        low = x2
        high = x1

    if low <= t.x <= high:
        count += 1
    else:
        return False

    if y1 <= y2:
        low = y1
        high = y2
    else:
        low = y2
        high = y1

    if low <= t.y <= high:
        count += 1
    else:
        return False

    #Check for l2
    if x3 <= x4:
        low = x3
        high = x4
    else:
        low = x4
        high = x3

    if low <= t.x <= high:
        count += 1
    else:
        return False

    if y3 <= y4:
        low = y3
        high = y4
    else:
        low = y4
        high = y3

    if low <= t.y <= high:
        count += 1
    else:
        return False

    if count == 4:
        return True
    else:
        return False
