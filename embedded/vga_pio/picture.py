from PIL import Image
import numpy as np
import os

image_height = 320
image_width = 240
image_path = "D:/Pictures/SONY/DSC03384.jpg"

image_string = ""

with Image.open(image_path) as img:
    img = img.resize((image_height, image_width))
    img = img.quantize(colors=256)
    img = img.convert("RGB")
    # Limit the colors to 2 bits per channel
    img = img.point(lambda x: np.floor(x / 64))
    # img.show()
    pixel = (image_width * image_height)
    image_string += f'unsigned char picture[{pixel}] = {{'
    for w in range(image_width):
        for h in range(image_height):
            colors = img.getpixel((h, w))
            colors = colors[0] << 4 | colors[1] << 2 | colors[2]
            image_string += f"{int(colors):01d}"
            if not h == image_height - 1:
                image_string += ", "
    image_string += "};"

if os.path.exists("picture.h"):
    print('file exists, removing!')
    os.remove("picture.h")

with open("picture.h", "w+") as f:
    f.write(image_string)

print('done!')