#! venv/bin/python3

import cv2
import numpy as np
from scipy.spatial.transform import Rotation

WINDOW_NAME = "test"

running = True
alpha = 0.0
test_image = "./assets/station.png"

def do_input():

    global running, alpha

    key = cv2.waitKey(1)

    if key == 27 or cv2.getWindowProperty(WINDOW_NAME, cv2.WND_PROP_VISIBLE) < 1:
        running = False

    elif key == 32:
        alpha -= 1.0

def projection(f, c):
    cx, cy = c

    mat = np.array([[f, 0, cx],
                    [0, f, cy],
                    [0, 0, 1]])
    return mat

def draw_grid(img):
    f, cx, cy, L = 810.5, 480, 270, 3.31 # Unit: [px], [px], [px], [m]

    camera_orientation = [-18.7, -8.2, 2.0] #Euler form

    camera_rotation = Rotation.from_euler('zyx', camera_orientation[::-1], degrees=True).as_matrix()
    camera_position = np.array([0, -L, 0])

    #Build view matrix from camera oritentation and position

    view_matrix = np.eye(4)
    view_matrix[:3, :3] = camera_rotation.T
    view_matrix[:3, 3] = np.dot(-camera_rotation.T, camera_position.T)

    # Prepare the camera projection
    proj_matrix = np.array([[f, 0, cx], 
                            [0, f, cy], 
                            [0, 0, 1]])

    grid_x = (-2, 3)
    grid_z = (5, 36)

    for x in range(*grid_x):
        p1 = np.array([x, 0, grid_z[0], 1.0])
        p2 = np.array([x, 0, grid_z[1], 1.0])

        view_p1 = np.matmul(view_matrix, p1.T)
        view_p2 = np.matmul(view_matrix, p2.T)

        p1_projected = np.matmul(proj_matrix, view_p1[:-1].T)
        p2_projected = np.matmul(proj_matrix, view_p2[:-1].T)

        print(p1_projected)

        #perspective devide
        p1_projected[0] /= p1_projected[2]
        p1_projected[1] /= p1_projected[2]

        p2_projected[0] /= p2_projected[2]
        p2_projected[1] /= p2_projected[2]

        color = (64, 128, 64)

        start = (int(p1_projected[0]), int(p1_projected[1]))
        end = (int(p2_projected[0]), int(p2_projected[1]))

        cv2.line(img, start, end, color, 2)

    return img

if __name__ == '__main__':

    img = cv2.imread(test_image)
    img= draw_grid(img)

    while running:
        cv2.imshow(WINDOW_NAME, img)
        do_input()


    cv2.destroyAllWindows()
