import sys

from PIL import Image


def convert_to_black_and_white(input_path, output_path):
    """Converts an image to black and white (grayscale) and saves it."""
    img = Image.open(input_path).convert("L")  # Convert to grayscale
    img.save(output_path)
    print(f"Black and white image saved: {output_path}")


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script.py <input_image> <output_image>")
        sys.exit(1)

    input_image = sys.argv[1]
    output_image = sys.argv[2]

    convert_to_black_and_white(input_image, output_image)
