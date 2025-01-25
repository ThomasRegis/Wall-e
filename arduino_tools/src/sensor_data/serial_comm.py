import serial

def lire_port_serie(port, baudrate=9600):
    try:
        # Ouverture du port série
        ser = serial.Serial(port, baudrate)
        print("Port série ouvert sur", port)
        
        # Lecture des données
        data = ser.Read().split(',')
        return data  
        # Affichage des données lues
        print("Données reçues:", data)
    
    except serial.SerialException as e:
        print("Erreur lors de l'ouverture du port série :", e)
    finally:
        # Fermeture du port série
        if ser.is_open:
            ser.close()
            print("Port série fermé.")


