import json

# save this as app.py
from flask import Flask, request, render_template
from flask_socketio import SocketIO, send, emit

app = Flask(__name__, static_folder="", template_folder="./")
socketio = SocketIO(app,cors_allowed_origins="*")

data_list = ['temp', 'humidity', 'imu', 'gps']
collected_data = {data_elem:[] for data_elem in data_list}

@app.route("/")
def index():
    return render_template('index.html')

@app.route('/test', methods=['GET'])
def test():
    
    data_res = []
    
    if request.method == 'GET':
        
        for data in data_list:
            data_res.append(request.args.get(data))
        
        output = ''
        for data_name, data in zip(data_list, data_res):
            if data != None:
                collected_data[data_name].append(data)
                output += '<h1>The {:s} value is: {}</h1>\n'.format(data_name, data)
        
        socketio.emit('update', json.dumps(collected_data), broadcast=True)
        
        #return output
        return "response sent!"

@socketio.on('connect')
def handle_hello(data):
    socketio.emit('update', json.dumps(collected_data), broadcast=True)


if __name__ == '__main__':
    # socketio.run(app, host= '10.0.1.4', port=9000, debug=True)
    socketio.run(app, host= 'localhost', port=9000, debug=True)
    