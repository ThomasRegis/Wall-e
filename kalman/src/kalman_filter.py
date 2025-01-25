import numpy as np
import math
import matplotlib.pyplot as plt

class Kalman:
    def __init__(self, yaw0, pitch0, roll0, q,r):
        self.transition = np.array([[1, 0, 0],
                                    [0, 1, 0],
                                    [0, 0, 1]])
        self.state = ([[yaw0],
                       [pitch0],
                       [roll0]])
        
        self.observation = np.array([[1, 0, 0],
                                    [0, 1, 0],
                                    [0, 0, 1]])
        
        self.Q = np.array([[q, 0, 0],
                           [0, q, 0],
                           [0, 0, q]])
        
        self.R = np.array([[r, 0, 0],
                           [0, r, 0],
                           [0, 0, r]])
        
        self.cov = np.array([[10**6, 0, 0],
                           [0, 10**6, 0],
                           [0, 0, 10**6]])
        self.pitch_vals = []
        self.roll_vals = []
        self.pitch_corrected_vals = []
        self.roll_corrected_vals = []
        self.pluserrorpitch = []
        self.pluserrorroll = []

 
    def prediction(self, yaw_dot,pitch_dot,roll_dot,dt):
        self.state = np.dot(self.transition,self.state) + np.dot(np.array([[dt, 0, 0],[0,dt, 0],[0, 0, dt]]),np.array([[yaw_dot],[pitch_dot],[roll_dot]]))
        self.cov = np.dot(np.dot(self.transition,self.cov),self.transition.T) + self.Q
        if len(self.pluserrorroll) < 100:
            self.pluserrorroll.append(math.sqrt(self.cov[2][2]))
            self.pluserrorpitch.append(math.sqrt(self.cov[1][1]))

    def update (self,yaw,pitch,roll):
        
        k = np.dot(np.dot(self.observation,self.cov),self.observation.T) + self.R
        K = np.dot(np.dot(self.cov,self.observation.T),np.linalg.inv(k))
        
        self.state = self.state + np.dot(K,np.array([[yaw],[pitch],[roll]])-np.dot(self.observation,self.state))
        I = np.eye(self.observation.shape[1])
        self.cov = np.dot(I -np.dot(K,self.observation),self.cov)

        if len(self.pitch_vals) < 100:
            self.pitch_corrected_vals.append(self.state[1, 0])  # pitch
            self.roll_corrected_vals.append(self.state[2, 0])   # roll
            self.pitch_vals.append(pitch) # copie de l'état complet
            self.roll_vals.append(roll)
        
        else :
            
            
            plt.figure(figsize=(10, 5)) 

            iterations = range(len(self.pitch_vals))
             
             # Graphique pour pitch
            plt.subplot(1, 2, 1)
            plt.plot(iterations, self.pitch_vals, label='Pitch measurement')
            plt.plot(iterations, self.pitch_corrected_vals, label=' Pitch angle corrected', color='green')
            plt.plot(iterations, self.pluserrorpitch, label='Error', color='red')

            # Affichage de la bande d'incertitude pour le pitch
            upper_bound = np.array(self.pluserrorpitch)
            lower_bound = - np.array(self.pluserrorpitch)
            #plt.fill_between(iterations, lower_bound, upper_bound, color='gray', alpha=0.2, label='Error')

            plt.title("Kalman filter: Pitch")
            plt.xlabel("Itération")
            plt.ylabel("Pitch")
            plt.legend()

            plt.ylim(top=2)

            plt.ylim(bottom = -2)

            plt.tight_layout()
            plt.show()  

            

# Créer une instance de la classe Voiture
# Appeler une méthode de la classe Voiture sur l'instance voiture1

