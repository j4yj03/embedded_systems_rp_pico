from PIL import Image
import numpy as np
import os

image_height = 640
image_width = 480
image_path = "D:/Pictures/SONY/DSC03384.jpg"

image_string = ""

with Image.open(image_path) as img:
    img = img.resize((image_height, image_width))
    img = img.quantize(colors=64)
    img = img.convert("RGB")
    # Limit the colors to 2 bits per channel
    img = img.point(lambda x: np.floor(x / 64))
    # img.show()
    image_string += f"uint8_t picture[{image_height}][{image_width}] = {{\n"
    for w in range(image_width):
        image_string += "{"
        for h in range(image_height):
            colors = img.getpixel((h, w))
            colors = colors[0] << 4 | colors[1] << 2 | colors[2]
            image_string += f"{int(colors):01d}"
            if not h == image_height - 1:
                image_string += ", "
        image_string += "}"
        if not w == image_width - 1:
            image_string += ",\n"
    image_string += "\n};"

if os.path.exists("picture.h"):
    os.remove("picture.h")

with open("picture.h", "w+") as f:
    f.write(image_string)

print('done!')