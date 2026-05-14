from flask import Flask, render_template,request, jsonify
app = Flask(__name__)
app.config['SECRET_KEY'] = ''

data = {'Pattern': "Solid", 'Temperature': 0, 'ButtonPressed' : False}
@app.route('/')
def hello_world():  # put application's code here
    return render_template('Home.html')
@app.route('/send_data', methods=['POST'])
def receive_data():
    try:
        content = request.get_json()
        data['Pattern'] = content['Pattern']
        data['Temperature'] = content['Temperature']
        data['ButtonPressed'] = content['ButtonPressed']

        print(f"Received data: Pattern={data['Pattern']}, Temperature={data['Temperature']}, ButtonPressed={data['ButtonPressed']}")
        return jsonify({'success': True})
    except Exception as e:
        print(f"Error receiving data: {str(e)}")
        return jsonify({'success': False, 'error': str(e)})
@app.route('/get_data', methods = ['POST','GET'])
def get_data():
    return jsonify(data)

@app.route('/get_js_data', methods = ['POST','GET'])
def get_js_data():
    return jsonify(data)

@app.route('/send_js_data', methods = ['POST','GET'])
def send_js_data():
    try:
        content = request.form
        data['Pattern'] = content['Pattern']
        return jsonify({'success': True})
    except Exception as e:
        print(f"Error receiving data: {str(e)}")
        return jsonify({'success': False, 'error': str(e)})

if __name__ == '__main__':
    app.run()
