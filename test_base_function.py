import board
import busio
import time
import neopixel

import adafruit_veml7700
import adafruit_tcs34725

# Set up I2C bus
i2c = busio.I2C(board.SCL, board.SDA)

# Initialize VEML7700 sensor
veml7700 = adafruit_veml7700.VEML7700(i2c)

# Initialize TCS34725 sensor
tcs = adafruit_tcs34725.TCS34725(i2c)
tcs.integration_time = 100
tcs.gain = 1

# Initialize NeoPixel
pixel_pin = board.D18  # Adjust this if your NeoPixel is connected to a different pin
num_pixels = 7
pixels = neopixel.NeoPixel(pixel_pin, num_pixels, brightness=1.0, auto_write=False)

# Function to map lux to brightness factor (0.0 to 1.0)
def lux_to_brightness(lux, max_lux=1000):
    brightness = min(max(lux / max_lux, 0.0), 1.0)
    return brightness

# Main loop
try:
    while True:
        # Read lux from VEML7700
        lux = veml7700.lux
        print('Lux:', lux)

        # Read RGB bytes from TCS34725
        r, g, b = tcs.color_rgb_bytes
        print('RGB:', r, g, b)

        # Map lux value to brightness factor
        brightness_factor = lux_to_brightness(lux)
        print('Brightness factor:', brightness_factor)

        # Scale RGB values according to brightness_factor
        scaled_r = int(r * brightness_factor)
        scaled_g = int(g * brightness_factor)
        scaled_b = int(b * brightness_factor)

        # Set NeoPixels to the scaled RGB color
        for i in range(num_pixels):
            pixels[i] = (scaled_r, scaled_g, scaled_b)
        pixels.show()

        time.sleep(1.0)

except KeyboardInterrupt:
    # Turn off the NeoPixels before exiting
    pixels.fill((0, 0, 0))
    pixels.show()
