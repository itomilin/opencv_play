# Libraries
import cv2
import numpy as np
import matplotlib.pyplot as plt

# open the image
plt.rcParams['toolbar'] = 'None'
f = cv2.imread('../imgs/img1.jpeg', 0)

# transform image into freq. domain and shifted
F = np.fft.fft2(f)
Fshift = np.fft.fftshift(F)

fig1 = plt.figure(figsize=plt.figaspect(0.75))
ax1 = fig1.add_subplot(1, 1, 1)
im1 = plt.imshow(np.log1p(np.abs(Fshift)), cmap='gray')
plt.axis('off')

#plt.imshow(np.log1p(np.abs(Fshift)), cmap='gray')
#plt.axis('off')
#plt.close()

# Butterwort Low Pass Filter
M,N = f.shape
H = np.zeros((M,N), dtype=np.float32)
D0 = 10 # cut of frequency
n = 1 # order 

for u in range(M):
    for v in range(N):
        D = np.sqrt((u-M/2)**2 + (v-N/2)**2)
        H[u,v] = 1 / (1 + (D/D0)**n)
        
fig2 = plt.figure(figsize=plt.figaspect(0.75))
ax2 = fig2.add_subplot(1, 1, 1)
im2 = plt.imshow(H, cmap='gray')
plt.axis('off')

# frequency domain image filters
Gshift = Fshift * H
G = np.fft.ifftshift(Gshift)
g = np.abs(np.fft.ifft2(G))

fig3 = plt.figure(figsize=plt.figaspect(0.75))
ax3 = fig3.add_subplot(1, 1, 1)
im3 = plt.imshow(g, cmap='gray')
plt.axis('off')

# Butterworth High Pass Filter
HPF = np.zeros((M,N), dtype=np.float32)
D0 = 10
n = 1
for u in range(M):
    for v in range(N):
        D = np.sqrt((u-M/2)**2 + (v-N/2)**2)
        HPF[u,v] = 1 / (1 + (D0/D)**n)

fig4 = plt.figure(figsize=plt.figaspect(0.75))
ax4 = fig4.add_subplot(1, 1, 1)
im4 = plt.imshow(HPF, cmap='gray')
plt.axis('off')

# frequency domain image filters
Gshift = Fshift * HPF
G = np.fft.ifftshift(Gshift)
g = np.abs(np.fft.ifft2(G))

fig5 = plt.figure(figsize=plt.figaspect(0.75))
ax5 = fig5.add_subplot(1, 1, 1)
im5 = plt.imshow(g, cmap='gray')
plt.axis('off')

cv2.imshow( "original", f )
plt.show()


while True:
    if cv2.waitKey( 0 ) == 27:
       cv2.destroyAllWindows()
       break
