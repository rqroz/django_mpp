# polygon_finder

=====
MPP
=====

Minimum Perimerer Polygon (MPP)is a technique used to represent the shape in a given boundary using the fewest possible number of sequences.

More information about this can be found at Chapter 11 of the book Digital Image Processing by Rafael C. Gonzalez, Richard E. Woods.

Quick start
-----------

1. Add "mpp" to your INSTALLED_APPS setting like this::

    INSTALLED_APPS = [
        ...
        'mpp',
    ]

2. Include the polls URLconf in your project urls.py like this::

    path('mpp/', include('mpp.urls')),

3. Go to the mpp's subfolder called 'opencv' and in processor.py you will find:

    from django_mpp.settings import BASE_DIR

    Change the 'django_mpp' part to the app that was created with the creation
    of your project (it usually has the same name of your project folder).

4. Start the development server and visit http://127.0.0.1:8000/mpp/
   and start having fun!
