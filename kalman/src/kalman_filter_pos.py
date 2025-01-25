import numpy as np

class KalmanPos:
    def __init__(self, x0, y0, z0, vx0,vy0,vz0,q,r):
        self.state = ([[x0],
                       [y0],
                       [z0],
                       [vx0],
                       [vy0],
                       [vz0]])
        
        self.observation = np.array([[1, 0, 0, 0, 0, 0],
                                     [0, 1, 0, 0, 0, 0],
                                     [0, 0, 1, 0, 0, 0]])
        
        self.Q = np.array([[q, 0, 0, 0, 0, 0],
                           [0, q, 0, 0, 0, 0],
                           [0, 0, q, 0, 0, 0],
                           [0, 0, 0, q, 0, 0],
                           [0, 0, 0, 0, q, 0],
                           [0, 0, 0, 0, 0, q]])
        
        self.R = np.array([[r, 0, 0],
                           [0, r, 0],
                           [0, 0, r]])
        
        self.cov = np.array([[1, 0, 0, 0, 0, 0],
                             [0, 1, 0, 0, 0, 0],
                             [0, 0, 1, 0, 0, 0],
                             [0, 0, 0, 1, 0, 0],
                             [0, 0, 0, 0, 1, 0],
                             [0, 0, 0, 0, 0, 1]])

    def prediction(self, ax, ay, az, dt):
        self.transition = np.array([[1, 0, 0, dt, 0, 0],
                                    [0, 1, 0, 0, dt, 0],
                                    [0, 0, 1, 0, 0, dt],
                                    [0, 0, 0, 1, 0, 0],
                                    [0, 0, 0, 0, 1, 0],
                                    [0, 0, 0, 0, 0, 1]])
        self.state = np.dot(self.transition,self.state) + dt*np.array([[0],[0],[0],[ax],[ay],[az]])#
        print(self.state)
        self.cov = np.dot(np.dot(self.transition,self.cov),self.transition.T) + self.Q

    def update (self,x,y,z):
        
        k = np.dot(np.dot(self.observation,self.cov),self.observation.T) + self.R
        K = np.dot(np.dot(self.cov,self.observation.T),np.linalg.inv(k))
        
        self.state = self.state + np.dot(K,np.array([[x],[y],[z]])-np.dot(self.observation,self.state))
        I = np.eye(self.observation.shape[1])
        self.cov = np.dot(I -np.dot(K,self.observation),self.cov)

ex = KalmanPos(0,0,0,5,5,5,0.1,0.05)
ex.prediction(-2,-2,-2,0.5)
ex.update(2.2,2.2,2.2)
print (ex.state)
print(ex.cov)