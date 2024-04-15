import cv2
import numpy as np
from mediapipe.python.solutions import hands
import socket

hands_module = hands.Hands(static_image_mode=False, max_num_hands=1, min_detection_confidence=0.5, min_tracking_confidence=0.5)

cap = cv2.VideoCapture(0)

prev_index_finger_tip_x = None
prev_index_finger_tip_y = None
swipe_detected = False
SWIPE_THRESHOLD = 50  # Пороговое значение для определения свайпа

# Настройки сокета
HOST = '127.0.0.1'  # IP адрес сервера
PORT = 65432        # Порт сервера

# Установка соединения с сервером
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    print("Connected to C++ server.")

    while True:
        ret, frame = cap.read()
        if not ret:
            break

        results = hands_module.process(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))

        if results.multi_hand_landmarks:
            for hand_landmarks in results.multi_hand_landmarks:
                index_finger_tip = hand_landmarks.landmark[hands.HandLandmark.INDEX_FINGER_TIP]

                if prev_index_finger_tip_x is not None and prev_index_finger_tip_y is not None:
                    index_finger_tip_x, index_finger_tip_y = int(index_finger_tip.x * frame.shape[1]), int(index_finger_tip.y * frame.shape[0])

                    if not swipe_detected:
                        # Разница между текущими и предыдущими координатами указательного пальца
                        dx = index_finger_tip_x - prev_index_finger_tip_x
                        dy = index_finger_tip_y - prev_index_finger_tip_y

                        # Определение направления свайпа
                        if abs(dx) > abs(dy):
                            if dx > SWIPE_THRESHOLD:
                                s.sendall(b"Swipe Left")  # Отправка свайпа вправо
                                swipe_detected = True
                            elif dx < -SWIPE_THRESHOLD:
                                s.sendall(b"Swipe Right")  # Отправка свайпа влево
                                swipe_detected = True
                        else:
                            if dy > SWIPE_THRESHOLD:
                                s.sendall(b"Swipe Down")  # Отправка свайпа вниз
                                swipe_detected = True
                            elif dy < -SWIPE_THRESHOLD:
                                s.sendall(b"Swipe Up")  # Отправка свайпа вверх
                                swipe_detected = True
                    else:
                        # Дополнительная проверка, чтобы убедиться, что палец переместился достаточно далеко
                        distance_moved = np.sqrt((index_finger_tip_x - prev_index_finger_tip_x)**2 + (index_finger_tip_y - prev_index_finger_tip_y)**2)
                        if distance_moved < SWIPE_THRESHOLD / 2:
                            swipe_detected = False

                prev_index_finger_tip_x = int(index_finger_tip.x * frame.shape[1])
                prev_index_finger_tip_y = int(index_finger_tip.y * frame.shape[0])

        cv2.imshow('Hand Detection', frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()
