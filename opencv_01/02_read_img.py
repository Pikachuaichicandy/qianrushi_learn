import cv2 as cv

src = cv.imread('img/01.jpg')
print(src)

cv.imshow('111', src)
cv.waitKey()  #witkey等待用户输入，0表示无限等待
# cv.destroyAllWindows()  #关闭所有窗口

#写入图片
cv.imwrite('img/02.jpg', src)