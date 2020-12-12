from flask import Flask, request

app = Flask(__name__)

@app.route("/", methods=["GET", "POST"])
def func():
    print(request.headers["X-Esp8266-Version"])
    return str(request.headers)

app.run(host="0.0.0.0", port="5000", debug=True)