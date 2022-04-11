# save this as app.py
from flask import Flask, request, render_template
from flask_socketio import SocketIO, send, emit

app = Flask(__name__)
socketio = SocketIO(app,cors_allowed_origins="*")

@app.route("/")
def index():
    return "base site!"
    # return render_template("index.html")

@app.route('/test', methods=['POST', 'GET'])
def test():
    if request.method == 'GET':
        language = request.args.get('data')
        framework = request.args.get('framework')
        
        socketio.emit('update',
        '''
        <h1>The language value is: {}</h1>
        <h1>The framework value is: {}</h1>
        '''.format(language, framework), broadcast=True)
        
        print("sdfhjksdfjkhdfshjksdf")
        
        socketio.emit('update', data="passing data along", broadcast=True)
        
        return '''
                  <h1>The language value is: {}</h1>
                  <h1>The framework value is: {}</h1>'''.format(language, framework)
    if request.method == 'POST':
        language = request.form.get('data')
        framework = request.form.get('framework')
        return '''
                  <h1>The language value is: {}</h1>
                  <h1>The framework value is: {}</h1>'''.format(language, framework)

@socketio.on('update')
def update(data):
    print('Current Value', data)
    return("Stuff happened")


if __name__ == '__main__':
    app.run(host= '128.173.76.190', port=9000, debug=True)