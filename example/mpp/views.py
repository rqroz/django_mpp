from django.shortcuts import render
from django.http import JsonResponse
from django.views.generic import View

from mpp.opencv.processor import *
from mpp import __file__ as mpp_init_path

import os

APP_PATH = os.path.dirname(mpp_init_path)

class IndexView(View):
    matrix_dim = 32

    command_line = APP_PATH+'/opencv/cfiles/MPP '+APP_PATH

    def get(self, request, *args, **kwargs):
        context = {
            'matrix_dim': self.matrix_dim
        }
        return render(request, 'mpp/index.html', context)

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
