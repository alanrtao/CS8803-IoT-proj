import board
import busio
import time
import threading
import json
import sys
from http.server import BaseHTTPRequestHandler, HTTPServer

import neopixel

import adafruit_veml7700
import adafruit_tcs34725

# Set up I2C bus
i2c = busio.I2C(board.SCL, board.SDA)

# Initialize VEML7700 sensor
veml7700 = adafruit_veml7700.VEML7700(i2c)

# Initialize TCS34725 sensor
tcs = adafruit_tcs34725.TCS34725(i2c)
tcs.integration_time = 300
tcs.gain = 16

# Initialize NeoPixel
pixel_pin = board.D18  # Adjust this if your NeoPixel is connected to a different pin
num_pixels = 7
pixels = neopixel.NeoPixel(pixel_pin, num_pixels, brightness=1.0, auto_write=False)

# Shared variable to hold target values
target_values = {
    'lux': None,
    'R': None,
    'G': None,
    'B': None,
}

# Lock for thread-safe updates
target_lock = threading.Lock()

# Function to adjust NeoPixels to match target values
def adjust_neopixels():
    # Initial RGB values
    current_pixel_values = [0, 0, 0]
    while True:
        with target_lock:
            # Skip adjustment if no target values are set
            if not all(value is not None for value in target_values.values()):
                time.sleep(0.1)
                continue

            # Read current sensor values
            current_lux = veml7700.lux
            current_r, current_g, current_b = tcs.color_rgb_bytes

            # Print current sensor readings
            print('Current Sensor Readings:')
            print(f'Lux: {current_lux}')
            print(f'RGB: {current_r}, {current_g}, {current_b}')

            # Calculate errors
            lux_error = target_values['lux'] - current_lux
            r_error = target_values['R'] - current_r
            g_error = target_values['G'] - current_g
            b_error = target_values['B'] - current_b

            # Simple proportional control coefficients
            k_lux = 0.01  # Adjust as needed
            k_rgb = 0.1   # Adjust as needed

            # Adjust brightness based on lux error
            brightness_adjustment = k_lux * lux_error
            new_brightness = pixels.brightness + brightness_adjustment
            pixels.brightness = min(max(new_brightness, 0.0), 1.0)

            # Adjust RGB values based on color errors
            adjusted_r = current_pixel_values[0] + k_rgb * r_error
            adjusted_g = current_pixel_values[1] + k_rgb * g_error
            adjusted_b = current_pixel_values[2] + k_rgb * b_error

            # Clamp RGB values between 0 and 255
            adjusted_r = int(min(max(adjusted_r, 0), 255))
            adjusted_g = int(min(max(adjusted_g, 0), 255))
            adjusted_b = int(min(max(adjusted_b, 0), 255))

            # Update current pixel values
            current_pixel_values = [adjusted_r, adjusted_g, adjusted_b]

            # Update NeoPixels
            for i in range(num_pixels):
                pixels[i] = (adjusted_r, adjusted_g, adjusted_b)
            pixels.show()

        # Sleep for a short interval before next adjustment
        time.sleep(0.1)

# HTTP request handler
class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        # Determine the length of the incoming data
        content_length = int(self.headers.get('Content-Length', 0))
        # Read the data from the request
        post_data = self.rfile.read(content_length)
        try:
            # Attempt to parse the data as JSON
            json_data = json.loads(post_data)
            # Debug: Print the received JSON data
            print("Received JSON data:")
            print(json.dumps(json_data, indent=4))

            with target_lock:
                # Update target values based on received JSON
                if 'lux' in json_data:
                    target_values['lux'] = json_data['lux']
                if 'R' in json_data:
                    target_values['R'] = json_data['R']
                if 'G' in json_data:
                    target_values['G'] = json_data['G']
                if 'B' in json_data:
                    target_values['B'] = json_data['B']

            # Send a 200 OK response
            self.send_response(200)
            self.end_headers()
            self.wfile.write(b'JSON data received successfully.')
        except json.JSONDecodeError:
            # Handle invalid JSON data
            self.send_response(400)
            self.end_headers()
            self.wfile.write(b'Invalid JSON data received.')
            print('Received invalid JSON data')

    def log_message(self, format, *args):
        # Override to prevent default logging to stderr
        return

def run(server_class=HTTPServer, handler_class=SimpleHTTPRequestHandler, port=8000):
    # Server settings
    server_address = ('', port)  # Listen on all interfaces, specified port
    httpd = server_class(server_address, handler_class)
    print(f'Starting HTTP server on port {port}...')
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print('Shutting down server.')
        httpd.server_close()
        # Turn off the NeoPixels before exiting
        pixels.fill((0, 0, 0))
        pixels.show()
        sys.exit(0)

# Start the adjustment thread
adjustment_thread = threading.Thread(target=adjust_neopixels)
adjustment_thread.daemon = True
adjustment_thread.start()

# Run the HTTP server
if __name__ == '__main__':
    run()
