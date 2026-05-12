from flask import Flask, render_template,request, redirect, url_for, jsonify
import json

app = Flask(__name__)
app.config['SECRET_KEY'] = ''

data = {'test': []}
@app.route('/', methods = ['POST','GET'])
def hello_world():  # put application's code here
    if request.method == 'POST':
        # if we press the turn on button
        if request.form['submit'] == 'Turn On':
            print('TURN ON')

        # if we press the turn off button
        elif request.form['submit'] == 'Turn Off':
            print('TURN OFF')
        else:
            pass

    return render_template('Test.html')

@app.route('/data', methods=['POST'])
def receive_data():
    try:
        content = request.get_json()

        test = content['test']


        data['test'].append(test)


        print(f"Received data: test={test}")
        return jsonify({'success': True})
    except Exception as e:
        print(f"Error receiving data: {str(e)}")
        return jsonify({'success': False, 'error': str(e)})


@app.route('/get_data')
def get_data():
    return jsonify(data)


if __name__ == '__main__':
    app.run(host="0.0.0.0")
