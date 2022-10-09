import cv2
import numpy as np
import os

def change_background( img_path ):
    cap = cv2.VideoCapture( '../imgs/green_screen_360.mp4' )
    _, file_name = os.path.split( img_path )
    drop_ext = file_name.split('.')[0]

    bg = cv2.imread( img_path )
    bg = cv2.resize(bg, (640, 360))

    l_green = np.array([0, 0, 45])
    u_green = np.array([177, 216, 255])

    frame_width = int(cap.get(3))
    frame_height = int(cap.get(4))
    out = cv2.VideoWriter( f'./processed_files/{drop_ext}.mp4',
                           0x7634706d,
                           20.0,
                           (frame_width, frame_height) )

    while True:
        ret, frame = cap.read()
        if not ret:
            break

        if cv2.waitKey(25) & 0xFF == ord('q'):
            break

        #frame = cv2.resize(frame, (640, 360))

        hsv = cv2.cvtColor( frame, cv2.COLOR_BGR2HSV )

        mask = cv2.inRange(hsv, l_green, u_green)
        frame[mask == 0] = bg[mask == 0]

        out.write(frame)
        #cv2.imshow("video2", frame)

    cap.release()
    out.release()

