import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors

def read_image(filename):
    with open(filename, "rb") as f:
        width = np.fromfile(f, dtype=np.int64, count=1)[0]
        height = np.fromfile(f, dtype=np.int64, count=1)[0]
        data = np.fromfile(f, dtype=np.uint8, count=width*height)
    image = data.reshape((height, width))
    return image

image = read_image("image.bin")
print(f"Image size: {image.shape[0]}x{image.shape[1]}")
print(f"Range of values: {image.min()}:{image.max()}")

plt.figure(1); plt.clf(); plt.gcf().set_size_inches(15, 15);

plt.imshow(image,
           interpolation='none',
           cmap='inferno',
           norm=colors.LogNorm(vmin=image.min(), vmax=image.max()),
)
plt.axis("off")
plt.savefig("mandelbrot.png", dpi=200)