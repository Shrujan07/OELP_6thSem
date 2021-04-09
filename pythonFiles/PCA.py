import pandas as pd
from sklearn.decomposition import PCA
import DataPreprocessing as dp
import sys
import numpy as np
import matplotlib.pyplot as plt
from sklearn.preprocessing import StandardScaler
from sklearn.cluster import KMeans
from scipy.stats import pearsonr


#################################################################################################
#################################################################################################
#################################################################################################

def implementClustering(principal_df):
	X_df = pd.DataFrame(principal_df)

	principal_df = StandardScaler().fit_transform(X_df)
	kmeans = KMeans(n_clusters=3, init='k-means++')
	y_kmeans3 = kmeans.fit_predict(principal_df)
	print(y_kmeans3)
	cent = kmeans.cluster_centers_
	print(cent)
	plt.figure(figsize=(10,7))
	
	X = np.array(principal_df)
	plt.scatter(X[:,0],X[:,1],c=y_kmeans3,cmap='rainbow')
	plt.title('K_means_clustering')
	plt.xlabel('PC1')
	plt.ylabel('PC2')
	plt.show()
	
		
def loading_matrix(pca_model):
	variables_name=['V60','Vmn','Vsd','Asd','A+mn','A+sd','Br_mn','Br_sd','W']
	mat = pd.DataFrame(pca_model.components_,columns=variables_name)
	print(np.transpose(mat))

def plot_principalComponents(pca_train):
	plt.figure(figsize=(8,6))
	plt.title("PCA for Drivability")
	plt.scatter(pca_train[:,0],pca_train[:,1],cmap='rainbow')
	plt.xlabel('PC1')
	plt.ylabel('PC2')
	plt.show()

def correlation(X,Y):
	return pearsonr(X,Y)[0]
	
if __name__=='__main__':
	dataset = pd.DataFrame(dp.X_norm)
	#print(dataset)
	pca_obd = PCA(n_components=2)
	principal_comp = pca_obd.fit(dp.X_norm)
	principal_comp = pca_obd.fit_transform(dp.X_norm)
	############# PRINTING THE TYPE ##########################################
	print(type(principal_comp))
	principal_df = pd.DataFrame(data=principal_comp,columns=['PC1','PC2'])
	print(principal_df)
	X = dp.X
	
	
	###################################################################################
	###############  CALCULAING CORRELATION MATRIX ####################################
	###################################################################################
	corr_matrix = []
	for i in range(X.shape[1]):
		temp = []
		for j in range(principal_comp.shape[1]):
			temp.append(correlation(X[:,i],principal_comp[:,j]))
		
		corr_matrix.append(temp)
	
	corr_matrix = np.array(corr_matrix)
	print(pd.DataFrame(corr_matrix,index= ['V60','Vmn','Vsd','Asd','A+mn','A+sd','Br_mn','Br_sd','W'],columns=['PC1','PC2']))
			
		
	###################################################################################
	##############  CALCULATINg VARIANCE RETAINED  ####################################
	###################################################################################
	
	print("Amount of data held after Dimensionality Reduction")
	print(sum(pca_obd.explained_variance_ratio_)*100)
	
	#RCA(principal_comp)
	#plot_principalComponents(principal_comp)

	
	#loading_matrix(pca_model)
	implementClustering(principal_df)
	
