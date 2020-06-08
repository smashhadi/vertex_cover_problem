from collections import defaultdict
import numpy as np
import re
from Intersect_class import point, line, intersect
from checks import check_intersect


def edge(a, b):
    return '<' + str(a) + ', ' + str(b) + '>'


def coordinate(x,y):
    return '(' + "{0:.2f}".format(x) + ', ' + "{0:.2f}".format(y) + ')'


def get_edges(street, edges, vert):
    EDGE = []

    for key, value in edges.iteritems():

        d = defaultdict(list)
        coor_list = street.get(key)
        for k, v in value:
            d[k].append(v)

        for ind, p in d.iteritems():
            temp = []
            temp.append(coordinate(coor_list[ind][0], coor_list[ind][1]))
            temp += p
            temp.append(coordinate(coor_list[ind + 1][0], coor_list[ind + 1][1]))

            for i in xrange(len(temp)-1):
                new_edge = edge(vert.index(temp[i]), vert.index(temp[i+1]))
                #print new_edge
                if (new_edge not in EDGE) and (temp[i] != temp[i+1]):
                    EDGE.append(new_edge)

    return EDGE


def find_graph(street):
    vert = []
    new_dict = {}

    for key, value in street.iteritems():
        v = []
        for x in value:
            temp = map(float, re.findall(r'-?\d+', x))
            v = np.append(v, temp)
        ver = np.reshape(v, (-1, 2))
        new_dict[key] = ver

    names = new_dict.keys()
    edge_dict = {}

    for n in names:
        edge_dict[n] = []

    for i in range(len(names)):
        a = new_dict.get(names[i])
        j = i+1
        while j < len(names):
            b = new_dict.get(names[j])
            for k in xrange(len(a)-1):
                for l in xrange(len(b)-1):
                    l1 = line(point(a[k][0], a[k][1]), point(a[k+1][0], a[k+1][1]))
                    l2 = line(point(b[l][0], b[l][1]), point(b[l+1][0], b[l+1][1]))
                    x1, y1 = l1.src.x, l1.src.y
                    x2, y2 = l1.dst.x, l1.dst.y
                    x3, y3 = l2.src.x, l2.src.y
                    x4, y4 = l2.dst.x, l2.dst.y
                    checks = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))
                    if checks == 0:
                        if x1 == x2 == x3 == x4:
                            if y1 <= y3 < y4 <= y2:
                                g = [coordinate(x1, y1), coordinate(x2, y2), coordinate(x3, y3), coordinate(x4, y4)]
                                for f in g:
                                    if f not in vert:
                                        vert.append(f)
                                # insert edge
                                if [k, g[2]] not in edge_dict[names[i]]:
                                    edge_dict[names[i]].append([k, g[2]])
                                if [k, g[3]] not in edge_dict[names[i]]:
                                    edge_dict[names[i]].append([k, g[3]])
                            elif y3 <= y1 < y2 <= y4:
                                g = [coordinate(x1, y1), coordinate(x2, y2), coordinate(x3, y3), coordinate(x4, y4)]
                                for f in g:
                                    if f not in vert:
                                        vert.append(f)
                                # insert edge
                                if [l, g[0]] not in edge_dict[names[j]]:
                                    edge_dict[names[j]].append([l, g[0]])
                                if [l, g[1]] not in edge_dict[names[j]]:
                                    edge_dict[names[j]].append([l, g[1]])
                        elif y1 == y2 == y3 == y4:
                            if x1 <= x3 < x4 <= x2:
                                g = [coordinate(x1, y1), coordinate(x2, y2), coordinate(x3, y3), coordinate(x4, y4)]
                                for f in g:
                                    if f not in vert:
                                        vert.append(f)
                                # insert edge
                                if [k, g[2]] not in edge_dict[names[i]]:
                                    edge_dict[names[i]].append([k, g[2]])
                                if [k, g[3]] not in edge_dict[names[i]]:
                                    edge_dict[names[i]].append([k, g[3]])
                            elif x3 <= x1 < x2 <= x4:
                                g = [coordinate(x1, y1), coordinate(x2, y2), coordinate(x3, y3), coordinate(x4, y4)]
                                for f in g:
                                    if f not in vert:
                                        vert.append(f)
                                # insert edge
                                if [l, g[0]] not in edge_dict[names[j]]:
                                    edge_dict[names[j]].append([l, g[0]])
                                if [l, g[1]] not in edge_dict[names[j]]:
                                    edge_dict[names[j]].append([l, g[1]])
                        elif (x2 != x1 and x4 != x3 and y2 != y1 and y4 != y3):
                            slope1 = (y2-y1)/(x2-x1)
                            slope2 = (y4-y3)/(x4-x3)
                            if slope1 == slope2:
                                y_int1 = y2 - (slope1*x2)
                                y_int2 = y4 - (slope2*x4)
                                if y_int1 == y_int2:

                                    if y1 <= y3 < y4 <= y2:
                                        g = [coordinate(x1, y1), coordinate(x2, y2), coordinate(x3, y3), coordinate(x4, y4)]
                                        for f in g:
                                            if f not in vert:
                                                vert.append(f)
                                        # insert edge
                                        if [k, g[2]] not in edge_dict[names[i]]:
                                            edge_dict[names[i]].append([k, g[2]])
                                        if [k, g[3]] not in edge_dict[names[i]]:
                                            edge_dict[names[i]].append([k, g[3]])
                                    elif x1 <= x3 < x4 <= x2:
                                        g = [coordinate(x1, y1), coordinate(x2, y2), coordinate(x3, y3), coordinate(x4, y4)]
                                        for f in g:
                                            if f not in vert:
                                                vert.append(f)
                                        # insert edge
                                        if [k, g[2]] not in edge_dict[names[i]]:
                                            edge_dict[names[i]].append([k, g[2]])
                                        if [k, g[3]] not in edge_dict[names[i]]:
                                            edge_dict[names[i]].append([k, g[3]])

                    else:
                        t = intersect(l1, l2)
                        #print 'Intersection of', l1, 'with', l2, 'is', t
                        if check_intersect(l1, l2, t):
                            tempo = [coordinate(x1, y1), coordinate(x2, y2), coordinate(t.x,t.y), coordinate(x3, y3), coordinate(x4, y4)]
                            #check if vertex is already in vertex list
                            for f in tempo:
                                if f not in vert:
                                    vert.append(f)

                            #insert edge
                            if [k, tempo[2]] not in edge_dict[names[i]]:
                                edge_dict[names[i]].append([k, tempo[2]])
                            if [l, tempo[2]] not in edge_dict[names[j]]:
                                edge_dict[names[j]].append([l, tempo[2]])
            j += 1

    #print edge_dict
    edges = get_edges(new_dict, edge_dict, vert)
    #ve = []
    #ed = []
    #for x in vert:
    #    temp = map(int, re.findall(r'-?\d+', x))
    #    ve.append(point(temp[0], temp[1]))

    #for y in edges:
    #    tem = map(int, re.findall(r'-?\d+', y))
    #    ed.append(Edge(tem[0], tem[1]))

    #vertices = {i: ve[i] for i in xrange(0, len(ve))}

    #return vertices, ed

    vertices = {i: vert[i] for i in xrange(0, len(vert))}

    return vertices, edges
