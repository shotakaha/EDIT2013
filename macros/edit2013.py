#! /usr/bin/env python
#! -*- coding:utf-8 -*-

import sys, os
import random
from optparse import OptionParser

from ROOT import PyConfig
PyConfig.IgnoreCommandLineOptions=True

from ROOT import gClient, gRandom
from ROOT import TFile, TTree, TChain
from ROOT import TCanvas, TH1D


## __________________________________________________
class Config(object):
    TreeName   = 'adc'
    TreeTitle  = 'EDIT2013'
    Nch        = 16
    Nrow       = 10
    AdcRange   = (0, 4096, 4096)
    DispSize   = (gClient.GetDisplayWidth(), gClient.GetDisplayHeight())
    
## __________________________________________________
class Edit2013(object):
    ## __________________________________________________
    def __init__(self):
        self.ch = TChain(Config.TreeName)
        self.config()
        
    ## __________________________________________________    
    def config(self):
        sys.stderr.write('TreeName   : %s\n' % Config.TreeName)
        sys.stderr.write('TreeTittle : %s\n' % Config.TreeTitle)
        sys.stderr.write('Nch        : %s\n' % Config.Nch)
        sys.stderr.write('Nrows      : %s\n' % Config.Nrow)
        sys.stderr.write('AdcRange   : %s\n' % (Config.AdcRange,))
        sys.stderr.write('DispSize   : %s\n' % (Config.DispSize,))
        sys.stderr.flush()

    ## __________________________________________________
    def generateDummyData(self, ofn , nrow = Config.Nrow):
        sys.stdout.write('generateDummyData : {0}\n'.format(ofn))
        ncolumn = Config.Nch
        gain = 10
        offset = 100
        #with open(ofn, 'w') as fout:
        fout = open(ofn, 'w')
        for row in range(nrow):
            random.seed()
            mean1 = random.uniform(0, 10)
            mean2 = random.randint(3, 5)
            #data = [str(int((gRandom.Poisson(mean1) * gRandom.Poisson(mean2) * gain) + offset)) for i in range(ncolumn)]
            data = [str(random.randint(0, 100) * gain + offset) for i in range(ncolumn)]
            line = ('\t').join(data)
            fout.write(line + '\n')
            if row % 10000 == 0:
                sys.stdout.write('[{0:3d}]\t{1}\n'.format(row, line))
        fout.close()                

    ## __________________________________________________
    def data2root(self, ifn):
        '''
        Convert raw data (assumed written in ASCII text) into ROOT format.
        '''
        if not os.path.exists(ifn):
            sys.stderr.write('Error:\tFile "{0}" does not exist.\n'.format(ifn))
            sys.exit(-1)

        ofn = ifn.replace('.txt', '.root')
        if os.path.exists(ofn):
            sys.stderr.write('Error:\tFile "{0}" already exist.\n'.format(ofn))
            sys.exit(-1)

        tname  = Config.TreeName
        title  = Config.TreeTitle
        channel_name = ['ch{0:02d}'.format(i) for i in range(Config.Nch)]
        branch_descriptor = ('/I:').join(channel_name) + ('/I')
        
        f = TFile(ofn, 'recreate')
        t = TTree(tname, title)
        t.ReadFile(ifn, branch_descriptor)
        t.Write()
        f.Close()
        sys.stdout.write('data2root : Creating {0}\n'.format(ofn))

    ## __________________________________________________
    def readData(self, ifn):
        '''
        Chain data in ROOT files.
        '''
        path, ext = os.path.splitext(ifn)
        if not ext in ['.root']:
            sys.stderr.write('readData : File "{0}" is not a ROOT file.\n'.format(ifn))
        else:
            sys.stdout.write('readData : File "{0}" added.\n'.format(ifn))
            self.ch.Add(ifn)
        #print self.ch.GetEntries()

    ## __________________________________________________
    def processData(self):
        '''
        Main part for analysis.
        '''
        ch = self.ch
        nentries = ch.GetEntries()
        xmin, xmax, xbin = Config.AdcRange
        hname  = 'hAdc%02d'
        htitle = 'hAdc%02d'
        hAdc = [TH1D(hname % i, htitle % i, xbin, xmin, xmax) for i in range(Config.Nch)]

        for i in range(Config.Nch):
            ch.Project(hAdc[i].GetName(), 'ch%02d' % i)

        self.hAdc = hAdc

    ## __________________________________________________
    def drawHist(self):
        '''
        Draw histograms on TCanvas.
        '''
        wx, wy = Config.DispSize
        c = TCanvas('c', 'c', wx, wy)
        if Config.Nch   <=  4: c.Divide(2, 2)
        elif Config.Nch <=  8: c.Divide(4, 2)
        elif Config.Nch <= 12: c.Divide(4, 3)
        elif Config.Nch <= 16: c.Divide(4, 4)
        elif Config.Nch <= 20: c.Divide(5, 4)
        elif Config.Nch <= 25: c.Divide(5, 5)

        hAdc = self.hAdc
        for i in range(Config.Nch):
            c.cd(i+1)
            hAdc[i].SetLineColor(i%8+1)
            hAdc[i].SetFillColor(i%8+1)
            hAdc[i].Draw()

        self.stop()

    ## __________________________________________________
    def stop(self):
        '''
        Ask if you want to quit or not in command prompt.
        '''
        sys.stderr.write('Press "q" to quit > ')
        ans = raw_input('')
        if ans in ['q', 'Q']:
            sys.stderr.write('\tQuit. Bye Bye\n')
        
            
## __________________________________________________
if __name__ == '__main__':
    usage = '%prog rootfile'

    parser = OptionParser(usage)
    parser.add_option('-g', '--gen',
                      dest = 'nrow',
                      help = 'generate dummy data with NROW lines. [default = %d]' % Config.Nrow)
    parser.add_option('-c', '--conv',
                      dest = 'convert',
                      action = 'store_true',
                      help = 'convert text file to ROOT file')
    parser.add_option('-n', '--nch',
                      dest = 'nch',
                      help = 'set channel number. [default = %d]' % Config.Nch)
    
    parser.set_defaults( nrow     = None,
                         convert  = False,
                         nch      = 16)
    (options, args) = parser.parse_args()

    edit = Edit2013()

    Config.Nch = int(options.nch)

    ### At least 1 filename argument needed
    if len(args) < 1:
        parser.error('Wrong number of argmunts. (%d)' % len(args))    

    ### Generate dummy data file
    if options.nrow:
        nrow = int(options.nrow)
        for ofn in args:
            edit.generateDummyData(ofn, nrow)
        sys.exit(-1)

    ### Convert rawdata into ROOT format
    if options.convert:
        for ifn in args:
            edit.data2root(ifn)
        sys.exit(-1)

    ### TChain trees in ROOT file and draw histograms
    for ifn in args:
        edit.readData(ifn)
    edit.processData()
    edit.drawHist()
        
