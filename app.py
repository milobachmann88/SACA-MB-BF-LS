from flask import Flask, request, jsonify

app = Flask(__name__)

lluvia = False

@app.route('/')
def hello_world():
   return '<p>Pronóstico del clima</p>'

@app.route('/api/clima', methods=['GET'])
def consultar_clima():
    print(f"La ESP32 se conectó desde: {request.remote_addr}")
    
    return jsonify({"lluvia": lluvia})

@app.route('/api/clima', methods=['POST'])
def actualizar_clima():
    global lluvia
    datos = request.json

    lluvia = datos['lluvia']

    print(f"Está lloviendo?: {lluvia}")
    return {'status': 'OK'}

app.run(host='0.0.0.0', port=5000, debug=True)