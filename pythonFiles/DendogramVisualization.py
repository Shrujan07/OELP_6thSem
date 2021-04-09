from scipy.cluster.hierarchy import dendrogram,linkage
import matplotlib.pyplot as plt
import DataPreprocessing as dp

def plot_dendrogram(X):
	linked = linkage(X,'ward')
	plt.figure(figsize=(10,7))
	dendrogram(linked)
	plt.show()

if __name__=='__main__':
	plot_dendrogram(dp.X_norm)


