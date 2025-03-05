from flask import Flask, request, jsonify
from flask_cors import CORS
import sys
# import openai
import re
import subprocess
import google.generativeai as genai

GOOGLE_API_KEY = "AIzaSyBVCO293mKf1UOWVQoKFev812x7JmU3TPc"
genai.configure(api_key=GOOGLE_API_KEY)

BOARD_FQBN = "arduino:avr:uno"
PORT = "COM5"  # Thay bằng cổng thực tế Arduino của bạn
SKETCH_PATH = "sketch/sketch.ino"  # Đường dẫn chứa file .ino

    
app = Flask(__name__)
CORS(app) # Cho phép tất cả các domain truy cập API

# def run_cmd(cmd):
#     print(f"🔹 Chạy lệnh: {cmd}")
#     result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
#     print(result.stdout)
#     if result.returncode != 0:
#         print(f"❌ Lỗi: {result.stderr}")
#         sys.exit(1)

def generate_code(request):
    prompt = f"Tạo một sketch Arduino hoàn chỉnh cho board Arduino uno để {request}. Sketch phải bao gồm tất cả thư viện cần thiết, hàm setup và loop được cấu hình đúng."
    model = genai.GenerativeModel('gemini-2.0-flash')
    response = model.generate_content(prompt)

    return response.text.strip()

def extract_code(text):
    """Trích xuất các khối mã từ văn bản."""
    code_blocks = re.findall(r"`(?:arduino||cpp||python)?\n(.*?)\n`", text, re.DOTALL)
    return "".join(code_blocks)

def compile_code(code):
    with open(SKETCH_PATH, 'w') as f:
        f.write(code)
    subprocess.run(['arduino-cli', 'compile', '--fqbn', BOARD_FQBN, SKETCH_PATH])

def upload_code(com_port):
    subprocess.run(['arduino-cli', 'upload', '-p', com_port, '--fqbn', BOARD_FQBN, SKETCH_PATH])

@app.route('/generate', methods=['POST'])
def generate_and_deploy():
    data = request.get_json()
    user_request = data['request']
    print(user_request)
    try:
        code = generate_code(user_request)
        # print(code)
        code = extract_code(code)
        print('\n--------------- Gen code -------------\n\n')
        print(code)
        print('\n--------------- Compile ---------------\n\n')
        compile_code(code)
        print('\n---------------- Upload ------------\n\n')
        upload_code(PORT)  # Thay bằng IP thực tế
        return jsonify({'message': 'Mã đã được tạo, biên dịch và triển khai thành công.'})
    except Exception as e:
        return jsonify({'message': f'Lỗi: {str(e)}'})

if __name__ == '__main__':
    app.run(debug=True, port=5000, host='0.0.0.0')