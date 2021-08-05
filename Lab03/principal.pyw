from ventana import *
from serial import *
from    threading import Thread
import sys

puerto = None # se usara para la conexion
lista = []

class VentanaPrincipal(QtWidgets.QWidget, Ui_Form):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        
        self.pushButton.clicked.connect(self.conectar)

        self.btnEnviar.clicked.connect(self.enviar)
    def conectar(self):
        hilo = Thread(target=conectar, daemon=True , args=(self.txtPuerto.text(),9600))
        hilo.start()

    def potenciometro(self):
        global puerto
        puerto.write("N".encode("ascii"))

    def compu(self):
        global puerto
        puerto.write("P".encode("ascii"))

    def secuecia(self):
        global puerto
        puerto.write("S".encode("ascii"))

    def guardar(self):
        global puerto
        puerto.write("G".encode("ascii"))

    def borrar(self):
        global puerto
        puerto.write("E".encode("ascii"))

    def enviar(self):
        global puerto
        puerto.write((self.txtDatos.text()+'\n').encode("ascii"))

def conectar(name, rate):
    global puerto, lista, Form
    try:
        puerto = Serial(port=name, baudrate=rate)
    except:
        pass
    while True:
        palabra = puerto.readline().decode("ascii")
       
        print(lista)
        Form.lblS1.setText(palabra)


app = QtWidgets.QApplication(sys.argv)
Form = VentanaPrincipal()
Form.show()
sys.exit(app.exec_())
