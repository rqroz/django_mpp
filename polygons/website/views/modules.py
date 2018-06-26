from django.shortcuts import render, get_object_or_404, redirect
from django.http import HttpResponse
from django.views.generic import View

from website.models import *
from website.forms import *
