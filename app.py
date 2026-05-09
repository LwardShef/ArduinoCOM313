#from flask import Flask, request, render_template
import socketio
from flask import Flask, render_template,request, redirect, url_for
from flask_socketio import SocketIO, send, emit, join_room, leave_room
import time
app = Flask(__name__)
app.config['SECRET_KEY'] = ''
socketio=SocketIO(app)
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

    return render_template('Home.html')

@socketio.on('connect')
def test_connect():
    print('Client connected')


if __name__ == '__main__':
    app.run()
