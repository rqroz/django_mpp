from .modules import *
from website.opencv.processor import *

from operator import attrgetter
import json


class IndexView(View):
    matrix_dim = 32

    def get(self, request, *args, **kwargs):
        context = {
            'matrix_dim': self.matrix_dim
        }
        return render(request, 'website/index.html', context)

    def post(self, request, *args, **kwargs):
        json_points_str = request.POST.get('points')
        points = self.create_points_from_json(json_points_str)
        points.sort(key=attrgetter('x', 'y'))

        for (i, p) in enumerate(points):
            print("Point %d => X: %d, Y: %d"%(i+1, p.x, p.y))


        context = {'points': json.dumps([ob.__dict__ for ob in points])}
        print(context)

        if request.is_ajax():
            return JsonResponse(context, safe=False)
        else:
            context['matrix_dim'] = self.matrix_dim
            return render(request, 'website/index.html', context)

    def create_points_from_json(self, json_str):
        json_obj = json.loads(json_str)
        points = []
        for el in json_obj:
            p = Point(el['x'], el['y'])
            points.append(p)

        return points
