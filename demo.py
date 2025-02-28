from flask import Flask, request, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)  # Cho phép tất cả các domain truy cập API

@app.route('/generate', methods=['POST'])
def generate_code():
    try:
        data = request.get_json()  # Lấy dữ liệu từ request
        user_request = data.get('request', '')  # Lấy nội dung từ request

        # Xử lý yêu cầu, ví dụ: tạo mã "Hello World"
        if "hello" in user_request.lower():
            generated_code = "print('Hello, World!')"  
        else:
            generated_code = "print('Không hiểu yêu cầu')"

        return jsonify({"message": "Code đã được tạo!", "code": generated_code})
    except Exception as e:
        return jsonify({"message": "Lỗi xử lý!", "error": str(e)}), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
