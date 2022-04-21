# save this as app.py
from flask import Flask, request, render_template
from flask_socketio import SocketIO, send, emit

app = Flask(__name__, static_folder="./", template_folder="./")
socketio = SocketIO(app,cors_allowed_origins="*")

@app.route("/")
def index():
    return render_template('index.html')

@app.route('/test', methods=['GET'])
def test():
    
    data_list = ['temp', 'humidity', 'imu', 'gps']
    data_res = []
    
    if request.method == 'GET':
        
        for data in data_list:
            data_res.append(request.args.get(data))
        
        output = ''
        
        for data_name, data in zip(data_list, data_res):
            if data != None:
                output += '<h1>The {:s} value is: {}</h1>\n'.format(data_name, data)
        
        socketio.emit('update', output, broadcast=True)
        
        #return output
        return "response sent!"

@socketio.on('hello')
def handle_hello(data):
    print(data)
    socketio.emit('write_back', broadcast=True)
    #return render_template('index2.html')


if __name__ == '__main__':
    # socketio.run(app, host= '128.173.76.190', port=9000, debug=True)
    # socketio.run(app, host= '127.0.0.1', port=9000, debug=True)
    # socketio.run(app, host= '172.20.10.4', port=9000, debug=True)
    socketio.run(app, host= '10.0.1.4', port=9000, debug=True)
    