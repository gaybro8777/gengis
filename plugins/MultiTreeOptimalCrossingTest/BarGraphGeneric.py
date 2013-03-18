from AbstractPlot import AbstractPlot

import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.figure import Figure
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas

class BarGraphGeneric(AbstractPlot):
	def __init__(self, parent):
		AbstractPlot.__init__(self, parent, (255,255,255))
		
	def Draw(self, x, yOne, stdOne = None, stdTwo = None,
				xLabel = None, xTicks = None):
		"""Draw Bar Graph"""

		self.figure = matplotlib.figure.Figure()
		N = len(x)

		ind = np.arange(N)  # the x locations for the groups
		width = 0.35       # the width of the bars

		ax = self.figure.add_subplot(111)
		rects1 = ax.bar(ind, yOne, color = 'g', yerr = stdOne)
		# # rects2 = ax.bar(ind + width , yTwo, width, color = 'r', yerr = stdTwo)
		
		# add some
		ax.set_ylabel("Tree Crossing Fit")
		if (xLabel != None):
			ax.set_xlabel(xLabel)
		# # if (locationTwo == 'N/A'):
			# # ax.set_title("Frequency of " + xLabel + " at site " + location)
		# # else:
			# # ax.set_title("Frequency of " + xLabel + " at sites " + location + " and " + locationTwo)
		ax.set_xticks(ind + width)
		if (xTicks != None):
			ax.set_xticklabels( xTicks ) #, rotation = 'vertical' )
		# # ax.legend( (rects1[0], rects2[0]), (location, locationTwo) )
		
		ax.set_ylim(ymin=0, ymax=1.0)
		
		self.figure.autofmt_xdate()
		self.canvas = FigureCanvas(self, -1, self.figure)

		self.SetPlotSize()
	