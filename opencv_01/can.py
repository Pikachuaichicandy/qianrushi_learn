# import cv2 as cv
# import numpy as np
# import random

# # 将图片数据读取进来
# img = cv.imread("img/canny.jpg",cv.IMREAD_COLOR)
# cv.imshow("img",img)

# # 1. 将图片转成灰度图片
# grayImg = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

# # 2. canny算法
# dstImg = cv.Canny(grayImg,50,180)

# cv.imshow('dstimg',dstImg)
# cv.waitKey(0)


# import cv2 as cv

# img = cv.imread("img/canny.jpg",cv.IMREAD_GRAYSCALE)
# cv.imshow("src",img)

# # 使用拉普拉斯算子
# dst = cv.Laplacian(img,cv.CV_32F)
# # 取绝对值，将数据转到uint8类型
# dst = cv.convertScaleAbs(dst)

# # cv.imshow("dst",dst)
# # dstImg = cv.Canny(dst,50,180)
# cv.imshow("dst",dst)
# cv.waitKey(0)
# cv.destroyAllWindows();



# import cv2 as cv

# img = cv.imread("img/canny.jpg",cv.IMREAD_GRAYSCALE)
# # 修改缩小比例为2/3
# scale_percent = 50  # 2/3 约等于 66.67%
# width = int(img.shape[1] * scale_percent / 100)
# height = int(img.shape[0] * scale_percent / 100)
# dim = (width, height)
# img = cv.resize(img, dim, interpolation = cv.INTER_AREA)

# cv.imshow("src",img)

# # 使用拉普拉斯算子
# dst = cv.Laplacian(img,cv.CV_32F)
# # 取绝对值，将数据转到uint8类型
# dst = cv.convertScaleAbs(dst)

# # cv.imshow("dst",dst)
# # dstImg = cv.Canny(dst,50,180)
# cv.imshow("dst",dst)
# cv.waitKey(0)
# cv.destroyAllWindows()



# import cv2 as cv
# # 将图片数据读取进来
# img = cv.imread("img/canny_1.jpg",cv.IMREAD_COLOR)
# cv.imshow('img',img)
# # 双边滤波
# dstImg = cv.bilateralFilter(img, 5, 100, 100)
# # 显示改变之后的图像
# cv.imshow('newimg',dstImg)
# cv.waitKey(0)



import cv2 as cv

# 读取图像
img = cv.imread("img/canny.jpg",cv.IMREAD_GRAYSCALE)
cv.imshow("src",img)

ret,thresh_img = cv.threshold(img, 225, 255, cv.THRESH_BINARY_INV)
cv.imshow("normal", thresh_img);

gaussian_img = cv.GaussianBlur(img,(5,5),0)
cv.imshow("g",gaussian_img)

ret,thresh_img = cv.threshold(gaussian_img, 0, 255, cv.THRESH_BINARY|cv.THRESH_OTSU)
cv.imshow("otsu", thresh_img);

print("阈值：",ret)
cv.waitKey(0)
cv.destroyAllWindows()