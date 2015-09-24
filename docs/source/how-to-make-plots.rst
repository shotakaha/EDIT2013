==================================================
How to make plots
==================================================

ROOT macro demos are in :file:`macors` directory.
Move to this directory first.
Start ROOT CINT and load MACRO.
Then execute as samples below.

*** IMPORTANT NOTICE ***
    These are just simple demos.
    I encourage you to look into the codes and improve it, or make your own.
    ( If you know ROOT, you can do it =D )


:Sample command 1: mppcTest.C to check one histogram

The function :func:`histText` is defined to return TH1D*.
You need to create TH1D* and draw histogram.

.. code-block:: bash

   $ root
   root[] > .L mppcTest.C
   root[] > TH1D *h1 = histText("h1", "../data/20130305/mppctest001.txt")
   root[] > h1->Draw()


:Sample command 2: adcCalibration.C to check three adc data at one time

The function "histText" is defined to return TCanvas*.
You need to create TCanvas*. (Canvas will be drawn automatically)

.. code-block:: bash

   $ root
   root[] > .L adcCalibration.C
   root[] > TCanvas *c1 = histText(32)


:Sample command 3: tracker.C for eventdisplay

You don't need to load tracker.C, just type

.. code-block:: bash

   $ root tracker.C

or, in case you want to start from certain run number,

.. code-block:: bash

   $ 'root tracker.C(3)'


Change branch descriptor
==================================================

All macros use method
:func:`TTree::ReadFile(FILENAME, BRANCH_DESCRIPTOR)`
to create TTree from text file.
If you changed orders or number of columns of output text format,
modify ``BRANCH_DESCRIPTOR`` as needed.
