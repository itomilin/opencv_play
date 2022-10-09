import os

import cv2
import pytesseract

from imutils import contours

def number_recognition( img ):
    _, file_name = os.path.split( img )
    car_number_img = None
    msg = "could not recognize"
    status = 0
    image = cv2.imread( img )

    height, width = image.shape[:2]

    image_gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY) # преобразовать изображение в

    #cv2.imshow("Gray Image", image_gray) #вывести на экран для проверки
    #cv2.waitKey()

    # пороговая обработка для выделения контуров
    threshold = cv2.threshold(image_gray, 0, 255, cv2.THRESH_OTSU)[1]
    #cv2.imshow("Threshold", threshold) #вывести на экран для проверки
    #cv2.waitKey()

    # Найдем все контуры
    contourss = cv2.findContours(threshold, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    contourss, _ = contours.sort_contours(contourss[0]) # отсортируем их

    chars = set('0123456789,') # создадим множество цифр для отсечения "мусорных" текстов

    for c in contourss: # переберем все контуры
        area = cv2.contourArea(c) # найдем площадь контура
        x, y, w, h = cv2.boundingRect(c)

        if area > 500: # если площадь соизмерима с номером
            img = image[y:y+h, x:x+w] # получим этот контур из исходного изображения
    #        cv2.waitKey()
            result = pytesseract.image_to_string(img, lang='rus+eng')

            if len(result) > 7:
                car_number_img = img.copy()
                chars = "-`‘\/')( ,."
                for char in chars:
                    result = result.replace( char, "" )
                msg = result
                cv2.imwrite( f"./processed_files/{file_name}", car_number_img )
                status = 1

    #cv2.imshow( "car_n", car_number_img )
    #cv2.waitKey()

    return msg, status

