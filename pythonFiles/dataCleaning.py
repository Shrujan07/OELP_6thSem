#CREATING NEW CSV FILE TAKING AVERAGE FOR VARIOUS PARAMETERS FOR A SINGLE TIME PERIO
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os


def dataExtraction(path):
	obd = pd.read_csv(path,index_col=False)
	

	#print(obd.columns)
	'''
	obd.columns = ['ENGINE_RUN_TINE', 'ENGINE_RPM', 'VEHICLE_SPEED',
					'THROTTLE', 'ENGINE_LOAD', 'COOLANT_TEMPERATURE',
					'LONG_TERM_FUEL_TRIM_BANK_1', 'SHORT_TERM_FUEL_TRIM_BANK_1',
					'INTAKE_MANIFOLD_PRESSURE', 'FUEL_TANK', 'ABSOLUTE_THROTTLE_B',
					'PEDAL_D', 'PEDAL_E', 'COMMANDED_THROTTLE_ACTUATOR',
					'FUEL_AIR_COMMANDED_EQUIV_RATIO', 'ABSOLUTE_BAROMETRIC_PRESSURE',
					'RELATIVE_THROTTLE_POSITION', 'INTAKE_AIR_TEMP',
					'TIMING_ADVANCE', 'CATALYST_TEMPERATURE_BANK1_SENSOR1',
					'CATALYST_TEMPERATURE_BANK1_SENSOR2', 'CONTROL_MODULE_VOLTAGE',
					'COMMANDED_EVAPORATIVE_PURGE', 'TIME_RUN_WITH_MIL_ON',
					'TIME_SINCE_TROUBLE_CODES_CLEARED',
					'DISTANCE_TRAVELED_WITH_MIL_ON', 'WARM_UPS_SINCE_CODES_CLEARED'
				  ]
	'''
	#print(obd.isna().sum())
	obd = obd.drop(columns=['TIME_SINCE_TROUBLE_CODES_CLEARED ()','DISTANCE_TRAVELED_WITH_MIL_ON ()', 'WARM_UPS_SINCE_CODES_CLEARED ()'])
	obd.dropna(inplace=True)
	print(obd.head())
	print(obd.tail())
	return obd

def dataSmoothing(df):
	time = np.array(df['ENGINE_RUN_TINE ()'])
	l = 0
	cnt = 0
	new_df = pd.DataFrame()
	
	for i in range(time.shape[0]-1):
		
		if time[i+1] != time[i]:
			prev = l+1
			l = i
			sub = df.loc[prev:l,:]
			n = sub.shape[0]
			s = dict()
			for j in sub.columns:
				s[j] = float(sub[j].sum())/n
			
			data = pd.Series(s)
			new_df = new_df.append(data,ignore_index=True)
	else:
		sub = df.loc[l+1:,:]
		n = sub.shape[0]
		s = dict()
		for j in sub.columns:
			s[j] = float(sub[j].sum())/n
			
		data = pd.Series(s)
		new_df = new_df.append(data,ignore_index=True)
		
		
	new_df = new_df[df.columns]
	
	return new_df
			
			
	
'''			
for i in os.listdir('/home/rohit/UnsupervisedLearning/DataSet_2'):
	obd = dataExtraction('/home/rohit/UnsupervisedLearning/DataSet_2/'+ i)
	obd.to_csv(r'/home/rohit/UnsupervisedLearning/OBD_Dataset/'+ i, index = False)
'''
#for i in os.listdir('/home/rohit/UnsupervisedLearning/DataSet_2'):
	
obd = dataExtraction('/home/rohit/UnsupervisedLearning/DataSet_2/' + 'live06.csv')
	#try:
		#print(f'{i}')
new_obd = dataSmoothing(obd)
new_obd.to_csv('/home/rohit/UnsupervisedLearning/final_OBD/' + 'live06.csv'	,index=False)
	#except(Exception):
	#	print(f"Error in  {i}")
	
	
#obd.plot(x='ENGINE_RUN_TINE',y=['LONG_TERM_FUEL_TRIM_BANK_1', 'SHORT_TERM_FUEL_TRIM_BANK_1','COMMANDED_EVAPORATIVE_PURGE'])
