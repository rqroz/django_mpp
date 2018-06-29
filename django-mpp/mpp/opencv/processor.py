import os, json
from operator import attrgetter
from mpp import __file__ as mpp_init_path

APP_PATH = os.path.dirname(mpp_init_path)

class Point(object):
    def __init__(self, x, y):
        self.x = x
        self.y = y

def create_points_from_json(json_str):
    json_obj = json.loads(json_str)
    points = []
    for el in json_obj:
        p = Point(el['x'], el['y'])
        points.append(p)

    points.sort(key=attrgetter('x', 'y'))

    return points


def write_points_to_file(points):
    with open(APP_PATH+'/opencv/points.txt', 'w') as pfile:
        for p in points:
            pfile.write("%d %d\n"%(p.x, p.y))

def recover_points_from_file():
    points = []
    print(APP_PATH)
    with open(APP_PATH+'/opencv/pointsfile.txt', 'r') as rfile:
        for line in rfile:
            parts = line.split()
            x = float(parts[0])
            y = float(parts[-1])
            p = Point(x, y)
            points.append(p)

    return points
