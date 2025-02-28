from flask import Flask, request, jsonify
from flask_cors import CORS
# import openai
import re
import subprocess
import google.generativeai as genai
GOOGLE_API_KEY = "AIzaSyBVCO293mKf1UOWVQoKFev812x7JmU3TPc"
genai.configure(api_key=GOOGLE_API_KEY)
    
app = Flask(__name__)
CORS(app) # Cho phép tất cả các domain truy cập API

def generate_code(request):
    prompt = f"Tạo một sketch Arduino hoàn chỉnh cho board ESP32 với màn hình OLED SSD1306 (128x64) để {request}. Sketch phải bao gồm thư viện ArduinoOTA để cập nhật qua OTA. Đảm bảo bao gồm tất cả thư viện cần thiết, hàm setup và loop, và phần OTA được cấu hình đúng."
    model = genai.GenerativeModel('gemini-2.0-flash')
    response = model.generate_content(prompt)

    return response.text.strip()

def extract_code(text):
    """Trích xuất các khối mã từ văn bản."""
    code_blocks = re.findall(r"`(?:arduino||cpp||python)?\n(.*?)\n`", text, re.DOTALL)
    return "".join(code_blocks)

def compile_code(code, output_file):
    with open('sketch.ino', 'w') as f:
        f.write(code)
    subprocess.run(['arduino-cli', 'compile', '--fqbn', 'esp32:esp32:esp32', 'sketch.ino', '-o', output_file])

def deploy_ota(binary_file, ip_address):
    subprocess.run(['arduino-cli', 'upload', '-p', f'{ip_address}:8266', '--fqbn', 'esp32:esp32:esp32', 'sketch.ino'])

@app.route('/generate', methods=['POST'])
def generate_and_deploy():
    data = request.get_json()
    user_request = data['request']
    print(user_request)
    try:
        code = generate_code(user_request)
        print(code)
        code = extract_code(code)
        print('\n-----------------------------------\n\n')
        print(code)
        # compile_code(code, 'output.bin')
        # deploy_ota('output.bin', '192.168.1.100')  # Thay bằng IP thực tế
        return jsonify({'message': 'Mã đã được tạo, biên dịch và triển khai thành công.'})
    except Exception as e:
        return jsonify({'message': f'Lỗi: {str(e)}'})

if __name__ == '__main__':
    app.run(debug=True)