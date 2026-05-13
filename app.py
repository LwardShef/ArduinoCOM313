from flask import Flask, render_template,request, redirect, url_for, jsonify
import json

app = Flask(__name__)
app.config['SECRET_KEY'] = ''

data = {'Pattern': "Solid", 'Temperature': 0}
@app.route('/', methods = ['POST','GET'])
def hello_world():  # put application's code here
    if request.method == 'POST':
        # if we press the turn on button
        if request.form['submit'] == 'Off':
            data['Pattern'] = "Off"
        # if we press the turn off button
        elif request.form['submit'] == 'Solid Colour':
            data['Pattern'] = "Solid"
        elif request.form['submit'] == 'Rainbow':
            data['Pattern'] = "Rainbow"
        elif request.form['submit'] == 'Chase':
            data['Pattern'] = "Chase"
        elif request.form['submit'] == 'Flame':
            data['Pattern']= "Flame"
        else:
            pass
    return render_template('Home.html', temperature = data['Temperature'])

@app.route('/send_data', methods=['POST'])
def receive_data():
    try:
        content = request.get_json()

        data['Pattern'] = content['Pattern']
        data['Temperature'] = content['Temperature']

        print(f"Received data: Pattern={data['Pattern']}, Temperature={data['Temperature']}")
        return jsonify({'success': True})
    except Exception as e:
        print(f"Error receiving data: {str(e)}")
        return jsonify({'success': False, 'error': str(e)})


@app.route('/get_data', methods = ['POST','GET'])
def get_data():
    return jsonify(data)


if __name__ == '__main__':
    app.run()
