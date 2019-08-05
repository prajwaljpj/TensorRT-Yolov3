import Inference
import cv2

inf = Inference.inference()

engine_file = "yolov3_fp32.engine"
image = cv2.imread("test.jpg")

loader = inf.load_trt(engine_file)
print("load model:", loader)

if loader:
    data = inf.infer_single_image(image)
    print(data)
