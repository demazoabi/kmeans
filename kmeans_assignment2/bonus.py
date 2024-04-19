import matplotlib
matplotlib.use('Agg')  # Set the backend to Agg before importing pyplot

import matplotlib.pyplot as plt
from sklearn.datasets import load_iris
from sklearn.cluster import KMeans

iris = load_iris()
X = iris.data
kValues = range(1, 11)
inerValues = []
for k in kValues:
    kmeans = KMeans(n_clusters=k, init='k-means++', random_state=0)
    kmeans.fit(X)
    inerValues.append(kmeans.inertia_)

plt.plot(kValues, inerValues, marker='o')
plt.xlabel('Number of Clusters (k)')
plt.ylabel('Inertia')
plt.title('Elbow Method for Optimal k')
plt.grid(True)

plt.annotate('Elbow', xy=(3, inerValues[2]), xytext=(4, inerValues[3]),
             arrowprops=dict(facecolor='black', shrink=0.05))

plt.savefig('elbow.png')



