import serial

class PortSerie:
    def __init__(self, port, baudrate=9600):
        self.port = port
        self.baudrate = baudrate

    def open_port(self):
        try:
            self.ser = serial.Serial(self.port, self.baudrate)
            print("Port série ouvert sur", self.port)
        except serial.SerialException as e:
            print("Erreur lors de l'ouverture du port série :", e)
    
    def lire_donnees(self):
        if self.ser and self.ser.is_open:
            try:
                #data = self.ser.readline().decode().strip("1D")
                self.ser.flushInput()
                data = self.ser.readline()
                data = data.decode('utf-8')
                data = data.rstrip()
                
                print("Données reçues:", data)
                return data
            except serial.SerialException as e:
                print("Erreur lors de la lecture des données :", e)
        else:
            print("Le port série n'est pas ouvert.")


