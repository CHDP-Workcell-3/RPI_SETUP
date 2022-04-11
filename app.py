# save this as app.py
from flask import Flask

app = Flask(__name__)

@app.route("/")
def hello():
    return "Hello, World!"

@app.route('/test', methods=['POST', 'GET'])
def test():
    if request.method == 'POST' or request.method == 'GET':
        language = request.form.get('language')
        framework = request.form.get('framework')
        return '''
                  <h1>The language value is: {}</h1>
                  <h1>The framework value is: {}</h1>'''.format(language, framework)


if __name__ == '__main__':
    app.run(host= '128.173.76.190', port=9000, debug=True)