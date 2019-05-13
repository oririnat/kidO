import cv2
import numpy as np
kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (7, 7))
binarizationThreshold = 127
changedPrecentThreshold=0.05
def diffrencing(previousImg,newImg):
	greyPreviousImg= cv2.cvtColor(previousImg, cv2.COLOR_BGR2GRAY)
	greyNewImg= cv2.cvtColor(newImg, cv2.COLOR_BGR2GRAY)
	diff=greyNewImg-greyPreviousImg
	diff=diff.astype("uint8")
	diff = cv2.erode(diff, kernel)
	diff = cv2.dilate(diff, kernel)
	height, width = diff.shape
	ret, diff = cv2.threshold(diff, binarizationThreshold, 255, cv2.THRESH_BINARY)
	changedPrecent=np.count_nonzero(diff)/(height*width)
	if changedPrecent > changedPrecentThreshold:
		x, y, w, h = cv2.boundingRect(diff)
		return newImg[y: y + h, x: x + w]
	else:
		return None