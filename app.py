from flask import Flask, render_template, jsonify, request
import requests

app = Flask(__name__)

# ESP8266 IP (update with your ESP's IP)
ESP_IP = "http://10.134.40.224"
MAX_SLOTS = 8
occupied = 0  

@app.route('/')
def index():
    return render_template('index.html', max_slots=MAX_SLOTS)

@app.route('/get_count')
def get_count():
    global occupied
    try:
        # Fetch live count from ESP8266
        response = requests.get(f"{ESP_IP}/count", timeout=2)
        occupied = int(response.text.strip())
        print(f"✅ Received count from ESP: {occupied}")
    except Exception as e:
        print(f"⚠️ Could not fetch ESP count: {e}")
        # keep last known count
    return jsonify({"count": occupied})

@app.route('/buy/<int:slot>')
def buy(slot):
    return render_template('buy.html', slot=slot)

if __name__ == "__main__":
     app.run(host="0.0.0.0", port=5000, debug=True)

