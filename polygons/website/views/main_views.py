from .modules import *
from website.opencv.processor import *

import os, subprocess

class IndexView(View):
    matrix_dim = 32
    command_line = BASE_DIR+'/website/opencv/cfiles/MPP '+BASE_DIR

    def get(self, request, *args, **kwargs):
        context = {
            'matrix_dim': self.matrix_dim
        }
        return render(request, 'website/index.html', context)

    def post(self, request, *args, **kwargs):
        json_points_str = request.POST.get('points')
        points = create_points_from_json(json_points_str)
        write_points_to_file(points)

        shell_result = os.system(self.command_line)

        context = {}
        r_points = recover_points_from_file()
        
        if r_points and shell_result == 0:
            context['points'] = json.dumps([ob.__dict__ for ob in r_points])
        else:
            context['error'] = 'Os pontos fornecidos não são válidos para este algorítimo.'

        return JsonResponse(context, safe=False)
