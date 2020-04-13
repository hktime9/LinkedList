import socket

def mergeSort(array):
	if(len(array)>1):
		mid= int(len(array)/2)
		left= array[mid:]
		right= array[:mid]
		mergeSort(left)
		mergeSort(right)
		i=j=k= 0
		while((i<len(left)) and (j<len(right))):
			if(left[i]<right[j]):
				array[k]= left[i]
				i= i+1
			else:
				array[k]= right[j]
				j= j+1
			k= k+1

		while(i<len(left)):
			array[k]= left[i]
			i= i+1
			k= k+1

		while(j<len(right)):
			array[k]= right[j]
			j= j+1
			k= k+1
	return array

def readArray(filename):
	returnArray= []
	with open(filename) as file:
		for line in file:
			returnArray.append(line)
	string= returnArray[0]
	string= string.split(" ")
	string.pop(-1)
	returnArray= []
	for i in range(0, len(string)):
		returnArray.append(int(string[i]))
	return returnArray

def writeArray(filename,array):
	string= str(len(array))+": "
	for i in range(0,len(array)):
		string= string+str(array[i])+" "
	f = open(filename, "w")
	f.write(string)
	f.close()

def alertMaster():
	s= socket.socket()
	s.connect(('127.0.0.1', 5000))

array= readArray("arrayInput.txt")
sort= mergeSort(array)
writeArray("arrayOutput.txt", sort)
alertMaster()