import os,sys
import ROOT
from larlite import larlite as fmwk
import numpy as np
import matplotlib.pyplot as plt

def module():
	"""
	if (len(sys.argv) != 2):
		print 'ERROR: run as follows:'
		print 'python MIPscript_integral.py WIRE_FILE.ROOT'
		print 'script currently accepts a single input file'
		sys.exit(0)
	"""
        #print "entered main function"
	# vectors where to store ADC amplitudes
	ADC_Y_max_v = []
	ADC_V_max_v = []
	ADC_U_max_v = []

	ADC_Y_min_v = []
	ADC_V_min_v = []
	ADC_U_min_v = []

	#ADC_ch_max_mat = [[]]*8256
	#ADC_ch_min_mat = [[]]*8256
	# DC
	ADC_ch_max_mat = {}
	ADC_ch_min_mat = {}

	lengthROI_ch_mat = {}
	int_adc_ch_mat = {}
        #print "made vectors"

	# open ROOT file
	fin = ROOT.TFile("/a/data/westside/afadeeva/CompOut/compWF_Apr1_bbb_5_first1000ev.root")  #(sys.argv[-1])
	# open TTree
	wire_caldata_tree = ROOT.TChain('wire_caldata_tree')
	wire_caldata_tree.Add("/a/data/westside/afadeeva/CompOut/compWF_Apr1_bbb_5_first1000ev_1.root")
	wire_caldata_tree.Add("/a/data/westside/afadeeva/CompOut/compWF_Apr1_bbb_5_first1000ev_2.root")
	t = fin.Get('wire_caldata_tree')
	print "entries ", t.GetEntries()
	# loop over all entries in TTree (each entry one event)
	for n in xrange(t.GetEntries()):   # t.GetEntries()-1,(t.GetEntries()-11),-1):  #t.GetEntries()):

		# get nth entry
		t.GetEntry(n)
		#print "Entry ", n, " is ", t.GetEntry(n)

		# get larlite wire branch. This is an instance
		# of event_wire which you can find in core/DataFormat/wire.h
		# this won't work if larlite is not setup
		# wire_v is a C++ std::vector so you can use all
		# the functions you know about std::vectors on it
		wire_v = t.wire_caldata_branch
		#print "wire caldata branch ",t.wire_caldata_branch	
		#print "wire_v size ", wire_v.size()
		# loop through all wires
		for i in xrange(wire_v.size()):

			wire = wire_v.at(i)
			channel = wire.Channel()
			#print 'channel ',channel
			#continue 
			#if(channel<100):
			# the goal is to find, for each wire, the
			# maximum (or minimum, it is up to you) ADC value for that wire
			# and plot it
			# to figure out how to navigate the wire object go read
			# core/DataFormat/wire.h

			# the plane number is a bit tricky, so you get that like this:
			plane = int(wire.View())

			ROIs = wire.SignalROI()

			ranges = ROIs.get_ranges()

			# copy of maximum ADCs for this wire
			maxadcs = []
			minadcs = []
			integrals = []
			lengths = []
			#print "size of ranges ", ranges.size()
			for j in xrange(ranges.size()):
		    
				#print "channel is ", channel
				data = ranges.at(j).data()
				adcmax = -1
				adcmin = 4095
				len_ROI = len(data)
				int_adc = 0
				for adc in data:
					if (adc > adcmax): adcmax = adc
					if (adc < adcmin): adcmin = adc
					int_adc += adc
					#print "adc is ", adc
				integrals.append(int_adc)
				lengths.append(len_ROI)
				if (abs(adcmax) > abs(adcmin)):
					maxadcs.append( adcmax )
				else:
					minadcs.append( adcmin )

			#print "size of max/min ADCs ", len(maxadcs)," ", len(minadcs)
			if not (channel in ADC_ch_max_mat):
				ADC_ch_max_mat[channel] = []
			if not (channel in ADC_ch_min_mat):
				ADC_ch_min_mat[channel] = []
			for adc in maxadcs:
				ADC_ch_max_mat[channel].append( int(adc) )        
			for adc in minadcs:
				ADC_ch_min_mat[channel].append(int(adc) )
			if not (channel in int_adc_ch_mat):
				int_adc_ch_mat[channel] = []
			if not (channel in lengthROI_ch_mat):
				lengthROI_ch_mat[channel] = []

			for x in integrals:
				int_adc_ch_mat[channel].append( x )
			for x in lengths:
				lengthROI_ch_mat[channel].append( x )
	#sys.exit()

	#for channel in ADC_ch_max_mat:
	   # print 'channel %i has entries '%channel
	   # print ADC_ch_max_mat[channel]
	   # print
	   # print


	#        print "matrix value max  ", ADC_ch_max_mat[channel]   
	#        print "matrix value min  " , ADC_ch_min_mat[channel]   
	#for g in xrange(len( ADC_ch_max_mat[10])):
	 #  print "difference ", ADC_ch_max_mat[10][g] - ADC_ch_max_mat[101][g]

	# we filled the vectors, let's plot them
	# how to make histograms using matplotlib?
	# se here: http://matplotlib.org/1.2.1/examples/api/histogram_demo.html
	# or our many examples here: https://github.com/NevisUB/Tutorials/tree/master/matplotlib
	print "Openning Root file"
	tfile = ROOT.TFile("Apr1_max_min_len_v_adc_all.root","RECREATE")
	#h_max_v = []
	#print "Filling histograms for max"
	"""for channel in ADC_ch_max_mat:
	    ch_ADC = ROOT.TH1I("ch%d_ADC_max" % channel,"channel %d ADC vals" % channel ,201,0,200)
	    for adc in ADC_ch_max_mat[channel]:
		ch_ADC.Fill(int(adc))	
	    h_max_v.append( ch_ADC )
	#    print "channel (write loop) ", channel
	print "Writing Max File"
	for h in h_max_v:
	   h.Write()

	h_min_v = []
	print "Filling histograms for min"
	for channel in ADC_ch_min_mat:
	    ch_ADC_min = ROOT.TH1I("ch%d_ADC_min" % channel,"channel %d ADC vals" % channel ,201,-200,0)
	    for adc in ADC_ch_min_mat[channel]:
		ch_ADC_min.Fill(int(adc))	
	    h_min_v.append( ch_ADC_min )

	print "Writing Min File"
	for g in h_min_v:
	   g.Write()

	hist_int_v = []
	for channel in int_adc_ch_mat:
	     int_ch_hist = ROOT.TH1I("int_ch%d" % channel,"channel %d integral of ADC vals" % channel,2001,-1000,1000)
	     for adc in int_adc_ch_mat[channel]:
		 int_ch_hist.Fill(int(adc))	
	     hist_int_v.append( int_ch_hist )
	for h in hist_int_v:
	    h.Write()

	hist_len_v = []
	for channel in lengthROI_ch_mat:
	    length_ch_hist = ROOT.TH1I("length_ch%d" % channel,"channel %d integral of ADC vals" % channel,201,0,200)
	    for leng in lengthROI_ch_mat[channel]:
		length_ch_hist.Fill(int(leng))
	    hist_len_v.append( length_ch_hist )
	for h in hist_len_v:
	    h.Write()

	len_int = ROOT.TH2I("len_int","length of ROI v integral of ADCs",201,0,200,2001,-1000,1000)
	if(len(lengthROI_ch_mat) == len(int_adc_ch_mat)):
	    for ch in int_adc_ch_mat:
		if(len(lengthROI_ch_mat[ch]) == len(int_adc_ch_mat[ch])):
		    for x in xrange(len(lengthROI_ch_mat[ch])):
			len_int.Fill(int(lengthROI_ch_mat[ch][x]),int_adc_ch_mat[ch][x])
	len_int.SetXTitle("length of ROI")
	len_int.SetYTitle("Sum of ADC values")
	len_int.Write()
	

	hist_len_v_ch = ROOT.TH2I("hist_len_v_ch","length v channel",8257,0,8256,201,0,200)
	hist_int_v_ch = ROOT.TH2I("hist_int_v_ch","integral adcs v channel",8257,0,8256,2001,-1000,1000)
	for channel in int_adc_ch_mat:
	    for x in int_adc_ch_mat[channel]:
	       hist_int_v_ch.Fill(int(channel),int(x))
	for channel in lengthROI_ch_mat:
	    for y in lengthROI_ch_mat[channel]:
	       hist_len_v_ch.Fill(int(channel),int(y))
	hist_len_v_ch.SetXTitle("Channel")
	hist_len_v_ch.SetYTitle("Length of ROI")
	hist_int_v_ch.SetXTitle("Channel")
	hist_int_v_ch.SetYTitle("Sum of ADC values")
	hist_len_v_ch.Write()
	hist_int_v_ch.Write()  
        """
	print "making histos"
	ch_ADC_min_all = ROOT.TH2I("ch_ADC_min_all","channel vs ADC",8257,0,8256,200,-200,0)
	ch_ADC_max_all = ROOT.TH2I("ch_ADC_max_all","channel vs ADC",8257,0,8256,200,0,200)
	for channel in ADC_ch_min_mat:
		for adc in ADC_ch_min_mat[channel]:
			ch_ADC_min_all.Fill(channel,adc)	
	for channel in ADC_ch_max_mat:
		for adc in ADC_ch_max_mat[channel]:
			ch_ADC_max_all.Fill(channel,adc)
	print "Histos filled"	
	ch_ADC_min_all.SetXTitle("Channel")
	ch_ADC_min_all.SetYTitle("Min. ADC Value")
	ch_ADC_max_all.SetXTitle("Channel")
	ch_ADC_max_all.SetYTitle("Max. ADC Value")

	ch_ADC_min_all.Write()
	ch_ADC_max_all.Write()
	print "Histos written"
	tfile.Close()

