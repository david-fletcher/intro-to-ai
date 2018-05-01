import cv2 as cv
import numpy as np
import sys

# The MIT License (MIT)
# Copyright (c) 2016 Vladimir Ignatev
#
# Permission is hereby granted, free of charge, to any person obtaining 
# a copy of this software and associated documentation files (the "Software"), 
# to deal in the Software without restriction, including without limitation 
# the rights to use, copy, modify, merge, publish, distribute, sublicense, 
# and/or sell copies of the Software, and to permit persons to whom the Software 
# is furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included 
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
# PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
# FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
# OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
# OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

def progress(count, total, status=''):
    bar_len = 60
    filled_len = int(round(bar_len * count / float(total)))

    percents = round(100.0 * count / float(total), 1)
    bar = '=' * filled_len + '-' * (bar_len - filled_len)

    sys.stdout.write('[%s] %s%s ...%s\r' % (bar, percents, '%', status))
sys.stdout.flush() # As suggested by Rom Ruben (see: http://stackoverflow.com/questions/3173320/text-progress-bar-in-the-console/27871113#comment50529068_27871113)


# David and Ryan's Code follows:


if(len(sys.argv) == 1):
	print "Please specify an image file."
	sys.exit()
elif(len(sys.argv) > 2):
	print "Too many arguements passed."
	sys.exit()

img = cv.imread(sys.argv[1])

if img is None:
	print "File not found."
	sys.exit()

height = img.shape[0]
width = img.shape[1]

for row in range(height-1):
	for col in range(width-1):
		blue = img[row,col,0]
		green = img[row,col,1]
		red = img[row,col,2]

		avg = (int(blue) + int(green) + int(red))/3

		img.itemset((row,col,0),avg)
		img.itemset((row,col,1),avg)
		img.itemset((row,col,2),avg)

		progress(row, height)

cv.imwrite('greyscale.jpg', img)
print("")
