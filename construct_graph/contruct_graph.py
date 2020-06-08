import sys
from checks import check_coor, check_pattern
from graph_calc import find_graph
import re


def main():
    pattern = re.compile(r'\(-?\d+,-?\d+\)')
    dict_street = {}
    while True:
        line = sys.stdin.readline()
        line = line.lstrip().rstrip()

        if line.startswith('a'):
            if check_pattern(line):
                words = line.split('\"')
                if check_coor(words[2]):
                    if words[1].lower() in dict_street.keys():
                        print >> sys.stderr, "Error: Street name already exists"
                    else:
                        coor = pattern.findall(words[2])
                        dict_street[words[1].lower()] = coor
            else:
                print>> sys.stderr, "Error: Enter again in correct format and spacing"

        elif line.startswith('r'):
            if check_pattern(line):
                words = line.split('\"')
                if words[1].lower() not in dict_street.keys():
                    print >> sys.stderr, "Error: 'r' specified for a street that does not exist. "
                else:
                    del dict_street[words[1].lower()]
            else:
                print >> sys.stderr, "Error: Enter again in correct format and spacing"

        elif line.startswith('c'):
            if check_pattern(line):
                words = line.split('\"')
                if words[1].lower() not in dict_street.keys():
                    print >> sys.stderr, "Error: 'c' specified for a street that does not exist. "
                else:
                    if check_coor(words[2]):
                        coor = pattern.findall(words[2])
                        dict_street[words[1].lower()] = coor

        elif line.startswith('g'):
            try:
                v, e = find_graph(dict_street)
                print "V = {"
                for k, val in v.iteritems():
                    print " ", k, ": ", val
                print "}\n"
                print "E = {"
                for ed in e:
                    print " ", ed
                print "}"
            except:
                print >> sys.stderr, "Error: Cannot find graph vertices and edges"

        elif line == '' and len(line) == 0:
            break

        else:
            print >> sys.stderr, "Error: Command should begin with either a, c, r or g. Try again."

        #print dict_street

    #print 'Finished reading input'

    # return exit code 0 on successful termination
    sys.exit(0)

if __name__ == '__main__':
    main()
