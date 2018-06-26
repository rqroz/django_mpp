from .modules import *


class IndexView(View):
    def get(self, request, *args, **kwargs):
        return render(request, 'website/index.html', {})
